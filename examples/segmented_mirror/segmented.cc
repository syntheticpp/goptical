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

#include <Goptical/Sys/System>
#include <Goptical/Sys/Mirror>
#include <Goptical/Sys/Image>
#include <Goptical/Sys/Stop>
#include <Goptical/Sys/Group>
#include <Goptical/Sys/SourcePoint>

#include <Goptical/Shape/Base>
#include <Goptical/Shape/Disk>
#include <Goptical/Shape/Ring>
#include <Goptical/Shape/RegularPolygon>

#include <Goptical/Curve/Base>
#include <Goptical/Curve/Composer>
#include <Goptical/Curve/Conic>

#include <Goptical/Trace/Tracer>
#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Params>
#include <Goptical/Trace/Result>

#include <Goptical/Io/RendererX3d>
#include <Goptical/Io/Renderer>

#include <Goptical/Math/VectorPair>
#include <Goptical/Math/Vector>

#include <Goptical/Error>

using namespace Goptical;

/* anchor hexseg1 */
class HexSegMirror : public Sys::Group
{
public:

  HexSegMirror(const Math::VectorPair3 &pos,
               const const_ref<Curve::Base> &curve,
               const const_ref<Shape::Base> &shape,
               double seg_radius, double separation)
    : Sys::Group(pos)
  {
/* anchor hexseg2 */
    if (seg_radius > separation)
      throw(Error("overlapping segments"));

    // sqrt(3)/2
    static const double sqrt_3_2 = 0.86602540378443864676;

    // hexagonal tessellation
    int x_count = ceil(shape->max_radius() / (separation * 1.5));
    int y_count = ceil(shape->max_radius() / (separation * 2 * sqrt_3_2));

    for (int x = -x_count; x <= x_count ; x++)
      {
        for (int y = -y_count; y <= y_count ; y++)
          {
            // find segment mirror 2d position
            double yoffset = x % 2 ? separation * sqrt_3_2 : 0;
            Math::Vector2 p(x * separation * 1.5,
                              yoffset + y * separation * 2 * sqrt_3_2 );
/* anchor hexseg3 */
            // skip if segment center is outside main shape
            if (!shape->inside(p))
              continue;
/* anchor hexseg4 */
            // find curve z offset at segment center to shift both
            // curve and segment in opposite directions.
            double z_offset = curve->sagitta(p);

            // create a composer curve for this segment and use it to translate main curve
            ref<Curve::Composer> seg_curve = ref<Curve::Composer>::create();

            seg_curve->add_curve(curve).xy_translate(-p).z_offset(-z_offset);
/* anchor hexseg5 */
            // create a segment mirror with hexagonal shape and translated curve
            ref<Sys::Mirror> seg = ref<Sys::Mirror>::create(Math::Vector3(p, z_offset), seg_curve,
                                             ref<Shape::RegularPolygon>::create(seg_radius, 6));

            // attach the new segment to our group component
            add(seg);
/* anchor hexseg6 */
            // keep a pointer to this new segment
            _segments.push_back(seg.ptr());
          }
      }
  }

  size_t get_segments_count() const
  {
    return _segments.size();
  }

  Sys::Mirror & get_segment(size_t i) const
  {
    return *_segments.at(i);
  }

private:
  std::vector<Sys::Mirror *> _segments;
};
/* anchor end */

int main()
{

  // System focal length                           2400
  // Back focal length                              100

  // Primary mirror diameter                        300
  // Primary mirror focal length                    800
  // Primary mirror conic constant              -1.0869

  // Distance secondary from system focus           675
  // Distance secondary from prime focus            225
  // Distance secondary from primary                575
  // Radius of curvature of secondary mirror        675
  // Secondary mirror conic constant            -5.0434

  /* anchor rc */
  Sys::System             sys;

  // Ring shaped segmented mirror with conic curve
  HexSegMirror            primary(Math::Vector3(0, 0, 800),
                                  ref<Curve::Conic>::create(-1600, -1.0869),
                                  ref<Shape::Ring>::create(300, 85),
                                  28, 30);
  sys.add(primary);

  Sys::Mirror             secondary(Math::VectorPair3(0, 0, 225, 0, 0, -1), 675, -5.0434, 100);
  sys.add(secondary);

  Sys::Image              image(Math::VectorPair3(0, 0, 900), 15);
  sys.add(image);

  Sys::Stop               stop(Math::vector3_0, 300);
  sys.add(stop);
  sys.set_entrance_pupil(stop);

  Sys::SourcePoint        source(Sys::SourceAtInfinity, Math::vector3_001);
  sys.add(source);
  /* anchor end */

//  primary.get_segment(5).rotate(0.001, 0, 0);

  /* anchor layout */
  Trace::Tracer         tracer(sys);

  // trace rays through the system
  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  Io::RendererX3d       x3d_renderer("layout.x3d");
  Io::Renderer          &renderer = x3d_renderer;

  sys.draw_3d(renderer);
  tracer.get_trace_result().draw_3d(renderer, true);
  /* anchor end */

  return 0;
}

