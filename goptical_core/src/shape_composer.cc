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

#include <limits>

#include <Goptical/Math/VectorPair>
#include <Goptical/Math/Transform>
#include <Goptical/Math/Triangle>
#include <Goptical/Shape/Composer>

namespace _Goptical {

  namespace Shape {

    Composer::Composer()
      : _list(),
        _update(false),
        _global_dist(true),
        _max_radius(0.0),
        _min_radius(std::numeric_limits<double>::max()),
        _bbox(Math::vector2_pair_00),
        _contour_cnt(0)
    {
    }

    Composer::Attributes::Attributes(const const_ref<Shape> &shape)
      : _shape(shape),
        _list()
    {
      _transform.reset();
      _inv_transform.reset();
    }

    Composer::Attributes & Composer::add_shape(const const_ref<Shape> &shape)
    {
      _list.push_back(Attributes(shape));
      _update = true;
      return _list.back();
    }

    Composer::Attributes & Composer::Attributes::include(const const_ref<Shape> &shape)
    {
      _list.push_back(Attributes(shape));
      _list.back()._exclude = false;
      return _list.back();
    }

    Composer::Attributes & Composer::Attributes::exclude(const const_ref<Shape> &shape)
    {
      _list.push_back(Attributes(shape));
      _list.back()._exclude = true;
      return _list.back();
    }

    bool Composer::Attributes::inside(const Math::Vector2 &point) const
    {
      Math::Vector2 tp(_inv_transform.transform(point));

      bool res = _shape->inside(tp);

      GOPTICAL_FOREACH(s, _list)
        res &= s->inside(tp);

      return res ^ _exclude;
    }

    bool Composer::inside(const Math::Vector2 &point) const
    {
      GOPTICAL_FOREACH(s, _list)
        if (s->inside(point))
          return true;

      return false;
    }

    void Composer::update()
    {
      Math::Vector2 a(0);
      Math::Vector2 b(0);

      _contour_cnt = 0;

      GOPTICAL_FOREACH(s, _list)
        {
          // update max radius

          double m = s->_transform.transform(Math::vector2_0).len() + s->_shape->max_radius();

          if (m > _max_radius)
            _max_radius = m;
          if (m < _min_radius)
            _min_radius = m;

          // update bounding box

          Math::VectorPair2 bi = s->_transform.transform_pair(s->_shape->get_bounding_box());

          for (unsigned int j = 0; j < 2; j++)
            {
              if (bi[0][j] > bi[1][j])
                std::swap(bi[0][j], bi[1][j]);

              if (bi[0][j] < a[j])
                a[j] = bi[0][j];

              if (bi[1][j] > b[j])
                b[j] = bi[1][j];
            }

          // update contour count
          _contour_cnt += s->_shape->get_contour_count();
        }

      _bbox = Math::VectorPair2(a, b);
      _update = false;
    }

    void Composer::update() const
    {
      const_cast<Composer*>(this)->update();
    }

    double Composer::max_radius() const
    {
      if (_update)
        update();

      return _max_radius;
    }

    double Composer::min_radius() const
    {
      if (_update)
        update();

      return _min_radius;
    }

    double Composer::get_outter_radius(const Math::Vector2 &dir) const
    {
      // FIXME
      return _max_radius;
    }

    double Composer::get_hole_radius(const Math::Vector2 &dir) const
    {
      // FIXME
      return 0;
    }

    Math::VectorPair2 Composer::get_bounding_box() const
    {
      if (_update)
        update();

      return _bbox;
    }

    void Composer::Attributes::pattern_callback(const Math::Vector2 &v, const Math::Vector2::put_delegate_t &f) const
    {
      Math::Vector2 p = _transform.transform(v);

      GOPTICAL_FOREACH(s, _list)
        if (!s->_exclude ^ s->_shape->inside(s->_inv_transform.transform(p)))
          return;

      f(p);
    }

    void Composer::get_pattern(const Math::Vector2::put_delegate_t &f,
                               const Trace::Distribution &d,
                               bool unobstructed) const
    {
      if (_global_dist)
        {
          Shape::get_pattern(f, d, unobstructed);
        }
      else
        {
          GOPTICAL_FOREACH(s, _list)
            {
              delegate_member<const Attributes,
                              void (const Math::Vector2 &, const Math::Vector2::put_delegate_t &) const,
                              &Attributes::pattern_callback> dm(*s);

              delegate_bind<typeof(dm), void (const Math::Vector2 &), arg2nd> db(dm, f);

              s->_shape->get_pattern(db, d, unobstructed);
            }
        }
    }

    unsigned int Composer::get_contour_count() const
    {
      if (_update)
        update();

      return _contour_cnt;
    }

    void Composer::Attributes::contour_callback(const Math::Vector2 &v, const Math::Vector2::put_delegate_t &f) const
    {
      f(_transform.transform(v));
    }

    void Composer::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
    {
      // FIXME add contour boolean operations

      GOPTICAL_FOREACH(s, _list)
        {
          unsigned int c = s->_shape->get_contour_count();

          if (contour < c)
            {
              delegate_member<const Attributes,
                              void (const Math::Vector2 &, const Math::Vector2::put_delegate_t &) const,
                              &Attributes::contour_callback> dm(*s);

              delegate_bind<typeof(dm), void (const Math::Vector2 &), arg2nd> db(dm, f);

              return s->_shape->get_contour(contour, db, resolution);
            }

          contour -= c;
        }
    }

    void Composer::Attributes::triangle_callback(const Math::Triangle<2> &t, const Math::Triangle<2>::put_delegate_t  &f) const
    {
      Math::Triangle<2> p;

      for (unsigned int i = 0; i < 3; i++)
        p[i] = _transform.transform(t[i]);

      f(p);
    }

    void Composer::get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const
    {
      GOPTICAL_FOREACH(s, _list)
        {
          delegate_member<const Attributes,
                          void (const Math::Triangle<2> &, const Math::Triangle<2>::put_delegate_t &) const,
                          &Attributes::triangle_callback> dm(*s);

          delegate_bind<typeof(dm), void (const Math::Triangle<2> &), arg2nd> db(dm, f);

          s->_shape->get_triangles(db, resolution);
        }
    }

  }
}

