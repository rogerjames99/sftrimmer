# set the include path found by configure
AM_CPPFLAGS= $(all_includes)

AM_CXXFLAGS = -Wreturn-type $(CXX_CPU_SWITCH)

bin_PROGRAMS = lscp

lscp_SOURCES = \
	CCursor.h \
	CFmt.h \
	KeyboardReader.cpp KeyboardReader.h \
	lscp.cpp lscp.h \
	LSCPClient.cpp LSCPClient.h \
	TerminalCtrl.cpp TerminalCtrl.h \
	TerminalPrinter.cpp TerminalPrinter.h

lscp_LDADD = $(top_builddir)/src/liblinuxsampler.la
