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

#include <Goptical/Sys/System>
#include <Goptical/Sys/Lens>
#include <Goptical/Sys/Group>
#include <Goptical/Sys/Stop>
#include <Goptical/Sys/OpticalSurface>

#include <Goptical/Material/Solid>

#include <Goptical/Shape/Disk>
#include <Goptical/Curve/Sphere>
#include <Goptical/Curve/Flat>

#include <Goptical/Math/Vector>
#include <Goptical/Math/Triangle>

#include <Goptical/Io/Renderer>

#include <Goptical/Trace/Ray>

namespace _Goptical {

  namespace Sys {

    Lens::Lens(const Math::VectorPair3 &p, double offset,
               const const_ref<Material::Base> &env)
      : Group(p),
        _last_pos(offset),
        _surfaces(_surfaces_storage),
        _next_mat(env)
    {
    }

    Lens::Lens(const Math::VectorPair3 &p,
               const const_ref<Curve::Base> &curve0,
               const const_ref<Shape::Base> &shape0,
               const const_ref<Curve::Base> &curve1,
               const const_ref<Shape::Base> &shape1,
               double thickness0,
               const const_ref<Material::Base> &glass0,
               const const_ref<Material::Base> &env)
      : Group(p),
        _last_pos(0),
        _surfaces(_surfaces_storage),
        _next_mat(env)
    {
      _surfaces.reserve(2);
      add_surface(curve0, shape0, thickness0, glass0);
      add_surface(curve1, shape1, 0, env);
    }

    Lens::Lens(const Math::VectorPair3 &p,
               double roc0, double ap_radius0,
               double roc1, double ap_radius1,
               double thickness0,
               const const_ref<Material::Base> &glass0,
               const const_ref<Material::Base> &env)
      : Group(p),
        _last_pos(0),
        _surfaces(_surfaces_storage),
        _next_mat(env)
    {
      _surfaces.reserve(2);
      add_surface(roc0, ap_radius0, thickness0, glass0);
      add_surface(roc1, ap_radius1, 0, env);
    }

    Lens::~Lens()
    {
    }

    unsigned int Lens::add_surface(const const_ref<Curve::Base> &curve,
                                   const const_ref<Shape::Base> &shape,
                                   double thickness,
                                   const const_ref<Material::Base> &glass)
    {
      assert(thickness >= 0.);

      OpticalSurface &s = _surfaces.create(Math::VectorPair3(0, 0, _last_pos),
                                           curve, shape, _next_mat, glass);

      _next_mat = glass;
      _last_pos += thickness;
      Container::add(s);
      return _surfaces.size() - 1;
    }

    unsigned int Lens::add_surface(double roc, double radius,
                                   double thickness,
                                   const const_ref<Material::Base> &glass)
    {
      const_ref<Curve::Base> curve;

      if (roc == 0.)
        curve = Curve::flat;
      else
        curve = ref<Curve::Sphere>::create(roc);

      return add_surface(curve, ref<Shape::Disk>::create(radius), thickness, glass);
    }

    void Lens::add_stop(const const_ref<Shape::Base> &shape, double thickness)
    {
      if (_stop.valid())
        throw Error("Can not add more than one stop per Lens");

      _stop = ref<Stop>::create(Math::VectorPair3(0, 0, _last_pos), shape);

      _last_pos += thickness;
      Container::add(*_stop);
    }

    void Lens::add_stop(double radius, double thickness)
    {
      return add_stop(ref<Shape::Disk>::create(radius), thickness);
    }

    double Lens::get_thickness(unsigned int index) const
    {
      double z = index + 1 == _surfaces.size()
        ? _last_pos : _surfaces.at(index + 1).get_local_position().z();

      return z - _surfaces.at(index).get_local_position().z();
    }

    void Lens::set_thickness(double thickness, unsigned int index)
    {
      double diff = thickness - get_thickness(index);

      for (unsigned int i = index; i < _surfaces.size(); i++)
        {
          Math::Vector3 p = _surfaces[i].get_local_position();
          p.z() += diff;
          _surfaces[i].set_local_position(p);
        }

      _last_pos += diff;
    }

    void Lens::set_left_material(const const_ref<Material::Base> &m)
    {
      _surfaces.front().set_material(0, m);
    }

    void Lens::set_right_material(const const_ref<Material::Base> &m)
    {
      _surfaces.back().set_material(1, m);
    }

    void Lens::set_glass_material(const const_ref<Material::Base> &m,
                                  unsigned int index)
    {
      _surfaces.at(index+1).set_material(0, m);
      _surfaces.at(index).set_material(1, m);
    }

    void Lens::set_left_curve(const const_ref<Curve::Base> &c)
    {
      _surfaces.front().set_curve(c);
    }

    void Lens::set_right_curve(const const_ref<Curve::Base> &c)
    {
      _surfaces.back().set_curve(c);
    }

    void Lens::set_curve(const const_ref<Curve::Base> &c, unsigned int index)
    {
      _surfaces.at(index).set_curve(c);
    }

    void Lens::set_shape(const const_ref<Shape::Base> &s)
    {
      for (unsigned int i = 0; i < _surfaces.size(); i++)
        _surfaces[i].set_shape(s);
    }

    void Lens::set_shape(const const_ref<Shape::Base> &s, unsigned int index)
    {
      _surfaces.at(index).set_shape(s);
    }

    Math::VectorPair3 Lens::get_exit_plane() const
    {
      return get_transform().transform_line(Math::VectorPair3(0., 0., _last_pos));
    }

    void Lens::draw_3d_e(Io::Renderer &r, const Element *ref) const
    {
      // surfaces
      Group::draw_3d_e(r, ref);

#if 0
      // FIXME check / broken code

      // draw lens contours
      for (unsigned int i = 0; i < _surfaces.size() - 1; i++)
        {
          const Surface & left = _surfaces[i];
          const Surface & right = _surfaces[i+1];

          const Shape::Base &s = left.get_shape();
          Io::Rgb color = left.get_color(r);

          if (&s != &right.get_shape())
            continue;

          for (unsigned int i = 0; i < s.get_contour_count(); i++)
            {
              // get 2d contour points

              std::vector<Math::Vector2 > contour;
              delegate_push<typeof(contour)> contour_push(contour);
              s.get_contour(i, contour_push, r.get_feature_size());

              unsigned int j;
              unsigned int count = contour.size();

              if (count < 2)
                continue;

              Math::Vector3 pr[count];
              Math::Vector3 pl[count];

              // compute 3d contour points

              const Math::Transform<3> &rtr = right.get_global_transform();
              const Math::Transform<3> &ltr = left.get_global_transform();

              for (j = 0; j < count; j++)
                {
                  pr[j] = rtr.transform(Math::Vector3(contour[j], right.get_curve().sagitta(contour[j])));
                  pl[j] = ltr.transform(Math::Vector3(contour[j], left.get_curve().sagitta(contour[j])));
                }

              // compute normals and draw

              Math::Vector3 prevn(Math::Triangle<3>(pr[count - 1], pl[count - 1], pl[0]).normal());
              Math::Triangle<3> cur(pr[0], pl[0], pl[1]);
              Math::Vector3 curn(cur.normal());

              for (j = 0; j < count; j++)
                {
                  Math::Triangle<3> next(pr[(j+1) % count], pl[(j+1) % count], pl[(j+2) % count]);
                  Math::Vector3 nextn(next.normal());

                  Math::Vector3 n1((prevn + curn) * 0.5);
                  Math::Vector3 n2((curn + nextn) * 0.5);

                  r.draw_triangle(cur, Math::Triangle<3>(n1, n1, n2), color);
                  r.draw_triangle(Math::Triangle<3>(pl[(j+1) % count], pr[(j+1) % count], pr[j]),
                                  Math::Triangle<3>(n2, n2, n1), color);

                  prevn = curn;
                  cur = next;
                  curn = nextn;
                }
            }
        }
#endif
    }

    void Lens::draw_2d_edge(Io::Renderer &r, const Surface &left, double l_y,
                            const Surface &right, double r_y, LensEdge type,
                            const Element *ref) const
    {
      const Math::Vector3 l3(0., l_y, left.get_curve().sagitta(Math::Vector2(0., l_y)));
      const Math::Vector2 l2(left.get_transform_to(ref).transform(l3).project_zy());

      const Math::Vector3 r3(0., r_y, right.get_curve().sagitta(Math::Vector2(0., r_y)));
      const Math::Vector2 r2(right.get_transform_to(ref).transform(r3).project_zy());

      switch (type)
        {
        case StraightEdge: {
          if (fabs(l2.y() - r2.y()) > 1e-6)
            {
              double m;

              if (fabs(l2.y()) > fabs(r2.y()))
                {
                  m = l2.y();
                  r.draw_segment(Math::VectorPair2(r2.x(), m, r2.x(), r2.y()), left.get_color(r));
                }
              else
                {
                  m = r2.y();
                  r.draw_segment(Math::VectorPair2(l2.x(), m, l2.x(), l2.y()), left.get_color(r));
                }

              r.draw_segment(Math::VectorPair2(l2.x(), m, r2.x(), m), left.get_color(r));

              break;
            }
        }

        case SlopeEdge:
          r.draw_segment(l2, r2, left.get_color(r));
          break;
        }
    }

#define GRP_BEGIN do { if (!grp) { r.group_begin(); grp = true; } } while (0)
#define GRP_END   do { if (grp) { r.group_end(); grp = false; } } while (0)

    void Lens::draw_2d_e(Io::Renderer &r, const Element *ref) const
    {
      bool grp = false;

      if (_stop.valid())
        _stop.staticcast<Element>()->draw_2d_e(r, ref);

      if (_surfaces.empty())
        return;

      const OpticalSurface &first = _surfaces[0];
      if (&first.get_material(1) != &first.get_material(0))
        {
          GRP_BEGIN;
          static_cast<const Element&>(first).draw_2d_e(r, ref);              
        }

      for (unsigned int i = 0; i < _surfaces.size() - 1; i++)
        {
          const OpticalSurface &left = _surfaces[i];
          const OpticalSurface &right = _surfaces[i+1];

          if (!dynamic_cast<const Material::Solid*>(&left.get_material(1)))
            {
              GRP_END;
            }
          else
            {
              // draw outter edges
              double left_top_edge =  left.get_shape().get_outter_radius(Math::vector2_01);
              double left_bot_edge =  -left.get_shape().get_outter_radius(-Math::vector2_01);
              double right_top_edge = right.get_shape().get_outter_radius(Math::vector2_01);
              double right_bot_edge = -right.get_shape().get_outter_radius(-Math::vector2_01);

              draw_2d_edge(r, left, left_top_edge, right, right_top_edge, StraightEdge, ref);
              draw_2d_edge(r, left, left_bot_edge, right, right_bot_edge, StraightEdge, ref);

              // draw hole edges if not coincident
              double left_top_hole =  left.get_shape().get_hole_radius(Math::vector2_01);
              double left_bot_hole =  -left.get_shape().get_hole_radius(-Math::vector2_01);
              double right_top_hole = right.get_shape().get_hole_radius(Math::vector2_01);
              double right_bot_hole = -right.get_shape().get_hole_radius(-Math::vector2_01);

              if (fabs(left_bot_hole - left_top_hole) > 1e-6 ||
                  fabs(right_bot_hole - right_top_hole) > 1e-6)
                {
                  draw_2d_edge(r, left, left_top_hole, right, right_top_hole, SlopeEdge, ref);
                  draw_2d_edge(r, left, left_bot_hole, right, right_bot_hole, SlopeEdge, ref);
                }
            }

          if (&right.get_material(1) != &right.get_material(0))
            {
              GRP_BEGIN;
              static_cast<const Element&>(right).draw_2d_e(r, ref);
            }
        }

      GRP_END;
    }

  }

}
