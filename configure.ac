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

CXXFLAGS+=" -Wall "

AC_ARG_ENABLE(debug, AC_HELP_STRING(--enable-debug, [Enable debug and asserts]), enable_tests=yes)
if test "x$enable_tests" = "xyes" ; then
   CXXFLAGS+=" -g "
else
   CPPFLAGS+=" -DNDEBUG "
fi

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

abs_srcdir=`cd $srcdir ; pwd`
abs_builddir=`pwd`
ac_configure_args+=" --with-goptical-core-include=$abs_srcdir/goptical_core/src "
ac_configure_args+=" --with-goptical-core-lib=$abs_builddir/goptical_core/src/libgoptical.la "

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

