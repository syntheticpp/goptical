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

#include <Goptical/Sys/System>
#include <Goptical/Sys/SourcePoint>
#include <Goptical/Sys/Image>

#include <Goptical/Trace/Tracer>
#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Params>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Sequence>

#include <Goptical/Math/Vector>
#include <Goptical/Math/Quaternion>
#include <Goptical/Math/Transform>

#include <Goptical/Design/Telescope/Newton>

using namespace Goptical;

template <class X>
class Newton : public X
{

public:
  Newton();

  void redraw();
  void resize(int width, int height);

  Sys::System sys;
  Trace::Tracer tracer;
};

template <class X>
Newton<X>::Newton()
  : X(),
    sys(),
    tracer(sys)
{
  // optical system

  Sys::SourcePoint      source(Sys::SourceAtInfinity, Math::vector3_001);
  sys.add(source);

  Design::Telescope::Newton     newton(Math::vector3_0, 750, 250);
  sys.add(newton);

  Sys::Image            image(newton.get_focal_plane(), 15);
  sys.add(image);

  sys.set_entrance_pupil(newton.get_primary());

  // ray trace

  tracer.get_params().set_default_distribution(
    Trace::Distribution(Trace::HexaPolarDist, 3));

  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  // viewport setup

  sys.draw_3d_fit(*X::renderer);

  X::translation = X::renderer->get_camera_transform().get_translation();
  X::rotation = Math::Vector3(0, 90, 0);

  X::main_loop();
}

template <class X>
void Newton<X>::redraw()
{
  X::renderer->clear();

  Math::Transform<3> t(
         Math::Quaternion::angle(Math::vector3_100, X::rotation.x()) * 
         Math::Quaternion::angle(Math::vector3_010, X::rotation.y()) *
         Math::Quaternion::angle(Math::vector3_001, X::rotation.z()),
         X::translation);

  X::renderer->set_camera_transform(t);

  // draw rays
  tracer.get_trace_result().draw_3d(*X::renderer);
  // draw system
  sys.draw_3d(*X::renderer);

  X::renderer->flush();
}

template <class X>
void Newton<X>::resize(int width, int height)
{
  // set output window size
  X::renderer->set_2d_size(width, height);

#if 1
  // use perspective projection
  X::renderer->set_perspective();
#else
  // use orthographic projection
  sys.draw_2d_fit(*X::renderer);
#endif
}

