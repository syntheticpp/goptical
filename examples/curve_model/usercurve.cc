/*

      This file is part of the Goptical library.
  
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

/* -*- indent-tabs-mode: nil -*- */

#include <iostream>
#include <fstream>

#include <Goptical/Math/Vector>

#include <Goptical/Sys/System>
#include <Goptical/Sys/SourcePoint>
#include <Goptical/Sys/Mirror>
#include <Goptical/Sys/Image>

#include <Goptical/Shape/Disk>

#include <Goptical/Curve/Rotational>

#include <Goptical/Trace/Tracer>
#include <Goptical/Trace/Sequence>
#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Params>

#include <Goptical/Analysis/Spot>
#include <Goptical/Analysis/Focus>

#include <Goptical/Io/RendererPlplot>
#include <Goptical/Io/RendererSvg>
#include <Goptical/Io/Rgb>

using namespace Goptical;

                                                                  /* anchor mycurve1 */
class MyCatenaryCurve : public Curve::Rotational
{
public:
  MyCatenaryCurve(double a)
    : _a(a)
  {
  }

private:
  double sagitta(double r) const
  {
    return _a * cosh(r / _a) - _a;
  }
                                                                  /* anchor mycurve2 */
  double derivative(double r) const
  {
    return sinh(r / _a);
  }
                                                                  /* anchor mycurve1 */

  double _a;
};
                                                                  /* anchor end */

int main()
{
  //**********************************************************************
  // Optical system definition

                                                                  /* anchor system */
  Sys::System             sys;

  // light source
  Sys::SourcePoint        source(Sys::SourceAtInfinity, Math::vector3_001);
  sys.add(source);

  // mirror
  Shape::Disk             shape(200);
  MyCatenaryCurve         curve(-3000);
  Sys::Mirror             primary(Math::Vector3(0, 0, 1500), curve, shape);

  sys.add(primary);

  // image plane
  Sys::Image              image(Math::vector3_0, 15);
  sys.add(image);
                                                                  /* anchor end */

  // set system entrance pupil
  sys.set_entrance_pupil(primary);


  Trace::Sequence seq;
  seq.append(source);
  seq.append(primary);
  seq.append(image);

  sys.get_tracer_params().set_sequential_mode(seq);
  sys.get_tracer_params().set_default_distribution(
    Trace::Distribution(Trace::HexaPolarDist, 8));


  {
                                                                  /* anchor focus */
    Analysis::Focus               focus(sys);

    image.set_plane(focus.get_best_focus());
                                                                  /* anchor end */
  }

  std::cout << seq << std::endl;

  {
                                                                  /* anchor spot */
    Io::RendererSvg            renderer("spot.svg",        200 * 3, 200 * 2, Io::rgb_black);

    renderer.set_margin_ratio(.35, .25, .1, .1);
    renderer.set_page_layout(3, 2);

    for (int i = 0; i < 6; i++)
      {
        Analysis::Spot spot(sys);

        renderer.set_page(i);
        spot.draw_diagram(renderer);

        source.rotate(0, .1, 0);
      }
                                                                  /* anchor end */
    return 0;
  }

}

