AM_CPPFLAGS = $(all_includes) $(SNDFILE_CFLAGS) $(GIG_CFLAGS)
METASOURCES = AUTO

AM_CXXFLAGS = -Wreturn-type -ffast-math $(CXX_CPU_SWITCH)

noinst_LTLIBRARIES = liblinuxsamplercommonengine.la
liblinuxsamplercommonengine_la_SOURCES = \
	BiquadFilter.h \
	EaseInEaseOut.h \
	Event.cpp Event.h \
	Fade.h \
	Sample.h SampleManager.h SampleFile.cpp SampleFile.h \
	Stream.h StreamBase.cpp StreamBase.h \
	DiskThreadBase.cpp DiskThreadBase.h \
	Voice.h AbstractVoice.cpp AbstractVoice.h VoiceBase.h \
	SignalUnit.h SignalUnit.cpp SignalUnitRack.h ModulatorGraph.cpp \
	MidiKeyboardManager.h \
	LFOAll.h \
	LFOBase.h \
	LFOCluster.h \
	LFOPulse.h \
	LFOSawIntMathNew.h \
	LFOSawIntMathOld.h \
	LFOSineBuiltinFn.h \
	LFOSineNumericComplexNr.h \
	LFOSquareIntMath.h \
	LFOSquarePulse.h \
	LFOTriangleDiHarmonic.h \
	LFOTriangleIntAbsMath.h \
	LFOTriangleIntMath.h \
	Note.h \
	Resampler.h \
	AbstractInstrumentManager.h AbstractInstrumentManager.cpp \
	InstrumentScriptVM.h InstrumentScriptVM.cpp \
	InstrumentScriptVMFunctions.h InstrumentScriptVMFunctions.cpp \
	InstrumentScriptVMDynVars.h InstrumentScriptVMDynVars.cpp \
	EG.h EG.cpp
liblinuxsamplercommonengine_la_LIBADD = $(SNDFILE_LIBS)
liblinuxsamplercommonengine_la_LDFLAGS = $(SNDFILE_CFLAGS)
