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

#ifndef GOPTICAL_RENDERER_X11_HH_
#define GOPTICAL_RENDERER_X11_HH_

#include <X11/Xlib.h>

#include "Goptical/common.hh"

#include "Goptical/Io/renderer_2d.hh"

namespace _Goptical {

  namespace Io {

    /**
       @short X11 rendering driver
       @header Goptical/Io/RendererX11
       @module {Core}
       @main

       This class implements a X11 graphic output driver.
     */
    class RendererX11 : public Renderer2d
    {
    public:

      RendererX11(unsigned int width = 800, unsigned int height = 600,
                  const Rgb &background = rgb_white,
                  Display *disp = 0, int src = -1,
                  Window win = -1, GC gc = 0);

      ~RendererX11();

      GOPTICAL_GET_ACCESSOR(Display *, disp, "X11 display");
      GOPTICAL_GET_ACCESSOR(int, scr, "X11 screen");
      GOPTICAL_GET_ACCESSOR(Window, win, "X11 window");
      GOPTICAL_GET_ACCESSOR(GC, gc, "X11 graphics context");

    private:

      /** @override */
      void clear();
      /** @override */
      void flush();

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

      inline double y_trans_pos(double y) const;
      inline Math::Vector2 trans_pos(const Math::Vector2 &v);

      long get_color(const Rgb &rgb) const;

      Display *_disp;
      bool _user_disp;
      int _scr;
      Window _win;
      GC _gc;
      bool _user_gc;
    };

  }

}

#endif

