/***************************************************************************
 *                                                                         *
 *   LinuxSampler - modular, streaming capable sampler                     *
 *                                                                         *
 *   Copyright (C) 2003, 2004 by Benno Senoner and Christian Schoenebeck   *
 *   Copyright (C) 2005 - 2008 Christian Schoenebeck                       *
 *   Copyright (C) 2009 Christian Schoenebeck and Grigor Iliev             *
 *   Copyright (C) 2010 - 2017 Christian Schoenebeck and Andreas Persson   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 ***************************************************************************/

#include "../../common/Features.h"
#include "Synthesizer.h"
#include "Profiler.h"
#include "Engine.h"
#include "EngineChannel.h"

#include "Voice.h"

namespace LinuxSampler { namespace gig {

    // sanity checks: fromGigLfoWave() assumes equally mapped enums
    static_assert(int64_t(::gig::lfo_wave_sine) == int64_t(LFO::wave_sine),
                  "enum LFO::wave_t not equally value mapped to libgig's enum ::gig::lfo_wave_t");
    static_assert(int64_t(::gig::lfo_wave_triangle) == int64_t(LFO::wave_triangle),
                  "enum LFO::wave_t not equally value mapped to libgig's enum ::gig::lfo_wave_t");
    static_assert(int64_t(::gig::lfo_wave_saw) == int64_t(LFO::wave_saw),
                  "enum LFO::wave_t not equally value mapped to libgig's enum ::gig::lfo_wave_t");
    static_assert(int64_t(::gig::lfo_wave_square) == int64_t(LFO::wave_square),
                  "enum LFO::wave_t not equally value mapped to libgig's enum ::gig::lfo_wave_t");

    // converts ::gig::lfo_wave_t (libgig) -> LFO::wave_t (LinuxSampler)
    inline LFO::wave_t fromGigLfoWave(::gig::lfo_wave_t wave) {
        // simply assuming equally mapped enums on both sides
        return static_cast<LFO::wave_t>(wave);
    }

    // Returns true for GigaStudio's original filter types (which are resembled
    // by LS very accurately with same frequency response and patch settings
    // behaviour), false for our own LS specific filter implementation types.
    constexpr bool isGStFilterType(::gig::vcf_type_t type) {
        return type == ::gig::vcf_type_lowpass ||
               type == ::gig::vcf_type_lowpassturbo ||
               type == ::gig::vcf_type_bandpass ||
               type == ::gig::vcf_type_highpass ||
               type == ::gig::vcf_type_bandreject;
    }

    Voice::Voice() {
        pEngine = NULL;
        pEG1 = &EG1;
        pEG2 = &EG2;
    }

    Voice::~Voice() {
    }

    EngineChannel* Voice::GetGigEngineChannel() {
        return static_cast<EngineChannel*>(pEngineChannel);
    }

    void Voice::SetEngine(LinuxSampler::Engine* pEngine) {
        Engine* engine = static_cast<Engine*>(pEngine);
        this->pEngine     = engine;
        this->pDiskThread = engine->pDiskThread;
        dmsg(6,("Voice::SetEngine()\n"));
    }

    Voice::SampleInfo Voice::GetSampleInfo() {
        SampleInfo si;
        si.SampleRate       = pSample->SamplesPerSecond;
        si.ChannelCount     = pSample->Channels;
        si.FrameSize        = pSample->FrameSize;
        si.BitDepth         = pSample->BitDepth;
        si.TotalFrameCount  = (uint)pSample->SamplesTotal;

        si.HasLoops       = pRegion->SampleLoops;
        si.LoopStart      = (si.HasLoops) ? pRegion->pSampleLoops[0].LoopStart  : 0;
        si.LoopLength     = (si.HasLoops) ? pRegion->pSampleLoops[0].LoopLength : 0;
        si.LoopPlayCount  = pSample->LoopPlayCount;
        si.Unpitched      = !pRegion->PitchTrack;

        return si;
    }

    Voice::RegionInfo Voice::GetRegionInfo() {
        RegionInfo ri;
        ri.UnityNote = pRegion->UnityNote;
        ri.FineTune  = pRegion->FineTune;
        ri.Pan       = pRegion->Pan;
        ri.SampleStartOffset = pRegion->SampleStartOffset;

        ri.EG2PreAttack        = pRegion->EG2PreAttack;
        ri.EG2Attack           = pRegion->EG2Attack;
        ri.EG2Decay1           = pRegion->EG2Decay1;
        ri.EG2Decay2           = pRegion->EG2Decay2;
        ri.EG2Sustain          = pRegion->EG2Sustain;
        ri.EG2InfiniteSustain  = pRegion->EG2InfiniteSustain;
        ri.EG2Release          = pRegion->EG2Release;

        ri.EG3Attack     = pRegion->EG3Attack;
        ri.EG3Depth      = pRegion->EG3Depth;
        ri.VCFEnabled    = pRegion->VCFEnabled;
        ri.VCFType       = Filter::vcf_type_t(pRegion->VCFType);
        ri.VCFResonance  = pRegion->VCFResonance;

        ri.ReleaseTriggerDecay = 0.01053 * (256 >> pRegion->ReleaseTriggerDecay);

        return ri;
    }

    Voice::InstrumentInfo Voice::GetInstrumentInfo() {
        InstrumentInfo ii;
        ii.FineTune = GetGigEngineChannel()->pInstrument->FineTune;
        ii.PitchbendRange = GetGigEngineChannel()->pInstrument->PitchbendRange;

        return ii;
    }

    double Voice::GetSampleAttenuation() {
        return pRegion->SampleAttenuation;
    }

    double Voice::GetVelocityAttenuation(uint8_t MIDIKeyVelocity) {
        return pRegion->GetVelocityAttenuation(MIDIKeyVelocity);
    }

    double Voice::GetVelocityRelease(uint8_t MIDIKeyVelocity) {
        return pRegion->GetVelocityRelease(MIDIKeyVelocity);
    }

    void Voice::ProcessCCEvent(RTList<Event>::Iterator& itEvent) {
        if (itEvent->Type == Event::type_control_change && itEvent->Param.CC.Controller) { // if (valid) MIDI control change event
            if (pRegion->AttenuationController.type == ::gig::attenuation_ctrl_t::type_controlchange &&
                itEvent->Param.CC.Controller == pRegion->AttenuationController.controller_number) {
                CrossfadeSmoother.update(AbstractEngine::CrossfadeCurve[CrossfadeAttenuation(itEvent->Param.CC.Value)]);
            }
        }
    }

    void Voice::ProcessChannelPressureEvent(RTList<Event>::Iterator& itEvent) {
        if (itEvent->Type == Event::type_channel_pressure) { // if (valid) MIDI channel pressure (aftertouch) event
            if (pRegion->AttenuationController.type == ::gig::attenuation_ctrl_t::type_channelaftertouch) {
                CrossfadeSmoother.update(AbstractEngine::CrossfadeCurve[CrossfadeAttenuation(itEvent->Param.ChannelPressure.Value)]);
            }
        }
    }

    void Voice::ProcessPolyphonicKeyPressureEvent(RTList<Event>::Iterator& itEvent) {
        // Not used so far
    }

    uint8_t Voice::MinCutoff() const {
        // If there's a cutoff controller defined then VCFVelocityScale means
        // "minimum cutoff". If there is no MIDI controller defined for cutoff
        // then VCFVelocityScale is already taken into account on libgig side
        // instead by call to pRegion->GetVelocityCutoff(MIDIKeyVelocity).
        return pRegion->VCFVelocityScale;
    }

    // This is called on any cutoff controller changes, however not when the
    // voice is triggered. So the initial cutoff value is retrieved by a call
    // to CalculateFinalCutoff() instead.
    void Voice::ProcessCutoffEvent(RTList<Event>::Iterator& itEvent) {
        if (VCFCutoffCtrl.value == itEvent->Param.CC.Value) return;
        float ccvalue = VCFCutoffCtrl.value = itEvent->Param.CC.Value;

        // if the selected filter type is an official GigaStudio filter type
        // then we preserve the original (no matter how odd) historical GSt
        // behaviour identically; for our own filter types though we deviate to
        // more meaningful behaviours where appropriate
        const bool isGStFilter = isGStFilterType(pRegion->VCFType);

        if (pRegion->VCFCutoffControllerInvert) ccvalue = 127 - ccvalue;
        // interpret "minimum cutoff" not simply as hard limit, rather
        // restrain it to min_cutoff..127 range, but spanned / remapped over
        // the entire controller range (0..127) to avoid a "dead" lower
        // controller zone (that is to avoid a certain CC value range where
        // the controller would not change the cutoff frequency)
        ccvalue = MinCutoff() + (ccvalue / 127.f) * float(127 - MinCutoff());

        float cutoff = CutoffBase * ccvalue;
        if (cutoff > 127.0f) cutoff = 127.0f;

        // the filter implementations of the original GSt filter types take an
        // abstract cutoff parameter range of 0..127, whereas our own filter
        // types take a cutoff parameter in Hz, so remap here:
        // 0 .. 127 [lin] -> 21 Hz .. 18 kHz [x^4] (center @2.2 kHz)
        if (!isGStFilter) {
            cutoff = (cutoff + 29.f) / (127.f + 29.f);
            cutoff = cutoff * cutoff * cutoff * cutoff * 18000.f;
            if (cutoff > 0.49f * pEngine->SampleRate)
                cutoff = 0.49f * pEngine->SampleRate;
        }

        fFinalCutoff = VCFCutoffCtrl.fvalue = cutoff;
    }

    double Voice::CalculateCrossfadeVolume(uint8_t MIDIKeyVelocity) {
        float crossfadeVolume;
        switch (pRegion->AttenuationController.type) {
            case ::gig::attenuation_ctrl_t::type_channelaftertouch:
                crossfadeVolume = Engine::CrossfadeCurve[CrossfadeAttenuation(GetGigEngineChannel()->ControllerTable[128])];
                break;
            case ::gig::attenuation_ctrl_t::type_velocity:
                crossfadeVolume = Engine::CrossfadeCurve[CrossfadeAttenuation(MIDIKeyVelocity)];
                break;
            case ::gig::attenuation_ctrl_t::type_controlchange: //FIXME: currently not sample accurate
                crossfadeVolume = Engine::CrossfadeCurve[CrossfadeAttenuation(GetGigEngineChannel()->ControllerTable[pRegion->AttenuationController.controller_number])];
                break;
            case ::gig::attenuation_ctrl_t::type_none: // no crossfade defined
            default:
                crossfadeVolume = 1.0f;
        }

        return crossfadeVolume;
    }

    double Voice::GetEG1ControllerValue(uint8_t MIDIKeyVelocity) {
        double eg1controllervalue = 0;
        switch (pRegion->EG1Controller.type) {
            case ::gig::eg1_ctrl_t::type_none: // no controller defined
                eg1controllervalue = 0;
                break;
            case ::gig::eg1_ctrl_t::type_channelaftertouch:
                eg1controllervalue = GetGigEngineChannel()->ControllerTable[128];
                break;
            case ::gig::eg1_ctrl_t::type_velocity:
                eg1controllervalue = MIDIKeyVelocity;
                break;
            case ::gig::eg1_ctrl_t::type_controlchange: // MIDI control change controller
                eg1controllervalue = GetGigEngineChannel()->ControllerTable[pRegion->EG1Controller.controller_number];
                break;
        }
        if (pRegion->EG1ControllerInvert) eg1controllervalue = 127 - eg1controllervalue;

        return eg1controllervalue;
    }

    Voice::EGInfo Voice::CalculateEG1ControllerInfluence(double eg1ControllerValue) {
        EGInfo eg;
        // (eg1attack is different from the others)
        if (pRegion->EG1Attack < 1e-8 && // attack in gig == 0
            (pRegion->EG1ControllerAttackInfluence == 0 ||
             eg1ControllerValue <= 10)) { // strange GSt special case
            eg.Attack = 0; // this will force the attack to be 0 in the call to EG1.trigger
        } else {
            eg.Attack  = (pRegion->EG1ControllerAttackInfluence)  ?
                1 + 0.031 * (double) (pRegion->EG1ControllerAttackInfluence == 1 ?
                                      1 : 1 << pRegion->EG1ControllerAttackInfluence) * eg1ControllerValue : 1.0;
        }
        eg.Decay   = (pRegion->EG1ControllerDecayInfluence)   ? 1 + 0.00775 * (double) (1 << pRegion->EG1ControllerDecayInfluence)   * eg1ControllerValue : 1.0;
        eg.Release = (pRegion->EG1ControllerReleaseInfluence) ? 1 + 0.00775 * (double) (1 << pRegion->EG1ControllerReleaseInfluence) * eg1ControllerValue : 1.0;

        return eg;
    }

    double Voice::GetEG2ControllerValue(uint8_t MIDIKeyVelocity) {
        double eg2controllervalue = 0;
        switch (pRegion->EG2Controller.type) {
            case ::gig::eg2_ctrl_t::type_none: // no controller defined
                eg2controllervalue = 0;
                break;
            case ::gig::eg2_ctrl_t::type_channelaftertouch:
                eg2controllervalue = GetGigEngineChannel()->ControllerTable[128];
                break;
            case ::gig::eg2_ctrl_t::type_velocity:
                eg2controllervalue = MIDIKeyVelocity;
                break;
            case ::gig::eg2_ctrl_t::type_controlchange: // MIDI control change controller
                eg2controllervalue = GetGigEngineChannel()->ControllerTable[pRegion->EG2Controller.controller_number];
                break;
        }
        if (pRegion->EG2ControllerInvert) eg2controllervalue = 127 - eg2controllervalue;

        return eg2controllervalue;
    }

    Voice::EGInfo Voice::CalculateEG2ControllerInfluence(double eg2ControllerValue) {
        EGInfo eg;
        eg.Attack  = (pRegion->EG2ControllerAttackInfluence)  ? 1 + 0.00775 * (double) (1 << pRegion->EG2ControllerAttackInfluence)  * eg2ControllerValue : 1.0;
        eg.Decay   = (pRegion->EG2ControllerDecayInfluence)   ? 1 + 0.00775 * (double) (1 << pRegion->EG2ControllerDecayInfluence)   * eg2ControllerValue : 1.0;
        eg.Release = (pRegion->EG2ControllerReleaseInfluence) ? 1 + 0.00775 * (double) (1 << pRegion->EG2ControllerReleaseInfluence) * eg2ControllerValue : 1.0;

        return eg;
    }

    void Voice::InitLFO1() {
        uint16_t lfo1_internal_depth;
        switch (pRegion->LFO1Controller) {
            case ::gig::lfo1_ctrl_internal:
                lfo1_internal_depth  = pRegion->LFO1InternalDepth;
                pLFO1->ExtController = 0; // no external controller
                bLFO1Enabled         = (lfo1_internal_depth > 0);
                break;
            case ::gig::lfo1_ctrl_modwheel:
                lfo1_internal_depth  = 0;
                pLFO1->ExtController = 1; // MIDI controller 1
                bLFO1Enabled         = (pRegion->LFO1ControlDepth > 0);
                break;
            case ::gig::lfo1_ctrl_breath:
                lfo1_internal_depth  = 0;
                pLFO1->ExtController = 2; // MIDI controller 2
                bLFO1Enabled         = (pRegion->LFO1ControlDepth > 0);
                break;
            case ::gig::lfo1_ctrl_internal_modwheel:
                lfo1_internal_depth  = pRegion->LFO1InternalDepth;
                pLFO1->ExtController = 1; // MIDI controller 1
                bLFO1Enabled         = (lfo1_internal_depth > 0 || pRegion->LFO1ControlDepth > 0);
                break;
            case ::gig::lfo1_ctrl_internal_breath:
                lfo1_internal_depth  = pRegion->LFO1InternalDepth;
                pLFO1->ExtController = 2; // MIDI controller 2
                bLFO1Enabled         = (lfo1_internal_depth > 0 || pRegion->LFO1ControlDepth > 0);
                break;
            default:
                lfo1_internal_depth  = 0;
                pLFO1->ExtController = 0; // no external controller
                bLFO1Enabled         = false;
        }
        if (bLFO1Enabled) {
            pLFO1->trigger(fromGigLfoWave(pRegion->LFO1WaveForm),
                           pRegion->LFO1Frequency,
                           pRegion->LFO1Phase,
                           LFO::start_level_mid, // see https://sourceforge.net/p/linuxsampler/mailman/linuxsampler-devel/thread/2189307.cNP0Xbctxq%40silver/#msg36774029
                           lfo1_internal_depth,
                           pRegion->LFO1ControlDepth,
                           pRegion->LFO1FlipPhase,
                           pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
            pLFO1->updateByMIDICtrlValue(pLFO1->ExtController ? GetGigEngineChannel()->ControllerTable[pLFO1->ExtController] : 0);
            pLFO1->setScriptDepthFactor(
                pNote->Override.AmpLFODepth.Value,
                pNote->Override.AmpLFODepth.Final
            );
            if (pNote->Override.AmpLFOFreq.isFinal())
                pLFO1->setScriptFrequencyFinal(
                    pNote->Override.AmpLFOFreq.Value, pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE
                );
            else
                pLFO1->setScriptFrequencyFactor(
                    pNote->Override.AmpLFOFreq.Value, pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE
                );
        }
    }

    void Voice::InitLFO2() {
        uint16_t lfo2_internal_depth;
        switch (pRegion->LFO2Controller) {
            case ::gig::lfo2_ctrl_internal:
                lfo2_internal_depth  = pRegion->LFO2InternalDepth;
                pLFO2->ExtController = 0; // no external controller
                bLFO2Enabled         = (lfo2_internal_depth > 0);
                break;
            case ::gig::lfo2_ctrl_modwheel:
                lfo2_internal_depth  = 0;
                pLFO2->ExtController = 1; // MIDI controller 1
                bLFO2Enabled         = (pRegion->LFO2ControlDepth > 0);
                break;
            case ::gig::lfo2_ctrl_foot:
                lfo2_internal_depth  = 0;
                pLFO2->ExtController = 4; // MIDI controller 4
                bLFO2Enabled         = (pRegion->LFO2ControlDepth > 0);
                break;
            case ::gig::lfo2_ctrl_internal_modwheel:
                lfo2_internal_depth  = pRegion->LFO2InternalDepth;
                pLFO2->ExtController = 1; // MIDI controller 1
                bLFO2Enabled         = (lfo2_internal_depth > 0 || pRegion->LFO2ControlDepth > 0);
                break;
            case ::gig::lfo2_ctrl_internal_foot:
                lfo2_internal_depth  = pRegion->LFO2InternalDepth;
                pLFO2->ExtController = 4; // MIDI controller 4
                bLFO2Enabled         = (lfo2_internal_depth > 0 || pRegion->LFO2ControlDepth > 0);
                break;
            default:
                lfo2_internal_depth  = 0;
                pLFO2->ExtController = 0; // no external controller
                bLFO2Enabled         = false;
        }
        if (bLFO2Enabled) {
            pLFO2->trigger(fromGigLfoWave(pRegion->LFO2WaveForm),
                           pRegion->LFO2Frequency,
                           pRegion->LFO2Phase,
                           LFO::start_level_mid, // see https://sourceforge.net/p/linuxsampler/mailman/linuxsampler-devel/thread/2189307.cNP0Xbctxq%40silver/#msg36774029
                           lfo2_internal_depth,
                           pRegion->LFO2ControlDepth,
                           pRegion->LFO2FlipPhase,
                           pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
            pLFO2->updateByMIDICtrlValue(pLFO2->ExtController ? GetGigEngineChannel()->ControllerTable[pLFO2->ExtController] : 0);
            pLFO2->setScriptDepthFactor(
                pNote->Override.CutoffLFODepth.Value,
                pNote->Override.CutoffLFODepth.Final
            );
            if (pNote->Override.CutoffLFOFreq.isFinal())
                pLFO2->setScriptFrequencyFinal(pNote->Override.CutoffLFOFreq.Value, pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
            else
                pLFO2->setScriptFrequencyFactor(pNote->Override.CutoffLFOFreq.Value, pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
        }
    }

    void Voice::InitLFO3() {
        uint16_t lfo3_internal_depth;
        switch (pRegion->LFO3Controller) {
            case ::gig::lfo3_ctrl_internal:
                lfo3_internal_depth  = pRegion->LFO3InternalDepth;
                pLFO3->ExtController = 0; // no external controller
                bLFO3Enabled         = (lfo3_internal_depth > 0);
                break;
            case ::gig::lfo3_ctrl_modwheel:
                lfo3_internal_depth  = 0;
                pLFO3->ExtController = 1; // MIDI controller 1
                bLFO3Enabled         = (pRegion->LFO3ControlDepth > 0);
                break;
            case ::gig::lfo3_ctrl_aftertouch:
                lfo3_internal_depth  = 0;
                pLFO3->ExtController = CTRL_TABLE_IDX_AFTERTOUCH;
                bLFO3Enabled         = true;
                break;
            case ::gig::lfo3_ctrl_internal_modwheel:
                lfo3_internal_depth  = pRegion->LFO3InternalDepth;
                pLFO3->ExtController = 1; // MIDI controller 1
                bLFO3Enabled         = (lfo3_internal_depth > 0 || pRegion->LFO3ControlDepth > 0);
                break;
            case ::gig::lfo3_ctrl_internal_aftertouch:
                lfo3_internal_depth  = pRegion->LFO3InternalDepth;
                pLFO3->ExtController = CTRL_TABLE_IDX_AFTERTOUCH;
                bLFO3Enabled         = (lfo3_internal_depth > 0 || pRegion->LFO3ControlDepth > 0);
                break;
            default:
                lfo3_internal_depth  = 0;
                pLFO3->ExtController = 0; // no external controller
                bLFO3Enabled         = false;
        }
        if (bLFO3Enabled) {
            pLFO3->trigger(fromGigLfoWave(pRegion->LFO3WaveForm),
                           pRegion->LFO3Frequency,
                           pRegion->LFO3Phase,
                           LFO::start_level_max, // see https://sourceforge.net/p/linuxsampler/mailman/linuxsampler-devel/thread/2189307.cNP0Xbctxq%40silver/#msg36774029
                           lfo3_internal_depth,
                           pRegion->LFO3ControlDepth,
                           pRegion->LFO3FlipPhase,
                           pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
            pLFO3->updateByMIDICtrlValue(pLFO3->ExtController ? GetGigEngineChannel()->ControllerTable[pLFO3->ExtController] : 0);
            pLFO3->setScriptDepthFactor(
                pNote->Override.PitchLFODepth.Value,
                pNote->Override.PitchLFODepth.Final
            );
            if (pNote->Override.PitchLFOFreq.isFinal())
                pLFO3->setScriptFrequencyFinal(pNote->Override.PitchLFOFreq.Value, pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
            else
                pLFO3->setScriptFrequencyFactor(pNote->Override.PitchLFOFreq.Value, pEngine->SampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
        }
    }

    float Voice::CalculateCutoffBase(uint8_t MIDIKeyVelocity) {
        float cutoff = pRegion->GetVelocityCutoff(MIDIKeyVelocity);
        if (pRegion->VCFKeyboardTracking) {
            cutoff *= RTMath::CentsToFreqRatioUnlimited((MIDIKey() - pRegion->VCFKeyboardTrackingBreakpoint) * 100);
        }
        return cutoff;
    }

    // This is just called when the voice is triggered. On any subsequent cutoff
    // controller changes ProcessCutoffEvent() is called instead.
    float Voice::CalculateFinalCutoff(float cutoffBase) {
        // if the selected filter type is an official GigaStudio filter type
        // then we preserve the original (no matter how odd) historical GSt
        // behaviour identically; for our own filter types though we deviate to
        // more meaningful behaviours where appropriate
        const bool isGStFilter = isGStFilterType(pRegion->VCFType);

        // get current cutoff CC or velocity value (always 0..127)
        float cvalue;
        if (VCFCutoffCtrl.controller) {
            cvalue = GetGigEngineChannel()->ControllerTable[VCFCutoffCtrl.controller];
            if (pRegion->VCFCutoffControllerInvert) cvalue = 127 - cvalue;
            if (isGStFilter) {
                // VCFVelocityScale in this case means "minimum cutoff" for GSt
                if (cvalue < MinCutoff()) cvalue = MinCutoff();
            } else {
                // for our own filter types we interpret "minimum cutoff"
                // differently: GSt handles this as a simple hard limit with the
                // consequence that a certain range of the controller is simply
                // dead; so for our filter types we rather remap that to
                // restrain within the min_cutoff..127 range as well, but
                // effectively spanned over the entire controller range (0..127)
                // to avoid such a "dead" lower controller zone
                cvalue = MinCutoff() + (cvalue / 127.f) * float(127 - MinCutoff());
            }
        } else {
            // in case of velocity, VCFVelocityScale parameter is already
            // handled on libgig side (so by calling
            // pRegion->GetVelocityCutoff(velo) in CalculateCutoffBase() above)
            cvalue = pRegion->VCFCutoff;
        }

        float fco = cutoffBase * cvalue;
        if (fco > 127.0f) fco = 127.0f;

        // the filter implementations of the original GSt filter types take an
        // abstract cutoff parameter range of 0..127, ...
        if (isGStFilter)
            return fco;

        // ... whereas our own filter types take a cutoff parameter in Hz, so
        // remap here 0 .. 127 [lin] -> 21 Hz .. 18 kHz [x^4] (center @2.2 kHz)
        fco = (fco + 29.f) / (127.f + 29.f);
        fco = fco * fco * fco * fco * 18000.f;
        if (fco > 0.49f * pEngine->SampleRate)
            fco = 0.49f * pEngine->SampleRate;
        return fco;
    }

    uint8_t Voice::GetVCFCutoffCtrl() {
        uint8_t ctrl;
        switch (pRegion->VCFCutoffController) {
            case ::gig::vcf_cutoff_ctrl_modwheel:
                ctrl = 1;
                break;
            case ::gig::vcf_cutoff_ctrl_effect1:
                ctrl = 12;
                break;
            case ::gig::vcf_cutoff_ctrl_effect2:
                ctrl = 13;
                break;
            case ::gig::vcf_cutoff_ctrl_breath:
                ctrl = 2;
                break;
            case ::gig::vcf_cutoff_ctrl_foot:
                ctrl = 4;
                break;
            case ::gig::vcf_cutoff_ctrl_sustainpedal:
                ctrl = 64;
                break;
            case ::gig::vcf_cutoff_ctrl_softpedal:
                ctrl = 67;
                break;
            case ::gig::vcf_cutoff_ctrl_genpurpose7:
                ctrl = 82;
                break;
            case ::gig::vcf_cutoff_ctrl_genpurpose8:
                ctrl = 83;
                break;
            case ::gig::vcf_cutoff_ctrl_aftertouch:
                ctrl = CTRL_TABLE_IDX_AFTERTOUCH;
                break;
            case ::gig::vcf_cutoff_ctrl_none:
            default:
                ctrl = 0;
                break;
        }

        return ctrl;
    }

    uint8_t Voice::GetVCFResonanceCtrl() {
        uint8_t ctrl;
        switch (pRegion->VCFResonanceController) {
            case ::gig::vcf_res_ctrl_genpurpose3:
                ctrl = 18;
                break;
            case ::gig::vcf_res_ctrl_genpurpose4:
                ctrl = 19;
                break;
            case ::gig::vcf_res_ctrl_genpurpose5:
                ctrl = 80;
                break;
            case ::gig::vcf_res_ctrl_genpurpose6:
                ctrl = 81;
                break;
            case ::gig::vcf_res_ctrl_none:
            default:
                ctrl = 0;
        }

        return ctrl;
    }

    void Voice::TriggerEG1(const EGInfo& egInfo, double velrelease, double velocityAttenuation, uint sampleRate, uint8_t velocity) {
        EG1.setStateOptions(
            pRegion->EG1Options.AttackCancel,
            pRegion->EG1Options.AttackHoldCancel,
            pRegion->EG1Options.Decay1Cancel,
            pRegion->EG1Options.Decay2Cancel,
            pRegion->EG1Options.ReleaseCancel
        );
        EG1.trigger(pRegion->EG1PreAttack,
                    (pNote && pNote->Override.Attack.isFinal()) ?
                        pNote->Override.Attack.Value :
                        RTMath::Max(pRegion->EG1Attack, 0.0316) * egInfo.Attack,
                    pRegion->EG1Hold,
                    (pNote && pNote->Override.Decay.isFinal()) ?
                        pNote->Override.Decay.Value :
                        pRegion->EG1Decay1 * egInfo.Decay * velrelease,
                    (pNote && pNote->Override.Decay.isFinal()) ?
                        pNote->Override.Decay.Value :
                        pRegion->EG1Decay2 * egInfo.Decay * velrelease,
                    pRegion->EG1InfiniteSustain,
                    (pNote && pNote->Override.Sustain.Final) ?
                        uint(pNote->Override.Sustain.Value * 1000.f) :
                        pRegion->EG1Sustain * (pNote ? pNote->Override.Sustain.Value : 1.f),
                    (pNote && pNote->Override.Release.isFinal()) ?
                        pNote->Override.Release.Value :
                        RTMath::Max(pRegion->EG1Release * velrelease, 0.014) * egInfo.Release,
                    velocityAttenuation,
                    sampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
    }

    void Voice::TriggerEG2(const EGInfo& egInfo, double velrelease, double velocityAttenuation, uint sampleRate, uint8_t velocity) {
        EG2.setStateOptions(
            pRegion->EG2Options.AttackCancel,
            pRegion->EG2Options.AttackHoldCancel,
            pRegion->EG2Options.Decay1Cancel,
            pRegion->EG2Options.Decay2Cancel,
            pRegion->EG2Options.ReleaseCancel
        );
        EG2.trigger(uint(RgnInfo.EG2PreAttack),
                    (pNote && pNote->Override.CutoffAttack.isFinal()) ?
                        pNote->Override.CutoffAttack.Value :
                        RgnInfo.EG2Attack * egInfo.Attack,
                    false,
                    (pNote && pNote->Override.CutoffDecay.isFinal()) ?
                        pNote->Override.CutoffDecay.Value :
                        RgnInfo.EG2Decay1 * egInfo.Decay * velrelease,
                    (pNote && pNote->Override.CutoffDecay.isFinal()) ?
                        pNote->Override.CutoffDecay.Value :
                        RgnInfo.EG2Decay2 * egInfo.Decay * velrelease,
                    RgnInfo.EG2InfiniteSustain,
                    (pNote && pNote->Override.CutoffSustain.Final) ?
                        uint(pNote->Override.CutoffSustain.Value * 1000.f) :
                        uint(RgnInfo.EG2Sustain),
                    (pNote && pNote->Override.CutoffRelease.isFinal()) ?
                        pNote->Override.CutoffRelease.Value :
                        RgnInfo.EG2Release * egInfo.Release * velrelease,
                    velocityAttenuation,
                    sampleRate / CONFIG_DEFAULT_SUBFRAGMENT_SIZE);
    }

    void Voice::ProcessGroupEvent(RTList<Event>::Iterator& itEvent) {
        dmsg(4,("Voice %p processGroupEvents event type=%d", (void*)this, itEvent->Type));

        // TODO: The SustainPedal condition could be wrong, maybe the
        // check should be if this Voice is in release stage or is a
        // release sample instead. Need to test this in GSt.
        // -- Andreas
        //
        // Commented sustain pedal check out. I don't think voices of the same
        // note should be stopped at all, because it doesn't sound naturally
        // with a drumkit.
        // -- Christian, 2013-01-08
        if (itEvent->Param.Note.Key != HostKey() /*||
            !GetGigEngineChannel()->SustainPedal*/) {
            dmsg(4,("Voice %p - kill", (void*)this));

            // kill the voice fast
            pEG1->enterFadeOutStage();
        }
    }

    void Voice::CalculateFadeOutCoeff(float FadeOutTime, float SampleRate) {
        EG1.CalculateFadeOutCoeff(FadeOutTime, SampleRate);
    }

    int Voice::CalculatePan(uint8_t pan) {
        int p;
        // Gst behaviour: -64 and 63 are special cases
        if (RgnInfo.Pan == -64)     p = pan * 2 - 127;
        else if (RgnInfo.Pan == 63) p = pan * 2;
        else                        p = pan + RgnInfo.Pan;

        if (p < 0) return 0;
        if (p > 127) return 127;
        return p;
    }

    release_trigger_t Voice::GetReleaseTriggerFlags() {
        release_trigger_t flags =
            (pRegion->NoNoteOffReleaseTrigger) ?
                release_trigger_none : release_trigger_noteoff; //HACK: currently this method is actually only called by EngineBase if it already knows that this voice requires release trigger, so I took the short way instead of checking (again) the existence of a ::gig::dimension_releasetrigger
        switch (pRegion->SustainReleaseTrigger) {
            case ::gig::sust_rel_trg_none:
                break;
            case ::gig::sust_rel_trg_maxvelocity:
                flags |= release_trigger_sustain_maxvelocity;
                break;
            case ::gig::sust_rel_trg_keyvelocity:
                flags |= release_trigger_sustain_keyvelocity;
                break;
        }
        return flags;
    }

}} // namespace LinuxSampler::gig