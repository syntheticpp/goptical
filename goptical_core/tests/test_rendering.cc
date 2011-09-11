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

#include "config.hh"

#include <Goptical/Io/Renderer>
#include <Goptical/Io/RendererViewport>
#include <Goptical/Io/Rgb>

#include <Goptical/Io/RendererSvg>
#include <Goptical/Io/RendererX3d>
#ifdef GOPTICAL_HAVE_PLPLOT
#include <Goptical/Io/RendererPlplot>
#endif
#ifdef GOPTICAL_HAVE_GD
#include <Goptical/Io/RendererGd>
#endif
#ifdef GOPTICAL_HAVE_PLPLOT
#include <Goptical/Io/RendererPlplot>
#endif
#ifdef GOPTICAL_HAVE_DIME
#include <Goptical/Io/RendererDxf>
#endif
#ifdef GOPTICAL_HAVE_X11
#include <Goptical/Io/RendererX11>
#endif

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>

using namespace Goptical;

void test_text_(Io::Renderer &r, const Math::Vector2 &pos, Io::TextAlignMask a)
{
  using namespace Io;

  for (double ra = 0; ra < 2.*M_PI - 1.e-6; ra += M_PI/6.)
    {
      Math::Vector2 dir(cos(ra), sin(ra));

      r.draw_segment(Math::VectorPair2(pos, pos + dir * 30), rgb_blue);
      r.draw_text(pos, dir, "A long long long test string",
                  a, 12, rgb_red);
    }
}

void test_text(Io::Renderer &r)
{
  test_text_(r, Math::Vector2(-90, 90), Io::TextAlignLeft |   Io::TextAlignMiddle);
  test_text_(r, Math::Vector2(-30, 90), Io::TextAlignCenter | Io::TextAlignMiddle);
  test_text_(r, Math::Vector2(30,  90), Io::TextAlignRight |  Io::TextAlignMiddle);
  test_text_(r, Math::Vector2(90,  90), Io::TextAlignCenter | Io::TextAlignTop);
  test_text_(r, Math::Vector2(-90, 30), Io::TextAlignCenter | Io::TextAlignMiddle);
  test_text_(r, Math::Vector2(-30, 30), Io::TextAlignCenter | Io::TextAlignBottom);
}

void test_circle(Io::Renderer &r)
{
  for (double ra = 0; ra < 80 ; ra += 10)
    {
      r.draw_circle(Math::vector2_0, ra, Io::rgb_green, false);
    }

  for (double x = -90; x < 90 + 1e-6 ; x += 30)
    {
      Math::Vector2 v(x, 0.);
      r.draw_circle(v, 8, Io::rgb_gray, true);
    }
}

void test_polygon(Io::Renderer &r)
{
  Math::Vector2 v[5];

  v[0] = Math::Vector2(-10, -50);
  v[1] = Math::Vector2(-5, -40);
  v[2] = Math::Vector2(-15, -30);
  v[3] = Math::Vector2(10, -25);
  v[4] = Math::Vector2(5, -50);

  r.draw_polygon(v, 5, Io::rgb_magenta, false, false);

  for (int i = 0; i < 5; i++)
    v[i] += Math::Vector2(30, 0);

  r.draw_polygon(v, 5, Io::rgb_magenta, true, false);

  for (int i = 0; i < 5; i++)
    v[i] += Math::Vector2(30, 0);

  r.draw_polygon(v, 5, Io::rgb_magenta, false, true);

  for (int i = 0; i < 5; i++)
    v[i] += Math::Vector2(30, 0);

  r.draw_polygon(v, 5, Io::rgb_magenta, true, true);
}

void test_point(Io::Renderer &r)
{
  for (int i = 0; i < 5; i++)
    for (double x = -90; x < 90 + 1e-6 ; x += 5)
      r.draw_point(Math::Vector2(x, -90 + i*5),
                   r.get_style_color(Io::StyleForeground), (Io::PointStyle)i);
}

/* Special class which forces use of fallback implementations from base class */

#define BASERENDERER_CLASS(R, ...)                                      \
  struct R##Basic : public Io::R                                        \
{                                                                       \
  R##Basic()                                                            \
    : R(__VA_ARGS__)                                                    \
  {                                                                     \
  }                                                                     \
                                                                        \
  void draw_circle(const Math::Vector2 &v, double r, const Rgb &rgb, bool filled) \
  {                                                                     \
    Renderer::draw_circle(v, r, rgb, filled);                           \
  }                                                                     \
                                                                        \
  void draw_polygon(const Math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed) \
  {                                                                     \
    Renderer::draw_polygon(array, count, rgb, filled, closed);          \
  }                                                                     \
};

using namespace Io;

BASERENDERER_CLASS(RendererSvg, "test_basic_svg.svg", 1600, 1200);
#ifdef GOPTICAL_HAVE_DIME
BASERENDERER_CLASS(RendererDxf, "test_basic_dxf.dxf");
#endif

int main()
{
  Io::Renderer *rt[] = {
#ifdef GOPTICAL_HAVE_X11
    new Io::RendererX11 (),
#endif
    new Io::RendererSvg ("test_2d_svg.svg", 1600, 1200),
    new RendererSvgBasic(),
    new Io::RendererX3d ("test_2d_x3d.x3d"),
#ifdef GOPTICAL_HAVE_GD
    new Io::RendererGd ("test_2d_gd_ttf.png", 1600, 1200, Io::rgb_white, "/usr/share/fonts/TTF/FreeSans.ttf"),
    new Io::RendererGd ("test_2d_gd.png", 1600, 1200, Io::rgb_black),
#endif
#ifdef GOPTICAL_HAVE_PLPLOT
    new Io::RendererPlplot ("png", "test_2d_plplot.png", 1600, 1200),
    new Io::RendererPlplot ("ps", "test_2d_plplot.ps", 1600, 1200),
#endif
#ifdef GOPTICAL_HAVE_DIME
    new Io::RendererDxf ("test_2d_dxf.dxf"),
    new RendererDxfBasic (),
#endif
    0
  };

  for (int i = 0; rt[i]; i++)
    {
      Io::Renderer &r = *rt[i];
      Io::RendererViewport *rv = dynamic_cast<Io::RendererViewport*>(&r);

      if (rv)
        rv->set_window(Math::VectorPair2(-100, -100, 100, 100));

      test_circle(r);
      test_polygon(r);
      test_point(r);
      test_text(r);

      if (rv)
        {
          rv->draw_frame_2d();
          rv->flush();
        }
    }

  sleep(5);

  for (int i = 0; rt[i]; i++)
    {
      delete rt[i];
    }

  return 0;
}

