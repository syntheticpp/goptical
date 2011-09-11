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


#ifndef GOPTICAL_CURVE_SPLINE_HH_
#define GOPTICAL_CURVE_SPLINE_HH_

#include "Goptical/common.hh"

#include "Goptical/Curve/rotational.hh"
#include "Goptical/Data/discrete_set.hh"

namespace _Goptical {

  namespace Curve {

    /**
       @short Rotationally symmetric spline curve implementation
       @header Goptical/Curve/Spline
       @module {Core}
       @main

       This class uses a 1d discrete data set to define a rotationally
       symmetric sagitta/gradient curve. Several data interpolations
       algorithm are available allowing use of Smooth Cubic spline
       interpolation with or without prescribed derivative/gradient
       data.

       @see Data::DiscreteSet
    */
    class Spline : public Rotational
    {
    public:
      /** Create an empty spline curve with no defined point */
      Spline();
      ~Spline();

      /** Get sagitta/derivative data container */
      inline const Data::DiscreteSet & get_data() const;

      /** get sagitta/derivative data container */
      inline Data::DiscreteSet & get_data();

      /** Clear all points and fit to an other rotationally symmetric curve.
          @param c Curve to fit
          @param radius Maximum radius where curve is defined
          @param points Number of sample points
      */
      void fit(const Rotational &c, double radius, unsigned int points);

      inline double sagitta(double r) const;
      inline double derivative(double r) const;

    protected:
      Data::DiscreteSet _data;
    };

  }
}

#endif

