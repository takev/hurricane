
AC_PREREQ(2.61)
AC_INIT(hurricane, 1.0.0, info@takevos.nl)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([buildutils])
AM_INIT_AUTOMAKE()

DX_INIT_DOXYGEN(hurricane, doxygen.cfg)
DX_DOXYGEN_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_DOT_FEATURE(OFF)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

AC_PROG_CC([gcc clang cc])
AC_PROG_CXX([g++ clang++ c++])
AC_PROG_INSTALL
AC_PROG_LIBTOOL

BOOST_REQUIRE
BOOST_TEST
BOOST_FILESYSTEM

AC_CONFIG_HEADERS([hurricane/config.h])
AC_CONFIG_FILES([Makefile hurricane/Makefile hurricane/hurricane.pc])
AC_OUTPUT

