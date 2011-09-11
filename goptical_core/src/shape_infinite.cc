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


#include <Goptical/Sys/Surface>
#include <Goptical/Shape/Infinite>
#include <Goptical/Math/Vector>
#include <Goptical/Error>

namespace _Goptical {

  namespace Shape {

    bool Infinite::inside(const Math::Vector2 &point) const
    {
      return true;
    }

    Math::VectorPair2 Infinite::get_bounding_box() const
    {
      return Math::vector2_pair_00;
    }

    void Infinite::get_pattern(const Math::Vector2::put_delegate_t  &f, const Trace::Distribution &d, bool unobstructed) const
    {
      throw Error("can not distribute rays accross an infinite surface shape");
    }

    unsigned int Infinite::get_contour_count() const
    {
      return 0;
    }

    void Infinite::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
    {
    }

    void Infinite::get_triangles(const Math::Triangle<2>::put_delegate_t  &t, double resolution) const
    {
    }

    double Infinite::get_outter_radius(const Math::Vector2 &dir) const
    {
      return 0;
    }

    Infinite infinite;

  }
}

