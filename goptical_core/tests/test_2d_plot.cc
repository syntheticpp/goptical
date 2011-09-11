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
#include <Goptical/Io/RendererAxes>
#include <Goptical/Io/Rgb>

#include <Goptical/Io/RendererSvg>
#ifdef GOPTICAL_HAVE_PLPLOT
#include <Goptical/Io/RendererPlplot>
#endif
#ifdef GOPTICAL_HAVE_GD
#include <Goptical/Io/RendererGd>
#endif
#ifdef GOPTICAL_HAVE_PLPLOT
#include <Goptical/Io/RendererPlplot>
#endif
#ifdef GOPTICAL_HAVE_X11
#include <Goptical/Io/RendererX11>
#endif

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>

#include <Goptical/Data/Plot>
#include <Goptical/Data/DiscreteSet>
#include <Goptical/Data/SampleSet>

using namespace Goptical;

void test_2d_plot(Io::RendererViewport &r)
{
  Data::DiscreteSet d1;
  Data::SampleSet d2;

  static const int N = 40.0;
  double x = -N/2.0;

  d2.resize(N);
  d2.set_interpolation(Data::Cubic2);
  
  for (int i = 0; i < N; i++)
    {
      d1.add_data(x, cos (x / 3.) * cos(x) / 2.);
      x += fabs(sin(i) + .5);

      d2.set_value(i, sin(x / 5.) * sin(x));
    }

  d1.set_interpolation(Data::Cubic2);

  Data::Plot p;

  p.get_axes().set_label("The X axis", Io::RendererAxes::X);
  p.get_axes().set_label("The Y axis", Io::RendererAxes::Y);
  p.set_title("A simple test plot");

  p.add_plot_data(d1, Io::rgb_red);
  p.add_plot_data(d2, Io::rgb_blue);

  p.draw(r);
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

int main()
{
  Io::RendererViewport *rt[] = {
#ifdef GOPTICAL_HAVE_X11
    new Io::RendererX11 (),
#endif
    new Io::RendererSvg ("test_plot_svg.svg", 800, 600),
    new RendererSvgBasic(),
#ifdef GOPTICAL_HAVE_GD
    new Io::RendererGd ("test_plot_gd_ttf.png", 800, 600, Io::rgb_white, "/usr/share/fonts/TTF/FreeSans.ttf"),
    new Io::RendererGd ("test_plot_gd.png", 800, 600, Io::rgb_black),
#endif
#ifdef GOPTICAL_HAVE_PLPLOT
    new Io::RendererPlplot ("png", "test_plot_plplot.png", 800, 600),
    new Io::RendererPlplot ("ps", "test_plot_plplot.ps", 800, 600),
#endif
    0
  };

  for (int i = 0; rt[i]; i++)
    {
      Io::RendererViewport &r = *rt[i];

      test_2d_plot(r);
      r.flush();
    }

  sleep(5);

  for (int i = 0; rt[i]; i++)
    {
      delete rt[i];
    }

  return 0;
}

