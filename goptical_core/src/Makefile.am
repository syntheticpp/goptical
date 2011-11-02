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

lib_LTLIBRARIES = libgoptical.la

libgoptical_la_SOURCES = curve_array.cc curve_base.cc curve_composer.cc      \
	curve_conic_base.cc curve_conic.cc curve_flat.cc                \
	curve_foucault.cc curve_grid.cc curve_parabola.cc               \
	curve_polynomial.cc curve_rotational.cc curve_sphere.cc         \
	curve_spline.cc curve_zernike.cc data_discrete_set.cc           \
	data_grid.cc data_plot.cc data_sample_set.cc data_set1d.cc      \
	data_set.cc light_ray.cc light_spectral_line.cc                 \
	material_air.cc material_catalog.cc material_base.cc                 \
	material_proxy.cc material_conrady.cc material_dielectric.cc    \
	material_dispersion_table.cc material_herzberger.cc             \
	material_metal.cc material_mirror.cc material_schott.cc         \
	material_sellmeier.cc material_abbe.cc                          \
	material_sellmeiermod.cc material_vacuum.cc math_matrix.cc      \
	math_transform.cc shape_base.cc shape_composer.cc shape_disk.cc      \
	shape_ellipse.cc shape_elliptical_ring.cc shape_infinite.cc     \
	shape_polygon.cc shape_rectangle.cc shape_regular_polygon.cc    \
	shape_ring.cc sys_container.cc sys_element.cc sys_group.cc      \
	sys_image.cc sys_lens.cc sys_mirror.cc sys_optical_surface.cc   \
	sys_source_point.cc sys_source_rays.cc sys_source.cc            \
	sys_surface.cc sys_system.cc sys_stop.cc trace_tracer.cc        \
	trace_result.cc trace_sequence.cc io_import_oslo.cc             \
	io_import_zemax.cc io_renderer_svg.cc io_renderer_x3d.cc        \
	io_renderer_axes.cc io_renderer.cc io_renderer_viewport.cc      \
	io_renderer_2d.cc io_rgb.cc data_interpolate_1d_.hxx            \
	shape_round_.hxx analysis_focus.cc analysis_rayfan.cc           \
	analysis_spot.cc analysis_pointimage.cc

if GOPTICAL_HAVE_DIME
libgoptical_la_SOURCES += io_renderer_dxf.cc
endif

if GOPTICAL_HAVE_GD
libgoptical_la_SOURCES += io_renderer_gd.cc
endif

if GOPTICAL_HAVE_GL
libgoptical_la_SOURCES += io_renderer_opengl.cc
endif

if GOPTICAL_HAVE_X11
libgoptical_la_SOURCES += io_renderer_x11.cc
endif

if GOPTICAL_HAVE_PLPLOT
libgoptical_la_SOURCES += io_renderer_plplot.cc
endif

libgoptical_la_CPPFLAGS = -I$(top_srcdir)/include
libgoptical_la_LDFLAGS = -I$(top_srcdir)/include -release $(PACKAGE_VERSION)

