/*
 * Copyright (c) 2014 - 2022 Christian Schoenebeck
 *
 * http://www.linuxsampler.org
 *
 * This file is part of LinuxSampler and released under the same terms.
 * See README file for details.
 */

#include "AbstractInstrumentManager.h"
#include "../AbstractEngine.h"
#include "../AbstractEngineChannel.h"

namespace LinuxSampler {

    // defined in src/engines/InstrumentManager.cpp
    extern InstrumentManagerThread thread;

    VMParserContext* AbstractInstrumentManager::ScriptResourceManager::Create(ScriptKey key, InstrumentScriptConsumer* pConsumer, void*& pArg) {
        AbstractEngineChannel* pEngineChannel = dynamic_cast<AbstractEngineChannel*>(pConsumer);
        return pEngineChannel->pEngine->pScriptVM->loadScript(key.code, key.patchVars);
    }

    void AbstractInstrumentManager::ScriptResourceManager::Destroy(VMParserContext* pResource, void* pArg) {
        delete pResource;
    }

    InstrumentManagerThread* AbstractInstrumentManager::GetInstrumentManagerThread() {
        return &thread;
    }

} // namespace LinuxSampler
