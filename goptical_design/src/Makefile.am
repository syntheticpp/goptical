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

SUBDIRS = Goptical

lib_LTLIBRARIES = libgoptical_design.la

libgoptical_design_la_SOURCES =	telescope_newton.cc telescope_cassegrain.cc
libgoptical_design_la_LDFLAGS = -release $(PACKAGE_VERSION)
libgoptical_design_la_LIBADD = $(GOPTICAL_CORE_LIB)

