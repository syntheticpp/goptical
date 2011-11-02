/*

      This file is part of the Goptical Core library.
  
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

#include <cstdio>
#include <stdlib.h>

#include "config.hh"

#include <Goptical/Io/Renderer>
#include <Goptical/Io/RendererViewport>
#include <Goptical/Io/Rgb>
#include <Goptical/Io/RendererSvg>

#include <Goptical/Trace/Distribution>

#include <Goptical/Shape/Base>
#include <Goptical/Shape/Disk>
#include <Goptical/Shape/Ring>
#include <Goptical/Shape/Ellipse>
#include <Goptical/Shape/EllipticalRing>
#include <Goptical/Shape/Rectangle>
#include <Goptical/Shape/RegularPolygon>
#include <Goptical/Shape/Polygon>
#include <Goptical/Shape/Composer>

#include <Goptical/Math/Vector>
#include <Goptical/Math/Triangle>

using namespace Goptical;

int main()
{
  struct shape_test_s
  {
    const char *name;
    Shape::Base *s;
  };

  Shape::Polygon *poly1 = new Shape::Polygon;

  poly1->add_vertex(Math::Vector2(30, 20));
  poly1->add_vertex(Math::Vector2(20, 30));
  poly1->add_vertex(Math::Vector2(-20, 30));
  poly1->add_vertex(Math::Vector2(-30, 20));
  poly1->add_vertex(Math::Vector2(-30, -10));
  poly1->add_vertex(Math::Vector2(-20, -20));
  poly1->add_vertex(Math::Vector2(20, -20));
  poly1->add_vertex(Math::Vector2(30, -10));

  srand48(time(0));

#if 0
  Shape::Polygon *poly2 = new Shape::Polygon;

  for (double a = 0; a < 2.0 * M_PI - 1e-8; a += 2.0 * M_PI / 15)
    {
      double r = (5 + drand48() * 30);
      poly2->add_vertex(Math::Vector2(cos(a) * r,
                                        sin(a) * r));
    }
#endif

  Shape::Composer *compose1 = new Shape::Composer;

  compose1->add_shape(ref<Shape::Disk>::create(30.))
    .translate(20.)
    .exclude(ref<Shape::Rectangle>::create(20.));

  compose1->add_shape(ref<Shape::Disk>::create(20.))
    .translate(-20.);

  shape_test_s st[] = {
    { "disk", new Shape::Disk(30) },
    { "ring", new Shape::Ring(30, 15) },
    { "ellipse1", new Shape::Ellipse(20, 35) },
    { "ellipse2", new Shape::Ellipse(35, 20) },
    { "ellipticalring1", new Shape::EllipticalRing(35, 20, 20) },
    { "ellipticalring2", new Shape::EllipticalRing(20, 35, 10) },
    { "rectangle", new Shape::Rectangle(70., 40.) },
    { "regularpolygon6", new Shape::RegularPolygon(30, 6) },
    { "regularpolygon5", new Shape::RegularPolygon(30, 5) },
    { "regularpolygon5r", new Shape::RegularPolygon(30, 5, 10.) },
    { "polygon1", poly1 },
    //    { "polygon2", poly2 },
    { "composer1", compose1 },
    { 0 }
  };

  for (int i = 0; st[i].name; i++)
    {
      shape_test_s &s = st[i];

      char fname[48];
      std::sprintf(fname, "test_shape_%s.svg", s.name);

      Io::RendererSvg rsvg(fname, 800, 600, Io::rgb_black);
      Io::RendererViewport &r = rsvg;

      r.set_window(Math::vector2_0, 70);

      {
        DPP_DELEGATE1_OBJ(d, void, (const Math::Triangle<2> &t),
                          Io::Renderer &, r, // _0
        {
          _0.draw_triangle(t, true, Io::Rgb(.2, .2, .2));
          _0.draw_triangle(t, false, Io::rgb_gray);
        });
        s.s->get_triangles(d, 10.);
      }

      for (unsigned int c = 0; c < s.s->get_contour_count(); c++)
        {
          std::vector<Math::Vector2 > poly;
          delegate_push<typeof(poly)> d(poly);
          s.s->get_contour(c, d, 10.);
          r.draw_polygon(&poly[0], poly.size(), Io::rgb_yellow);
        }

      for (double a = 0; a < 2.0 * M_PI - 1e-8; a += 2.0 * M_PI / 50)
        {
          Math::Vector2 d(cos(a), sin(a));

          double ro = s.s->get_outter_radius(d);
          r.draw_point(d * ro, Io::rgb_magenta, Io::PointStyleCross);
          double rh = s.s->get_hole_radius(d);
          r.draw_point(d * rh, Io::rgb_cyan, Io::PointStyleCross);
        }

      r.draw_circle(Math::vector2_0, s.s->max_radius(), Io::rgb_red);
      r.draw_circle(Math::vector2_0, s.s->min_radius(), Io::rgb_blue);

      r.draw_box(s.s->get_bounding_box(), Io::rgb_cyan);

      {
        DPP_DELEGATE1_OBJ(d, void, (const Math::Vector2 &v),
                          Io::Renderer &, r, // _0
        {
          _0.draw_point(v, Io::rgb_green);
        });

        Trace::Distribution dist;
        s.s->Base::get_pattern(d, dist);
      }
    }

}

