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
#include <cstring>
#include <stdlib.h>

#include "config.hh"

#include <Goptical/Io/RendererViewport>
#include <Goptical/Io/RendererSvg>
#include <Goptical/Io/Rgb>

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

using namespace Goptical;

#define SINGLE_IMAGE

size_t err = 0;

int main()
{
  struct shape_test_s
  {
    const char *name;
    Shape::Base *s;
    bool unobstructed;
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

  shape_test_s st[] = {
    { "disk", new Shape::Disk(30) },
    { "ring", new Shape::Ring(30, 15) },
    { "ring_unob", new Shape::Ring(30, 15), true },
    { "ring2", new Shape::Ring(35, 10) },
    { "ring2_unob", new Shape::Ring(35, 10), true },
    { "ellipse1", new Shape::Ellipse(20, 30) },
    { "ellipse2", new Shape::Ellipse(30, 20) },
    { "ellipticalring1", new Shape::EllipticalRing(30, 20, 20) },
    { "ellipticalring1_unob", new Shape::EllipticalRing(30, 20, 20), true },
    { "ellipticalring2", new Shape::EllipticalRing(20, 32, 10) },
    { "ellipticalring2_unob", new Shape::EllipticalRing(20, 32, 10), true },
    { "rectangle", new Shape::Rectangle(70., 40.) },
    { "regularpolygon6", new Shape::RegularPolygon(30, 6) },
    { "regularpolygon5", new Shape::RegularPolygon(30, 5) },
    { "regularpolygon5r", new Shape::RegularPolygon(30, 5, 10.) },
    { "polygon1", poly1 },
    { 0 }
  };

  const char *pname[] = {
    "default",
    "sagittal",
    "tangential",
    "cross",
    "square",
    "triangular",
    "hexpolar",
    "random"
  };

  for (int i = 0; st[i].name; i++)
    {
      shape_test_s &s = st[i];

#ifdef SINGLE_IMAGE
      char fname[48];
      std::sprintf(fname, "test_pattern_%s.svg", s.name);

      Io::RendererSvg rsvg(fname, 800, 400, Io::rgb_white);
      Io::RendererViewport &r = rsvg;

      r.set_page_layout(4, 2);
#endif

      for (int j = 0; j <= Trace::RandomDist; j++)
        {
#ifndef SINGLE_IMAGE
          char fname[48];
          std::sprintf(fname, "test_pattern_%s_%s.svg", s.name, pname[j]);

          Io::RendererSvg rsvg(fname, 400, 400, Io::rgb_white);
          Io::RendererViewport &r = rsvg;
#else
          r.set_page(j);
#endif

          std::cerr << s.name << " " << pname[j] << std::endl;

          r.set_window(Math::vector2_0, 70);
          r.set_feature_size(.1);

          std::vector<Math::Vector2 > pts;
          dpp::delegate_push<typeof(pts)> d(pts);

          try {
            Trace::Distribution dist((Trace::Pattern)j);
            s.s->get_pattern(d, dist, s.unobstructed);
          } catch (...) {
            continue;
          }

          if (pts.size() < 4)
            err++;

          bool first = true;

          GOPTICAL_FOREACH(v, pts)
            {
              r.draw_point(*v, Io::rgb_red, Io::PointStyleCross);

              // Chief ray must be the first ray in list, some analysis do rely on this
              if (!first && v->close_to(Math::vector2_0, 1) && j != Trace::RandomDist)
                {
                  std::cerr << "-- chief !first " << *v << "\n";
                  err++;
                }

              if (!s.unobstructed && !s.s->inside(*v))
                {
                  std::cerr << "-- !inside " << *v << "\n";
                  err++;
                }
              
              if (j != Trace::RandomDist)
                {
                  // check for duplicates
                  GOPTICAL_FOREACH(w, pts)
                    if (&*v != &*w && v->close_to(*w, 1))
                      {
                        std::cerr << "-- dup " << *w << *v << "\n";
                        err++;
                      }
                }

              first = false;
            }

          for (unsigned int c = 0; c < s.s->get_contour_count(); c++)
            {
              std::vector<Math::Vector2 > poly;
              delegate_push<typeof(poly)> d(poly);
              s.s->get_contour(c, d, 10.);
              r.draw_polygon(&poly[0], poly.size(), Io::rgb_black);
            }

          r.draw_text(Math::Vector2(0, -43), Math::vector2_10,
                      fstring("%s: %i points", pname[j], pts.size()),
                      //pname[j],
                      Io::TextAlignCenter | Io::TextAlignBottom, 18);
        }

      //      r.draw_pages_grid(Io::rgb_black);
    }

  return err > 0;
}

