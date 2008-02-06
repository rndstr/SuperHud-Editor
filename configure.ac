dnl rerun autoreconf -fvi -Wall after modifying this.

dnl AC_PREREQ(2.60)
AC_INIT([superhudeditor], [0.4.0], [rolansch@ethz.ch])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/superhudeditor.cpp])
AM_CONFIG_HEADER([config.h])

dnl checks for programs.
AC_PROG_CXX
AC_PROG_CC


dnl more features
AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug],
                              [enable support for debugging (default disabled)])],
              [enable_debug=$enableval],
              [enable_debug=no])
AC_MSG_CHECKING([whether debugging enabled])
AC_MSG_RESULT($enable_debug)
AM_CONDITIONAL([ENABLE_DEBUG], test x$enable_debug = xyes)

AC_ARG_ENABLE([cpma],
              [AS_HELP_STRING([--disable-cpma],
                              [disable support for CPMA SuperHud (default enabled)])],
              [disable_cpma=$enableval],
              [disable_cpma=no])
AC_MSG_CHECKING([whether CPMA SuperHud support is disabled])
AC_MSG_RESULT($disable_cpma)
AM_CONDITIONAL([DISABLE_CPMA], test x$disable_cpma = xyes)

AC_ARG_ENABLE([q4max],
              [AS_HELP_STRING([--disable-q4max],
                              [disable support for Q4MAX SuperHud (default enabled)])],
              [disable_q4max=$enableval],
              [disable_q4max=no])
AC_MSG_CHECKING([whether Q4MAX SuperHud support is disabled])
AC_MSG_RESULT($disable_q4max)
AM_CONDITIONAL([DISABLE_Q4MAX], test x$disable_q4max = xyes)


dnl checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

AM_OPTIONS_WXCONFIG
reqwx=2.8.7
dnl check if WX was built with opengl support
dnl but this doesn't give a warning if wxGTK wasn't built with opengl
if test x$enable_debug = xyes; then
  AM_PATH_WXCONFIG($reqwx, wxWin=1, , [base,core,gl,net,aui], [--debug])
else
  AM_PATH_WXCONFIG($reqwx, wxWin=1, , [base,core,gl,net,aui], [])
fi
AC_MSG_NOTICE([
-------------------------------------------
Make sure that wxWidgets (wxGTK) was 
built with opengl support or else this
won't compile.
-------------------------------------------
])

dnl check for opengl headers
AC_CHECK_HEADER([GL/gl.h], , [AC_MSG_ERROR([Sorry, can't do anything for you])])
AC_CHECK_HEADER([GL/glu.h], , [AC_MSG_ERROR([Sorry, can't do anything for you])])


dnl Checks for library functions.
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.

		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"


AC_CONFIG_FILES([
	Makefile
	src/Makefile
	])

AC_OUTPUT
