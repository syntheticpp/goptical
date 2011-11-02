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


#ifndef GOPTICAL_CURVE_GRID_HH_
#define GOPTICAL_CURVE_GRID_HH_

#include "Goptical/common.hh"

#include "base.hh"
#include "Goptical/Data/grid.hh"

namespace _Goptical {

  namespace Curve {

    /**
       @short Define surface curve from interpolated grid data set.
       @header Goptical/Curve/Grid
       @module {Core}
       @main

       This class use the @ref Data::Grid 2d data set container to
       define a 3d non-symmetric curve. Several data interpolation
       algorithms are available allowing use of @ref Data::Bicubic spline
       interpolation with or without prescribed derivative/gradient
       data. @see Data::Grid .

       The @ref Spline curve model is preferred when dealing
       with @ref Rotational {rotationally symmetric curves}.
    */
    class Grid : public Base
    {
    public:
      /** Creates a grid curve with n*n sample points defined over the
          given radius. */
      Grid(unsigned int n, double radius);

      ~Grid();

      /** Get embedded sagitta/gradient data container */
      inline const Data::Grid &get_data() const;

      /** Get embedded sagitta/gradient data container */
      inline Data::Grid &get_data();

      /** Set grid values to best fit an other curve. Gradient data
          will be extracted only if the interpolation have been set to
          Data::BicubicDeriv previously */
      void fit(const Base &c);

      double sagitta(const Math::Vector2 & xy) const;
      void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

    protected:
      Data::Grid _data;
    };

  }

}

#endif
