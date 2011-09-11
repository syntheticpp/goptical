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


#ifndef GOPTICAL_RENDERER_2D_HH_
#define GOPTICAL_RENDERER_2D_HH_

#include "Goptical/common.hh"

#include "Goptical/Math/transform.hh"
#include "Goptical/Io/renderer_viewport.hh"

namespace _Goptical {

  namespace Io {

    /**
       @short Base class for 2d rendering drivers
       @header Goptical/Io/Renderer2d
       @module {Core}
       @internal

       This class provide default implementations for 3d projection
       and 3d drawing primitives. It's designed to be used as a base
       class for 2d only renderers so that they can perform 3d
       rendering too.
     */

    class Renderer2d : public RendererViewport
    {
    protected:
      Renderer2d();

      using Renderer::draw_point;
      using Renderer::draw_segment;
      using Renderer::draw_text;

      /** project in 2d space and scale for ploting to 2d output */
      inline Math::Vector2 project_scale(const Math::Vector3 &v);

      /** project in 2d space */
      inline Math::Vector2 project(const Math::Vector3 &v);

      /** @override */
      void set_perspective();
      /** @override */
      void set_orthographic();
      /** @override */
      Math::Transform<3> get_camera_transform() const;
      /** @override */
      void set_camera_transform(const Math::Transform<3> & t);

      /** @override */
      void draw_point(const Math::Vector3 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const Math::VectorPair3 &l, const Rgb &rgb);
      /** @override */
      void draw_text(const Math::Vector3 &pos, const Math::Vector3 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

    private:
      Math::Vector2 projection_ortho(const Math::Vector3 &v) const;
      Math::Vector2 projection_perspective(const Math::Vector3 &v) const;

      Math::Vector2 (Renderer2d::*_projection)(const Math::Vector3 &v) const;

      Math::Transform<3> _cam_transform;
      double _eye_dist;
    };

  }
}

#endif

