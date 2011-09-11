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

CXXFLAGS+=" -Wall "

AC_ARG_ENABLE(tests, AC_HELP_STRING(--disable-tests, [Disable compilation of tests]), disable_tests=yes)
AM_CONDITIONAL(TESTS, test x$disable_tests != xyes)

AC_ARG_ENABLE(debug, AC_HELP_STRING(--enable-debug, [Enable debug and asserts]), enable_tests=yes)
if test "x$enable_tests" = "xyes" ; then
   CXXFLAGS+=" -g "
else
   CPPFLAGS+=" -DNDEBUG "
fi

AC_CHECK_HEADER(gsl/gsl_math.h, , AC_MSG_ERROR([GNU scientific library (libgsl) is missing]))
LDFLAGS+=" -lgsl -lgslcblas "

AC_CHECK_HEADER(dime/Base.h, [
  GOPTICAL_HAVE_DIME=true
  AC_DEFINE(GOPTICAL_HAVE_DIME, 1, [dime DXF rendering library enabled])
  LDFLAGS+=" -ldime "
])
AM_CONDITIONAL(GOPTICAL_HAVE_DIME, test x$GOPTICAL_HAVE_DIME = xtrue)

AC_CHECK_HEADER(gd.h, [
  GOPTICAL_HAVE_GD=true
  AC_DEFINE(GOPTICAL_HAVE_GD, 1, [gd rendering library enabled])
  LDFLAGS+=" -lgd "
])
AM_CONDITIONAL(GOPTICAL_HAVE_GD, test x$GOPTICAL_HAVE_GD = xtrue)

AC_CHECK_HEADER(GL/gl.h, [
  AC_CHECK_HEADER(GL/glu.h, [
    AC_CHECK_HEADER(GL/glut.h, [
      GOPTICAL_HAVE_GL=true
      AC_DEFINE(GOPTICAL_HAVE_GL, 1, [gl rendering library enabled])
      LDFLAGS+=" -lglut -lGL -lGLU "
    ])
  ])
])
AM_CONDITIONAL(GOPTICAL_HAVE_GL, test x$GOPTICAL_HAVE_GL = xtrue)

AC_CHECK_HEADER(X11/Xlib.h, [
  GOPTICAL_HAVE_X11=true
  AC_DEFINE(GOPTICAL_HAVE_X11, 1, [x11 rendering library enabled])
  LDFLAGS+=" -lX11 "
])
AM_CONDITIONAL(GOPTICAL_HAVE_X11, test x$GOPTICAL_HAVE_X11 = xtrue)

AC_CHECK_HEADER(plplot/plstream.h, [
  GOPTICAL_HAVE_PLPLOT=true
  AC_DEFINE(GOPTICAL_HAVE_PLPLOT, 1, [plplot rendering library enabled])
  LDFLAGS+=" -lplplotcxxd "
])
AM_CONDITIONAL(GOPTICAL_HAVE_PLPLOT, test x$GOPTICAL_HAVE_PLPLOT = xtrue)

AC_CONFIG_FILES([ Makefile src/Makefile src/Goptical/Makefile
  src/Goptical/Analysis/Makefile src/Goptical/Curve/Makefile
  src/Goptical/Data/Makefile src/Goptical/Io/Makefile
  src/Goptical/Light/Makefile src/Goptical/Material/Makefile
  src/Goptical/Math/Makefile src/Goptical/Shape/Makefile
  src/Goptical/Sys/Makefile src/Goptical/Trace/Makefile tests/Makefile
  ])

AC_OUTPUT


