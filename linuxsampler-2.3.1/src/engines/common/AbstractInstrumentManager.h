/*
 * Copyright (c) 2014-2022 Christian Schoenebeck
 *
 * http://www.linuxsampler.org
 *
 * This file is part of LinuxSampler and released under the same terms.
 * See README file for details.
 */

#ifndef LS_ABSTRACTINSTRUMENTMANAGER_H
#define LS_ABSTRACTINSTRUMENTMANAGER_H

#include "../InstrumentManager.h"
#include "../../common/ResourceManager.h"
#include "../../common/global_private.h"
#include "InstrumentScriptVM.h"
#include "../InstrumentManagerThread.h"

namespace LinuxSampler {

    typedef ResourceConsumer<VMParserContext> InstrumentScriptConsumer;

    /**
     * Identifies uniquely a compiled script.
     *
     * Note: every engine channel now has its own compiled script object
     * (a.k.a. VMParserContext). Originally a compiled script was shared by
     * multiple engine channels. This was wrong: we cannot share compiled
     * script instances among multiple engine channels (parts), for two
     * reasons:
     *
     * 1. VMParserContext not only encompasses the compiled tree
     *    presentation of the requested script, but also global variables
     *    and we don't want those global variables to be modified by
     *    different sampler parts, as this would not be expected behaviour
     *    by instrument script authors.
     *
     * 2. If there is more than one sampler engine instance (e.g. if there
     *    are multiple audio output device instances) this would even crash,
     *    because each sampler engine instance has its own ScriptVM
     *    instance, and a (VM)ParserContext is always tied to exactly one
     *    ScriptVM instance.
     *
     * We would not be buying much by sharing compiled scripts anyway, as a
     * script usually compiles in couple microseconds and RAM usage is also
     * neglectable.
     */
    struct ScriptKey {
        String code; ///< Script's source code.
        std::map<String,String> patchVars; ///< Patch variables being overridden by instrument.
        EngineChannel* engineChannel; ///< Unique owning engine channel on which the compiled script is/was loaded onto.
        bool wildcardPatchVars; ///< Seldom use: Allows lookup for consumers of a specific script by ignoring all (overridden) patch variables.
        bool wildcardEngineChannel; ///< Seldom use: Allows lookup for consumers of a specific script by ignoring on which engine channel it was loaded.

        inline bool operator<(const ScriptKey& o) const {
            if (wildcardPatchVars && wildcardEngineChannel)
                return code < o.code;
            else if (wildcardPatchVars)
                return code < o.code || (code == o.code && engineChannel < o.engineChannel);
            else if (wildcardEngineChannel)
                return code < o.code || (code == o.code && patchVars < o.patchVars);
            else
                return code < o.code || (code == o.code && (patchVars < o.patchVars || (patchVars == o.patchVars && engineChannel < o.engineChannel)));
        }

        inline bool operator>(const ScriptKey& o) const {
            if (wildcardPatchVars && wildcardEngineChannel)
                return code > o.code;
            else if (wildcardPatchVars)
                return code > o.code || (code == o.code && engineChannel > o.engineChannel);
            else if (wildcardEngineChannel)
                return code > o.code || (code == o.code && patchVars > o.patchVars);
            else
                return code > o.code || (code == o.code && (patchVars > o.patchVars || (patchVars == o.patchVars && engineChannel > o.engineChannel)));
        }

        inline bool operator==(const ScriptKey& o) const {
            if (wildcardPatchVars && wildcardEngineChannel)
                return code == o.code;
            else if (wildcardPatchVars)
                return code == o.code && engineChannel == o.engineChannel;
            else if (wildcardEngineChannel)
                return code == o.code && patchVars == o.patchVars;
            else
                return code == o.code && patchVars == o.patchVars && engineChannel == o.engineChannel;
        }

        inline bool operator!=(const ScriptKey& o) const {
            return !(operator==(o));
        }
    };

    class AbstractInstrumentManager : public InstrumentManager {
    public:
        AbstractInstrumentManager() { }
        InstrumentManagerThread* GetInstrumentManagerThread();
        virtual ~AbstractInstrumentManager() { }

        /**
         * Resource manager for loading and sharing the parsed (executable) VM
         * presentation of real-time instrument scripts. The key used here, and
         * associated with each script resource, is not as one might expect the
         * script name or something equivalent, instead the key used is
         * actually the entire script's source code text (and additionally
         * potentially patched variables). The value (the actual resource) is of
         * type @c VMParserContext, which is the parsed (executable) VM
         * representation of the respective script.
         */
        class ScriptResourceManager : public ResourceManager<ScriptKey, VMParserContext> {
        public:
            ScriptResourceManager() {}
            virtual ~ScriptResourceManager() {}
        protected:
            // implementation of derived abstract methods from 'ResourceManager'
            virtual VMParserContext* Create(ScriptKey key, InstrumentScriptConsumer* pConsumer, void*& pArg);
            virtual void Destroy(VMParserContext* pResource, void* pArg);
            virtual void OnBorrow(VMParserContext* pResource, InstrumentScriptConsumer* pConsumer, void*& pArg) {} // ignore
        } scripts;
    };
    
} // namespace LinuxSampler

#endif // LS_ABSTRACTINSTRUMENTMANAGER_H
