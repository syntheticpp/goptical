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

LDADD = $(top_builddir)/src/libgoptical.la

AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = test_discrete_set test_coordinates test_rendering     \
        test_2d_plot test_shapes test_materials test_patterns

TESTS = test_discrete_set test_coordinates test_materials test_patterns

test_discrete_set_SOURCES = test_discrete_set.cc
test_coordinates_SOURCES = test_coordinates.cc
test_rendering_SOURCES = test_rendering.cc
test_2d_plot_SOURCES = test_2d_plot.cc
test_shapes_SOURCES = test_shapes.cc
test_materials_SOURCES = test_materials.cc
test_patterns_SOURCES = test_patterns.cc

EXTRA_DIST = test_discrete_set-Cubic2DerivInit.txt                      \
        test_discrete_set-Cubic2Deriv.txt                               \
        test_discrete_set-Cubic2.txt                                    \
        test_discrete_set-CubicDerivInit.txt                            \
        test_discrete_set-CubicDeriv.txt                                \
        test_discrete_set-CubicSimple.txt test_discrete_set-Cubic.txt   \
        test_discrete_set-in.txt test_discrete_set-Linear.txt           \
        test_discrete_set-Nearest.txt test_discrete_set-Quadratic.txt

clean-local:
	rm -f *.svg

