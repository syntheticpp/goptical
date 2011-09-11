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


#ifndef GOPTICAL_CURVE_SPLINE_HXX_
#define GOPTICAL_CURVE_SPLINE_HXX_

#include "Goptical/Curve/rotational.hxx"
#include "Goptical/Data/discrete_set.hxx"

namespace _Goptical {

  namespace Curve {

    const Data::DiscreteSet & Spline::get_data() const
    {
      return _data;
    }

    Data::DiscreteSet & Spline::get_data()
    {
      return _data;
    }

    double Spline::sagitta(double r) const
    {
      return _data.interpolate(r);
    }

    double Spline::derivative(double r) const
    {
      return _data.interpolate(r, 1);
    }

  }
}

#endif

