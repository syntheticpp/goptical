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

AC_INIT(goptical, 1.0)

AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_MACRO_DIR(build-aux)

AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_LANG(C++)

CXXFLAGS="$CXXFLAGS -Wall "

AC_ARG_ENABLE(debug, AC_HELP_STRING(--enable-debug, [Enable debug and asserts]), enable_debug=yes)
if test "x$enable_debug" = "xyes" ; then
   CXXFLAGS="$CXXFLAGS -g "
else
   CPPFLAGS="$CPPFLAGS -DNDEBUG "
fi

AC_CHECK_LIB(m, cosf, , AC_MSG_ERROR([can not find m library]))

AC_ARG_ENABLE(tests, AC_HELP_STRING(--disable-tests, [Disable compilation of tests]), disable_tests=yes)

# Some AC_ARG_ENABLE are only used in sub-packages but are declared here for --help
AC_ARG_ENABLE(gl, AC_HELP_STRING(--disable-gl, [Disable use of opengl renderer]), GOPTICAL_HAVE_GL=false)
AC_ARG_ENABLE(x11, AC_HELP_STRING(--disable-x11, [Disable use of X11 renderer]), GOPTICAL_HAVE_X11=false)
AC_ARG_ENABLE(dime, AC_HELP_STRING(--disable-dime, [Disable use of Dime DXF renderer]), GOPTICAL_HAVE_DIME=false)
AC_ARG_ENABLE(gd, AC_HELP_STRING(--disable-gd, [Disable use of libGd renderer]), GOPTICAL_HAVE_GD=false)
AC_ARG_ENABLE(plplot, AC_HELP_STRING(--disable-plplot, [Disable use of PlPlot renderer]), GOPTICAL_HAVE_PLPLOT=false)

#
# opengl library check
#

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

if test x$GOPTICAL_HAVE_X11 != xfalse ; then
AC_CHECK_HEADER(X11/Xlib.h, [ AC_CHECK_LIB(X11, XOpenDisplay, [
  GOPTICAL_HAVE_X11=true
  AC_DEFINE(GOPTICAL_HAVE_X11, 1, [x11 rendering library enabled])
  LIBS="$LIBS -lX11 "
])])
fi
AM_CONDITIONAL(GOPTICAL_HAVE_X11, test x$GOPTICAL_HAVE_X11 = xtrue)

abs_srcdir=`cd $srcdir ; pwd`
abs_builddir=`pwd`
ac_configure_args="$ac_configure_args --with-goptical-core-include=$abs_srcdir/goptical_core/src "
ac_configure_args="$ac_configure_args --with-goptical-core-lib=$abs_builddir/goptical_core/src/libgoptical.la "

AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  doc/texinfo/Makefile
  examples/Makefile
  examples/simple_refractor/Makefile
  examples/hierarchical_design/Makefile
  examples/render_3d/Makefile
  examples/curve_model/Makefile
  examples/tessar_lens/Makefile
  examples/segmented_mirror/Makefile
])

AC_CONFIG_SUBDIRS(goptical_core)
AC_CONFIG_SUBDIRS(goptical_design)
AC_OUTPUT

