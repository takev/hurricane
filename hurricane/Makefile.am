
DEFAULT_INCLUDES=-I$(top_srcdir)

BOOST_CPPFLAGS_ALL = -DBOOST_ALL_DYN_LINK
BOOST_CPPFLAGS_ALL+= $(BOOST_CPPFLAGS)
BOOST_CPPFLAGS_ALL+= $(BOOST_TEST_CPPFLAGS)
BOOST_CPPFLAGS_ALL+= $(BOOST_FILESYSTEM_CPPFLAGS)
BOOST_CPPFLAGS_ALL+= $(BOOST_SYSTEM_CPPFLAGS)

BOOST_LDFLAGS_ALL  = $(BOOST_LDFLAGS)
BOOST_LDFLAGS_ALL += $(BOOST_TEST_LDFLAGS)
BOOST_LDFLAGS_ALL += $(BOOST_FILESYSTEM_LDFLAGS)
BOOST_LDFLAGS_ALL += $(BOOST_SYSTEM_LDFLAGS)

BOOST_LIBS_ALL     = $(BOOST_LIBS)
BOOST_LIBS_ALL    += $(BOOST_TEST_LIBS)
BOOST_LIBS_ALL    += $(BOOST_FILESYSTEM_LIBS)
BOOST_LIBS_ALL    += $(BOOST_SYSTEM_LIBS)

LIBS 		= $(BOOST_LIBS_ALL)
AM_LDFLAGS 	= $(BOOST_LDFLAGS_ALL)
AM_CPPFLAGS 	= -g -Wall -W -pedantic -std=c++1y $(DEFAULT_INCLUDES) $(BOOST_CPPFLAGS_ALL)
AM_CFLAGS 	= -g -Wall -W -pedantic -std=c99   $(DEFAULT_INCLUDES) $(BOOST_CPPFLAGS_ALL)

bin_PROGRAMS = hurricane utils_tests Tokenizer_tests VHDLSourceFile_tests
TESTS = utils_tests Tokenizer_tests VHDLSourceFile_tests

hurricane_SOURCES = hurricane.cc
hurricane_SOURCES+= Options.cc
hurricane_SOURCES+= utils.cc
hurricane_SOURCES+= Library.cc
hurricane_SOURCES+= Project.cc
hurricane_SOURCES+= Tokenizer.cc
hurricane_SOURCES+= SourceFile.cc
hurricane_SOURCES+= VHDLSourceFile.cc

utils_tests_LDFLAGS	= $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
utils_tests_LDADD	= $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)
utils_tests_SOURCES 	= utils_tests.cc utils.cc

Tokenizer_tests_LDFLAGS	= $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
Tokenizer_tests_LDADD	= $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)
Tokenizer_tests_SOURCES	= Tokenizer_tests.cc Tokenizer.cc

VHDLSourceFile_tests_LDFLAGS	= $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
VHDLSourceFile_tests_LDADD	= $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)
VHDLSourceFile_tests_SOURCES	= VHDLSourceFile_tests.cc VHDLSourceFile.cc SourceFile.cc Options.cc utils.cc Tokenizer.cc

