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


#ifndef GOPTICAL_RENDERER_GD_HH_
#define GOPTICAL_RENDERER_GD_HH_

extern "C" {
#include <gd.h>
#include <gdfontl.h>
}

#include "Goptical/common.hh"

#include "Goptical/Io/renderer_2d.hh"

namespace _Goptical {

  namespace Io {

    /**
       @short GD rendering driver
       @header Goptical/Io/RendererGd
       @module {Core}
       @main

       This class implements a Gd graphic output driver. It needs
       the gd library to compile.
     */
    class RendererGd : public Renderer2d
    {
    public:
      /** Create a new gd renderer using specified gd resources */
      RendererGd(gdImagePtr im, const Rgb &background = rgb_white,
                 gdFontPtr font = 0, const char *TFfontlist = 0);

      /** Create a new gd renderer with given resolution, png output
          will be written to file on renderer destruction. */
      RendererGd(const char *filename, unsigned int width = 800,
                 unsigned int height = 600, const Rgb &background = rgb_white,
                 const char *TFfontlist = 0);

      /** Create a new gd renderer with given resolution */
      RendererGd(unsigned int width = 800, unsigned int height = 600,
                 const Rgb &background = rgb_white, const char *TFfontlist = 0);

      ~RendererGd();

      void set_brush_thickness(unsigned int t);

      /** Write image to file in png format */
      void write(const std::string &filename);

      /** Clear current image buffer */
      void clear();

    private:

      /** @override */
      void set_2d_size(double width, double height);

      /** @override */
      void draw_ray_line(const Math::VectorPair3 &l, const Trace::Ray &ray);

      /** @override */
      void draw_point(const Math::Vector2 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const Math::VectorPair2 &l, const Rgb &rgb);
      /** @override */
      void draw_circle(const Math::Vector2 &c, double r, const Rgb &rgb, bool filled);
      /** @override */
      void draw_text(const Math::Vector2 &pos, const Math::Vector2 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);
      /** @override */
      void draw_polygon(const Math::Vector2 *array, unsigned int count,
                        const Rgb &rgb, bool filled, bool closed);

      inline double y_trans_pos(double y) const;
      inline Math::Vector2 trans_pos(const Math::Vector2 &v);

      void init(const Rgb &background);

      inline int rgb_to_gdcolor(const Rgb &rgb, float alpha) const;
      inline int rgb_to_gdcolor(const Rgb &rgb) const;
      inline int style_to_gdcolor(Style style) const;
      inline int ray_to_gdcolor(const Light::Ray & ray);

      bool _user_im;
      const char *_filename;
      unsigned int _thickness;

      gdImagePtr _im;
      gdFontPtr _font;
      const char *_fontlist;
    };

  }
}

#endif

