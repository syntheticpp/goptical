/*

      This file is part of the Goptical Design library.
  
      The Goptical library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
  
      The Goptical library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public
      License along with the Goptical library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
  
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet
      

*/

#include <Goptical/Math/Vector>

#include <Goptical/Sys/System>
#include <Goptical/Sys/SourcePoint>
#include <Goptical/Design/Telescope/Cassegrain>
#include <Goptical/Sys/Image>

#include <Goptical/Trace/Tracer>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Params>
#include <Goptical/Trace/Sequence>

#include <Goptical/Io/RendererSvg>
#include <Goptical/Io/RendererViewport>

using namespace Goptical;

#define COMPARE(a, b)                                                   \
  if (fabs((a)-(b)) > 1e-4)                                             \
    {                                                                   \
      std::cerr << "at " << __LINE__ << a << " " << b << std::endl;     \
      exit(1);                                                          \
    }

int main()
{
#if 0
  Sys::System             sys;

  Sys::SourcePoint        source(Sys::SourceAtInfinity, Math::vector3_001);
  sys.add(source);
  //  source.rotate(1.4, 0, 0);

  Design::Telescope::Cassegrain<Design::Telescope::RitcheyChretien>
    telescope(Math::vector3_0, 2000, 8./3., 250, 100, 2);
  sys.add(telescope);

  Sys::Image              image(telescope.get_focal_plane(), telescope.get_unvignetted_image_diameter() / 2);
  sys.add(image);

  Trace::Sequence seq(sys);
  std::cout << seq;

  Trace::Tracer         tracer(sys);

  sys.set_entrance_pupil(telescope.get_primary());

  tracer.get_params().set_default_distribution(Trace::Distribution(Trace::CrossDist, 5));
  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  Io::RendererSvg       svg_renderer("cassegrain_layout.svg", 700, 300);
  Io::RendererViewport  &renderer = svg_renderer;

  sys.draw_2d_fit(renderer);
  sys.draw_2d(renderer);

  tracer.get_trace_result().draw_2d(renderer);
#endif

  {
    Design::Telescope::Cassegrain<Design::Telescope::RitcheyChretien>
      telescope(Math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), -1.12582);
    COMPARE(telescope.get_secondary_conic(), -6.50737);
  }

  {
    Design::Telescope::Cassegrain<Design::Telescope::ClassicalCassegrain>
      telescope(Math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), -1);
    COMPARE(telescope.get_secondary_conic(), -4.84);
  }

  {
    Design::Telescope::Cassegrain<Design::Telescope::DallKirkham>
      telescope(Math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), -0.6347);
    COMPARE(telescope.get_secondary_conic(), 0);
  }

  {
    Design::Telescope::Cassegrain<Design::Telescope::PressmannCamichel>
      telescope(Math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), 0);
    COMPARE(telescope.get_secondary_conic(), 8.4117);
  }

  return 0;
}

