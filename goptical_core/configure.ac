# Copyright (C) 2010-2011 Free Software Foundation, Inc
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT(goptical_core, 1.0)
PACKAGE_VERSION=1.0
AC_SUBST(PACKAGE_VERSION)

AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_MACRO_DIR(build-aux)

AM_INIT_AUTOMAKE(goptical_core, 1.0)
AC_CONFIG_HEADERS(config.hh)

AC_PROG_CXX
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_LANG(C++)

CXXFLAGS="$CXXFLAGS -Wall "

AC_ARG_ENABLE(tests, AC_HELP_STRING(--disable-tests, [Disable compilation of tests]), disable_tests=yes)
AM_CONDITIONAL(TESTS, test x$disable_tests != xyes)

AC_ARG_ENABLE(debug, AC_HELP_STRING(--enable-debug, [Enable debug and asserts]), enable_debug=yes)
if test "x$enable_debug" = "xyes" ; then
   CXXFLAGS="$CXXFLAGS -g "
else
   CPPFLAGS="$CPPFLAGS -DNDEBUG "
fi

# c++ library checking macro
AC_DEFUN([AT_CHECK_CPPLIB], [
  SAVED_LDFLAGS=$LDFLAGS
  LDFLAGS="$LDFLAGS $1"
  AC_MSG_CHECKING([$1 C++ library availability])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([#include <$2>], [$3])],
    [
      AC_MSG_RESULT([yes])
      $4
    ], [
      AC_MSG_RESULT([no])
    ]
)
  LDFLAGS=$SAVED_LDFLAGS
])

AC_CHECK_LIB(m, cosf, , AC_MSG_ERROR([can not find m library]))

#
# gsl library check
#

LIBS="$LIBS -lgsl -lgslcblas "
AC_CHECK_HEADER(gsl/gsl_math.h, [ AC_CHECK_LIB(gslcblas, cblas_srot, [ AC_CHECK_LIB(gsl, gsl_error, [ 
  GOPTICAL_HAVE_GSL=true
])])])
if test x$GOPTICAL_HAVE_GSL != xtrue ; then
  AC_MSG_ERROR([GNU scientific library (libgsl) is missing])
fi

#
# dime library check
#

AC_ARG_ENABLE(dime, AC_HELP_STRING(--disable-dime, [Disable use of Dime DXF renderer]), GOPTICAL_HAVE_DIME=false)

if test x$GOPTICAL_HAVE_DIME != xfalse ; then
AT_CHECK_CPPLIB(-ldime, dime/Output.h, new dimeOutput();, [
  GOPTICAL_HAVE_DIME=true
  AC_DEFINE(GOPTICAL_HAVE_DIME, 1, [dime DXF rendering library enabled])
  LIBS="$LIBS -ldime "
])
fi
AM_CONDITIONAL(GOPTICAL_HAVE_DIME, test x$GOPTICAL_HAVE_DIME = xtrue)

#
# gd library check
#

AC_ARG_ENABLE(gd, AC_HELP_STRING(--disable-gd, [Disable use of libGd renderer]), GOPTICAL_HAVE_GD=false)

if test x$GOPTICAL_HAVE_GD != xfalse ; then
AC_CHECK_HEADER(gd.h, [ AC_CHECK_LIB(gd, gdImageCreateTrueColor, [
  GOPTICAL_HAVE_GD=true
  AC_DEFINE(GOPTICAL_HAVE_GD, 1, [gd rendering library enabled])
  LIBS="$LIBS -lgd "
])])
fi
AM_CONDITIONAL(GOPTICAL_HAVE_GD, test x$GOPTICAL_HAVE_GD = xtrue)

#
# opengl library check
#

AC_ARG_ENABLE(gl, AC_HELP_STRING(--disable-gl, [Disable use of opengl]), GOPTICAL_HAVE_GL=false)

if test x$GOPTICAL_HAVE_GL != xfalse ; then
AC_CHECK_HEADER(GL/gl.h, [ AC_CHECK_LIB(GL, glLoadIdentity, [
  AC_CHECK_HEADER(GL/glu.h, [ AC_CHECK_LIB(GLU, gluPerspective, [
    AC_CHECK_HEADER(GL/glut.h, [ AC_CHECK_LIB(glut, glutInit, [
      GOPTICAL_HAVE_GL=true
      AC_DEFINE(GOPTICAL_HAVE_GL, 1, [gl rendering library enabled])
      LIBS="$LIBS -lglut -lGL -lGLU "
    ])])
  ])])
])])
fi
AM_CONDITIONAL(GOPTICAL_HAVE_GL, test x$GOPTICAL_HAVE_GL = xtrue)

#
# X11 library check
#

AC_ARG_ENABLE(x11, AC_HELP_STRING(--disable-x11, [Disable use of x11]), GOPTICAL_HAVE_X11=false)

if test x$GOPTICAL_HAVE_X11 != xfalse ; then
AC_CHECK_HEADER(X11/Xlib.h, [ AC_CHECK_LIB(X11, XOpenDisplay, [
  GOPTICAL_HAVE_X11=true
  AC_DEFINE(GOPTICAL_HAVE_X11, 1, [x11 rendering library enabled])
  LIBS="$LIBS -lX11 "
])])
fi
AM_CONDITIONAL(GOPTICAL_HAVE_X11, test x$GOPTICAL_HAVE_X11 = xtrue)

#
# plplot library check
#

AC_ARG_ENABLE(plplot, AC_HELP_STRING(--disable-plplot, [Disable use of PlPlot renderer]), GOPTICAL_HAVE_PLPLOT=false)

if test x$GOPTICAL_HAVE_PLPLOT != xfalse ; then
AT_CHECK_CPPLIB(-lplplotd -lplplotcxxd, plplot/plstream.h, new plstream();, [
  GOPTICAL_HAVE_PLPLOT=true
  AC_DEFINE(GOPTICAL_HAVE_PLPLOT, 1, [plplot rendering library enabled])
  LIBS="$LIBS -lplplotcxxd -lplplotd "
])
fi
AM_CONDITIONAL(GOPTICAL_HAVE_PLPLOT, test x$GOPTICAL_HAVE_PLPLOT = xtrue)



AC_CONFIG_FILES([ Makefile src/Makefile src/Goptical/Makefile
  src/Goptical/Analysis/Makefile src/Goptical/Curve/Makefile
  src/Goptical/Data/Makefile src/Goptical/Io/Makefile
  src/Goptical/Light/Makefile src/Goptical/Material/Makefile
  src/Goptical/Math/Makefile src/Goptical/Shape/Makefile
  src/Goptical/Sys/Makefile src/Goptical/Trace/Makefile tests/Makefile
  ])

AC_OUTPUT


