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

#include <Goptical/Shape/Disk>
#include <Goptical/Math/VectorPair>

#include "shape_round_.hxx"

namespace _Goptical {

  namespace Shape {

    bool DiskBase::inside(const Math::Vector2 &point) const
    {
      return (Math::square(point.x()) + Math::square(point.y()) <= Math::square(_radius));
    }

    Math::VectorPair2 DiskBase::get_bounding_box() const
    {
      Math::Vector2 hs(_radius, _radius);

      return Math::VectorPair2(-hs, hs);
    }

    double DiskBase::get_outter_radius(const Math::Vector2 &dir) const
    {
      return _radius;
    }

    template class Round<DiskBase, false>;

  }

}

