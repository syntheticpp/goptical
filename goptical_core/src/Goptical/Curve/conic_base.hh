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


#ifndef GOPTICAL_CURVE_CONIC_BASE_HH_
#define GOPTICAL_CURVE_CONIC_BASE_HH_

#include "Goptical/common.hh"

#include "Goptical/Curve/rotational.hh"
#include "curve_roc.hh"

namespace _Goptical {

  namespace Curve {

    /**
       @short Base class for conic family of curves
       @header Goptical/Curve/ConicBase
       @module {Core}

       This base class defines common properties of rotationally
       symmetric conic curves. These curves are all defined by a
       radius of curvature and deformation coefficient.

       Fitting can be used to find best fit conic of an other
       rotationally symmetric curve either with fixed or free
       deformation parameter.
     */

    class ConicBase : public Rotational, public CurveRoc
    {
    public:
      /** get Schwarzschild constant */
      inline double get_schwarzschild() const;

      /** get eccentricity */
      inline double get_eccentricity() const;

      /** Adjust radius of curvature to best fit given
          curve. Deformation Won't be changed by this function. See
          Conic::fit() to adjust deformation too.

          @param curve Curve to fit
          @param radius Maximum radius used to get sample points
          @param count Number of sample points to use
      */
      double fit_roc(const Rotational &c, double radius, unsigned int count);

      virtual bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const = 0;
      virtual double sagitta(double r) const = 0;
      virtual double derivative(double r) const = 0;

    protected:
      inline ConicBase(double roc, double sc);

      double _sh;       // Schwarzschild constant + 1
    };
  }

}

#endif

