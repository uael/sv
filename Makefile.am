## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL.autotools configure.sh
dist_doc_DATA		= README.md README.autotools LICENSE
AM_CFLAGS		= -I$(srcdir)/src -I$(srcdir)/include -Wall $(AX_CFLAGS)

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/libsv.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/libsv.pc

## --------------------------------------------------------------------

# Commented out because  at present there is  no Texinfo documentation
# included.  It will come later.  (Marco Maggi; May 17, 2017)
#
# AM_MAKEINFOFLAGS	= --no-split
# info_TEXINFOS		= doc/semver.texi
# doc_semver_TEXINFOS	= doc/macros.texi

#page
#### libraries

libsv_CURRENT		= @libsv_VERSION_INTERFACE_CURRENT@
libsv_REVISION		= @libsv_VERSION_INTERFACE_REVISION@
libsv_AGE		= @libsv_VERSION_INTERFACE_AGE@

include_HEADERS		= include/semver.h

lib_LTLIBRARIES		= libsv.la
libsv_la_LDFLAGS	= -version-info $(libsv_CURRENT):$(libsv_REVISION):$(libsv_AGE)
libsv_la_SOURCES	= \
	src/comp.c		\
	src/id.c		\
	src/num.c		\
	src/range.c		\
	src/semver.c

#page
#### test

check_PROGRAMS	= \
	test/semver		\
	test/comp		\
	test/match		\
	test/range

TESTS				= $(check_PROGRAMS)

libsv_test_cppflags		= -I$(srcdir)/src -I$(srcdir)/include
libsv_test_ldadd		= libsv.la

test_semver_CPPFLAGS		= $(libsv_test_cppflags)
test_semver_LDADD		= $(libsv_test_ldadd)

test_comp_CPPFLAGS		= $(libsv_test_cppflags)
test_comp_LDADD			= $(libsv_test_ldadd)

test_match_CPPFLAGS		= $(libsv_test_cppflags)
test_match_LDADD		= $(libsv_test_ldadd)

test_range_CPPFLAGS		= $(libsv_test_cppflags)
test_range_LDADD		= $(libsv_test_ldadd)

installcheck-local: $(TEST)
	@for f in $(TEST); do $(builddir)/$$f; done

#page
#### Static analysis with Clang's Static Analyzer
#
# See the documentation for the command line tool at:
#
#    <http://clang-analyzer.llvm.org/scan-build.html>
#
# To run the tool we must do:
#
#    $ make clean
#    $ make clang-static-analysis
#
# The  program  "scan-build"  works  by  overriding  the  CC  and  CXX
# environment variables.
#

.PHONY: clang-static-analysis

clang-static-analysis:
	scan-build make

### end of file
