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


#ifndef GOPTICAL_SHAPE_POLYGON_HH_
#define GOPTICAL_SHAPE_POLYGON_HH_

#include "Goptical/common.hh"

#include "base.hh"
#include "Goptical/Math/vector_pair.hh"

namespace _Goptical {

  namespace Shape {

    /**
       @short Polygon shape
       @header Goptical/Shape/Polygon
       @module {Core}
       @main

       This class implements the polygon shape.

       Triangle tessellation required for proper 3d display only works
       with convex polygons yet.

       @see RegularPolygon
     */

    class Polygon : public Base
    {
    public:
      /** Create a polygon with given radius and edge count */
      Polygon();

      unsigned int add_vertex(const Math::Vector2 &v);
      void insert_vertex(const Math::Vector2 &v, unsigned int id);
      void delete_vertex(unsigned int id);

      inline unsigned int get_vertices_count() const;
      inline const Math::Vector2 & get_vertex(unsigned int id);

    private:

      /** @override */
      double max_radius() const;
      /** @override */
      double min_radius() const;
      /** @override */
      double get_outter_radius(const Math::Vector2 &dir) const;
      /** @override */
      Math::VectorPair2 get_bounding_box() const;
      /** @override */
      bool inside(const Math::Vector2 &point) const;
      /** @override */
      inline unsigned int get_contour_count() const;
      /** @override */
      void get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const;
      /** @override */
      void get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const;

      /** update _min_radius and bounding box */
      void update();

      typedef std::vector<Math::Vector2 > vertices_t;

      bool _updated;
      vertices_t _vertices;
      Math::VectorPair2 _bbox;
      double _max_radius;
      double _min_radius;
    };

  }

}

#endif

