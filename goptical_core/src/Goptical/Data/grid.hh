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


#ifndef GOPTICAL_DATA_SAMPLEGRID_HH_
#define GOPTICAL_DATA_SAMPLEGRID_HH_

#include <vector>

#include "Goptical/common.hh"

#include "Goptical/Math/vector.hh"
#include "Goptical/Data/set.hh"

namespace _Goptical {

  namespace Data {

    /**
       @short 2d numerical data set with interpolation.
       @header Goptical/Data/Grid
       @module {Core}
       @main

       This class provides a numerical data container where data value
       can be defined for each point of a 2d rectangular grid. The
       grid is defined by grid samples count (n1, n2), a 2d origin
       vector and 2 steps values.

       Severals interpolation algorithms are available to guess values
       between grid defined knots, see @ref Interpolation.
     */
    class Grid : public Set
    {
    public:
      /** Create a Grid object with given sample counts and metrics.
          @param n1 Number of samples along the first dimension axis.
          @param n2 Number of samples along the second dimension axis.
          @param origin Grid origin point
          @param step Step values for each grid dimensions
      */
      Grid(unsigned int n1, unsigned int n2,
           const Math::Vector2 & origin = Math::Vector2(0, 0),
           const Math::Vector2 & step = Math::Vector2(1, 1));

      ~Grid();

      /** Set grid origin 2d vector and step values */
      inline void set_metrics(const Math::Vector2 & origin, const Math::Vector2 & step);
      /** Get origin vector */
      inline const Math::Vector2 & get_origin() const;
      /** Get step values vector */
      inline const Math::Vector2 & get_step() const;

      /** Change grid size by defining new sample counts for each dimensions */
      inline void resize(unsigned int n1, unsigned int n2);

      /** Change all grid points stored values */
      void set_all_y(double y = 0.0);

      /** Change all grid points stored gradient. Only available when
          Data::BicubicDeriv interpolation is selected. */
      void set_all_d(const Math::Vector2 & deriv = Math::Vector2(0, 0));

      /** Get 2d vector on grid of the sample point at index (n1, n2) */
      inline Math::Vector2 get_x_value_i(unsigned int n1, unsigned int n2) const;

      /** Get value stored at sample point index (n1, n2) */
      inline double get_y_value(unsigned int n1, unsigned int n2) const;
      /** Get modifiable reference to value stored at sample point index (n1, n2) */
      inline double & get_y_value(unsigned int n1, unsigned int n2);

      /** Get value stored at nearest sample point from 2d vector on grid */
      inline double get_nearest_y(const Math::Vector2 & v) const;
      /** Get modifiable reference to value stored at nearest sample
          point from 2d vector on grid */
      inline double & get_nearest_y(const Math::Vector2 & v);

      /** Get 1st derivative/gradient vector at sample point index
          (n1, n2). Only available when Data::BicubicDeriv
          interpolation is selected. */
      inline const Math::Vector2 & get_d_value(unsigned int n1, unsigned int n2) const;
      /** Get modifiable reference to 1st derivative/gradient vector
          at sample point index (n1, n2). Only available when
          Data::BicubicDeriv interpolation is selected. */
      inline Math::Vector2 & get_d_value(unsigned int n1, unsigned int n2);

      /** Get 1st derivative/gradient vector stored at nearest sample
          point from 2d vector on grid. Only available when
          Data::BicubicDeriv interpolation is selected. */
      inline const Math::Vector2 & get_nearest_d(const Math::Vector2 & v) const;
      /** Get modifiable reference to 1st derivative/gradient vector
          stored at nearest sample point from 2d vector on grid. Only
          available when Data::BicubicDeriv interpolation is selected. */
      inline Math::Vector2 & get_nearest_d(const Math::Vector2 & v);

      /** Interpolate data at given 2d vector point on grid using
          currently selected interpolation algorithm */
      inline double interpolate(const Math::Vector2 & v) const;

      /** Interpolate gradient at given 2d vector point on grid using
          currently selected interpolation algorithm */
      inline Math::Vector2 interpolate_deriv(const Math::Vector2 & v) const;

      // inherited from Set
      inline unsigned int get_dimensions() const;
      inline unsigned int get_count(unsigned int dimension) const;
      double get_x_value(unsigned int x, unsigned int dimension) const;
      double get_y_value(const unsigned int n[]) const;
      double interpolate(const double x[]) const;
      double interpolate(const double x[], unsigned int deriv, unsigned int dimension) const;
      Math::range_t get_x_range(unsigned int dimension) const;
      void set_interpolation(Interpolation i);

    private:

      struct poly_t
      {
        double p[16];
      };

      void update_nearest(unsigned int x[2], const Math::Vector2 & v) const;
      void update_linear(unsigned int x[2], const Math::Vector2 & v) const;
      void update_bicubic(unsigned int x[2], const Math::Vector2 & v) const;
      void update_bicubic_diff(unsigned int x[2], const Math::Vector2 & v) const;
      void update_bicubic_deriv(unsigned int x[2], const Math::Vector2 & v) const;

      void lookup_nearest(unsigned int x[2], const Math::Vector2 & v) const;
      void lookup_interval(unsigned int x[2], const Math::Vector2 & v) const;

      double interpolate_nearest_y(const unsigned int x[2], const Math::Vector2 & v) const;
      double interpolate_linear_y(const unsigned int x[2], const Math::Vector2 & v) const;
      double interpolate_bicubic_y(const unsigned int x[2], const Math::Vector2 & v) const;

      void interpolate_nearest_d(const unsigned int x[2], Math::Vector2 & d, const Math::Vector2 & v) const;
      void interpolate_linear_d(const unsigned int x[2], Math::Vector2 & d, const Math::Vector2 & v) const;
      void interpolate_bicubic_d(const unsigned int x[2], Math::Vector2 & d, const Math::Vector2 & v) const;

      void resize_y(unsigned int x1, unsigned int x2);
      void resize_yd(unsigned int x1, unsigned int x2);

      inline void invalidate();

      /** get cross derivative from numerical differentiation */
      void get_cross_deriv_diff(double cd[]) const;
      /** get derivative from numerical differentiation */
      void get_deriv_diff(Math::Vector2 d[]) const;
      /** get smooth derivative from continuous 2nd derivative */
      template <int v> void get_deriv_smooth(Math::Vector2 d[], unsigned int w, unsigned int o) const;

      /** set bicubic polynomial coefficients */
      static void set_poly(poly_t &p, const double t[16]);

      unsigned int _size[2];

      std::vector <double> _y_data;
      std::vector <Math::Vector2 > _d_data;
      std::vector <poly_t> _poly;

      void (Grid::*_update)(unsigned int x[2], const Math::Vector2 & v) const;
      void (Grid::*_lookup)(unsigned int x[2], const Math::Vector2 & v) const;
      double (Grid::*_interpolate_y)(const unsigned int x[2], const Math::Vector2 & v) const;
      void (Grid::*_interpolate_d)(const unsigned int x[2], Math::Vector2 & d, const Math::Vector2 & v) const;
      void (Grid::*_resize)(unsigned int x1, unsigned int x2);

      Math::Vector2 _origin;
      Math::Vector2 _step;
    };

  }
}


#endif

