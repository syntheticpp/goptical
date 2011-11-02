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


#ifndef GOPTICAL_SHAPE_ELLIPSE_HXX_
#define GOPTICAL_SHAPE_ELLIPSE_HXX_

#include <algorithm>

#include "base.hxx"

namespace _Goptical {

  namespace Shape {

    double EllipseBase::get_x_radius(void) const
    {
      return _xr;
    }

    double EllipseBase::get_y_radius(void) const
    {
      return _yr;
    }

    double EllipseBase::max_radius() const
    {
      return std::max(_yr, _xr);
    }

    double EllipseBase::min_radius() const
    {
      return std::min(_yr, _xr);
    }

    double EllipseBase::get_external_xradius() const
    {
      return _xr;
    }

    double EllipseBase::get_internal_xradius() const
    {
      return 0.0;
    }

    double EllipseBase::get_xy_ratio() const
    {
      return _xy_ratio;
    }

  }
}

#endif

