# let GCC auto detect whether it (sh/c)ould output its messages in color
export GCC_COLORS=auto

# require automake >= 1.5 (mandatory for option dist-bzip2)
AUTOMAKE_OPTIONS = 1.5 dist-bzip2

SUBDIRS = src gfx po debian doc win32

EXTRA_DIST = \
	autogen.sh \
	clean.sh \
	Makefile.svn

DISTCLEANFILES =		\
	intltool-extract	\
	intltool-merge		\
	intltool-update
