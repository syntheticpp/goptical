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

AC_INIT(goptical_design, 1.0)
PACKAGE_VERSION=1.0
AC_SUBST(PACKAGE_VERSION)

AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_MACRO_DIR(build-aux)

AM_INIT_AUTOMAKE(goptical_design, 1.0)

AC_PROG_CXX
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_LANG(C++)

CXXFLAGS="$CXXFLAGS -Wall "
CXXFLAGS="$GOPTICAL_CXXFLAGS $CXXFLAGS"
LDFLAGS="$GOPTICAL_LDFLAGS $LDFLAGS"

AC_ARG_ENABLE(tests, AC_HELP_STRING(--disable-tests, [Disable compilation of tests]), disable_tests=yes)
AM_CONDITIONAL(TESTS, test x$disable_tests != xyes)

AC_ARG_ENABLE(debug, AC_HELP_STRING(--enable-debug, [Enable debug and asserts]), enable_debug=yes)
if test "x$enable_debug" = "xyes" ; then
   CXXFLAGS="$CXXFLAGS -g "
else
   CPPFLAGS="$CPPFLAGS -DNDEBUG "
fi

AC_ARG_WITH([goptical-core-include],
  AS_HELP_STRING([--with-goptical-core-include], [location of the Goptical core library headers]), [
  CPPFLAGS="-I$withval $CPPFLAGS"
])

AC_ARG_WITH([goptical-core-lib],
  AS_HELP_STRING([--with-goptical-core-lib], [location of the Goptical core library file]), [
  GOPTICAL_CORE_LIB="$withval"
], [
  GOPTICAL_CORE_LIB="-lgoptical"
])
AC_SUBST([GOPTICAL_CORE_LIB])

AC_CHECK_LIB(m, cosf, , AC_MSG_ERROR([can not find m library]))

AC_CHECK_HEADER(Goptical/common.hh, , AC_MSG_ERROR([GNU Goptical core library headers not found]))

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/Goptical/Makefile
  src/Goptical/Design/Makefile
  src/Goptical/Design/Telescope/Makefile
  tests/Makefile
])

AC_OUTPUT

