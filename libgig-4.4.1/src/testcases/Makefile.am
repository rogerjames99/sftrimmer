AM_CPPFLAGS = $(all_includes)

EXTRA_PROGRAMS = libgigtests
libgigtests_SOURCES = \
	main.cpp \
	GigWriteTest.cpp GigWriteTest.h
libgigtests_LDADD = $(top_builddir)/src/libgig.la -lcppunit

EXTRA_DIST = CMakeLists.txt HelperTest.cpp
