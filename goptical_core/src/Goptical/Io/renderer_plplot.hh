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

#ifndef GOPTICAL_RENDERER_PLPLOT_HH_
#define GOPTICAL_RENDERER_PLPLOT_HH_

#include <map>

#include "Goptical/common.hh"

#include "Goptical/Io/renderer_2d.hh"

class plstream;

namespace _Goptical {

  namespace Io {

    /**
       @short Plplot rendering driver
       @header Goptical/Io/RendererPlplot
       @module {Core}
       @main

       This class implements a Plplot graphic output driver. It needs
       the plplot library to compile.
     */
    class RendererPlplot : public Renderer2d
    {
    public:

      /** Initialize renderer from existing plplot context */
      RendererPlplot(plstream *pls);

      /** Create a plplot renderer with specified output size */
      RendererPlplot(const char *device, const char *filename,
                     unsigned int width = 800, unsigned int height = 600,
                     const Rgb &background = rgb_white);

      ~RendererPlplot();

      /** get plplot context object */
      inline plstream * get_pls();

      /** @override */
      void set_page_layout(unsigned int cols, unsigned int rows);
      /** @override */
      void set_page(unsigned int page);

    private:

      /** @override */
      void draw_point(const Math::Vector2 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const Math::VectorPair2 &l, const Rgb &rgb);
      /** @override */
      void draw_circle(const Math::Vector2 &c, double r,
                       const Rgb &rgb, bool filled);
      /** @override */
      void draw_polygon(const Math::Vector2 *array, unsigned int count,
                        const Rgb &rgb, bool filled, bool closed);
      /** @override */
      void draw_text(const Math::Vector2 &c, const Math::Vector2 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

      /** @override */
      void draw_axes_2d(const RendererAxes &a);
      /** @override */
      void draw_axes_3d(const RendererAxes &a, const Math::Vector3 &range);
      /** @override */
      void draw_plot(const Data::Plot &plot);

      /** @override */
      void update_2d_window();
      /** @override */
      void set_2d_size(double width, double height);

      int get_color_id(const Rgb &rgb);

      /** setup plplot vpor & wind for drawing */
      void pl_set_drawing_window();

      /** setup plplot vpor & wind for data plot */
      void pl_set_plot_window();

      /** draw axes */
      void pl_draw_axes(const RendererAxes &a);

      void palette_init();

      typedef std::map<const Rgb, int> palette_t;
      plstream *_pls;
      bool _user_pls;
      palette_t _palette;
      int _palette_size;
    };

  }
}

#endif

