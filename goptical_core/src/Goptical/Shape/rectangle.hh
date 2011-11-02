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


#ifndef GOPTICAL_SHAPE_RECTANGLE_HH_
#define GOPTICAL_SHAPE_RECTANGLE_HH_

#include "Goptical/common.hh"

#include "base.hh"
#include "Goptical/Math/vector.hh"

namespace _Goptical {

  namespace Shape {

    /**
       @short Rectangle and square shape
       @header Goptical/Shape/Rectangle
       @module {Core}
       @main
     */

    class Rectangle : public Base
    {
    public:
      /** Create a rectangle with given width and height */
      inline Rectangle(double width, double height);
      /** Create a square with given side length */
      inline Rectangle(double sqsize);

      /** @override */
      inline double max_radius() const;
      /** @override */
      inline double min_radius() const;
      /** @override */
      double get_outter_radius(const Math::Vector2 &dir) const;
      /** @override */
      Math::VectorPair2 get_bounding_box() const;
      /** @override */
      bool inside(const Math::Vector2 &point) const;
      /** @override */
      void get_pattern(const Math::Vector2::put_delegate_t  &v, const Trace::Distribution &d, bool unobstructed) const;
      /** @override */
      unsigned int get_contour_count() const;
      /** @override */
      void get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const;
      /** @override */
      void get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const;

    private:
      Math::Vector2 get_step(double resolution) const;

      Math::Vector2 _halfsize;
    };

  }

}

#endif

