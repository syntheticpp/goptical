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


#include <Goptical/Shape/Ring>
#include <Goptical/Math/VectorPair>

#include "shape_round_.hxx"

namespace _Goptical {

  namespace Shape {

    bool RingBase::inside(const Math::Vector2 &point) const
    {
      double d = Math::square(point.x()) + Math::square(point.y());

      return d <= Math::square(_radius) && d >= Math::square(_hole_radius);
    }

    Math::VectorPair2 RingBase::get_bounding_box() const
    {
      Math::Vector2 hs(_radius, _radius);

      return Math::VectorPair2(-hs, hs);
    }

    double RingBase::get_outter_radius(const Math::Vector2 &dir) const
    {
      return _radius;
    }

    double RingBase::get_hole_radius(const Math::Vector2 &dir) const
    {
      return _hole_radius;
    }

    template class Round<RingBase, true>;

  }

}

