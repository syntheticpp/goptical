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


#ifndef GOPTICAL_RENDERER_OPENGL_HXX_
#define GOPTICAL_RENDERER_OPENGL_HXX_

#include "Goptical/Io/renderer.hxx"

namespace _Goptical {

  namespace Io {

    void RendererOpengl::glVertex(const Math::Vector2 &v)
    {
      glVertex2d(v.x(), v.y());
    }

    void RendererOpengl::glVertex(const Math::Vector3 &v)
    {
      glVertex3d(v.x(), v.y(), v.z());
    }

    void RendererOpengl::glNormal(const Math::Vector3 &v)
    {
      glNormal3d(v.x(), v.y(), v.z());
    }

    void RendererOpengl::glColor(const Rgb &rgb)
    {
      glColor4f(rgb.r, rgb.g, rgb.b, rgb.a);
    }

  }
}

#endif

