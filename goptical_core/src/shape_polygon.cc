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

#include <cassert>
#include <limits>

#include <Goptical/Shape/Polygon>

#include <Goptical/Trace/Distribution>
#include <Goptical/Math/Triangle>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Shape {

    Polygon::Polygon()
      : _updated(false),
        _vertices(),
        _bbox(Math::vector2_pair_00),
        _max_radius(0),
        _min_radius(1e100)
    {
    }

    inline void Polygon::update()
    {
      size_t s = _vertices.size();
      if (s < 3)
        throw Error("Polygon shape has less than 3 vertices");

      _max_radius = 0;
      _min_radius = std::numeric_limits<double>::max();
      _bbox = Math::VectorPair2(std::numeric_limits<double>::max(),
                                  -std::numeric_limits<double>::max());

      const Math::Vector2 *prev = &_vertices[s - 1];
      const Math::Vector2 *cur;

      for (unsigned int i = 0; i < s; i++)
        {
          cur = &_vertices[i];

          // update max radius
          double max = cur->len();
          if (max > _max_radius)
            _max_radius = max;

          // update min radius
          double min = Math::VectorPair2(*cur, *prev).seg_pt_clst_pt(Math::vector2_0).len();
          if (min < _min_radius)
            _min_radius = min;

          // udate bounding box
          for (unsigned int i = 0; i < 2; i++)
            {
              if ((*cur)[i] < _bbox[0][i])
                _bbox[0][i] = (*cur)[i];
              else if ((*cur)[i] > _bbox[1][i])
                _bbox[1][i] = (*cur)[i];
            }

          prev = cur;
        }
    }

    void Polygon::insert_vertex(const Math::Vector2 &v, unsigned int id)
    {
      _updated = false;
      assert(id <= _vertices.size());
      _vertices.insert(_vertices.begin() + id, v);
    }

    unsigned int Polygon::add_vertex(const Math::Vector2 &v)
    {
      _updated = false;
      unsigned int pos = _vertices.size();
      insert_vertex(v, pos);
      return pos;
    }

    void Polygon::delete_vertex(unsigned int id)
    {
      _updated = false;
      assert(id < _vertices.size());
      _vertices.erase(_vertices.begin() + id);
    }

    bool Polygon::inside(const Math::Vector2 &p) const
    {
      unsigned int s = _vertices.size();

      if (s < 3)
        return false;

      unsigned int count = 0;
      const Math::Vector2 *w = &_vertices[s - 1];

      // FIXME optimize
      for (unsigned int i = 0; i < s; i++)
        {
          const Math::Vector2 *v = &_vertices[i];

          // Algorithm from http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/
          if ((((v->y() <= p.y()) && (p.y() < w->y())) || ((w->y() <= p.y()) && (p.y() < v->y()))) &&
              (p.x() < (w->x() - v->x()) * (p.y() - v->y()) / (w->y() - v->y()) + v->x()))
            count++;
          w = v;
        }

      return (count & 1) != 0;
    }

    void Polygon::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
    {
      GOPTICAL_FOREACH(v, _vertices)
        f(*v);
    }

    void Polygon::get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const
    {
      const Math::Vector2 &z = _vertices[0];

      for (unsigned int i = 1; i < _vertices.size() - 1; i++)
        {
          const Math::Vector2 &a = _vertices[i];
          const Math::Vector2 &b = _vertices[i + 1];

          f(Math::Triangle<2>(z, a, b));
        }
    }

    double Polygon::max_radius() const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();
      return _max_radius;
    }

    double Polygon::min_radius() const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();
      return _min_radius;
    }

    Math::VectorPair2 Polygon::get_bounding_box() const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();
      return _bbox;
    }

    double Polygon::get_outter_radius(const Math::Vector2 &dir) const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();

      double r = 0;
      unsigned int s = _vertices.size();

      const Math::VectorPair2 d(Math::vector2_0, dir * _max_radius * 2.);
      const Math::Vector2 *w = &_vertices[s - 1];

      // FIXME optimize
      for (unsigned int i = 0; i < s; i++)
        {
          const Math::Vector2 *v = &_vertices[i];

          try {
            double a = Math::VectorPair2(*w, *v).seg_intersect_seg<false, false>(d).len();
            r = std::max(r, a);
          } catch (...) {
          }

          w = v;
        }

      return r;
    }

  }

}
