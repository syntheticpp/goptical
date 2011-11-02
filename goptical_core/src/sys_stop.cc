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

#include <Goptical/Sys/Stop>

#include <Goptical/Trace/Result>
#include <Goptical/Trace/Params>
#include <Goptical/Trace/Ray>

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>

#include <Goptical/Shape/Base>
#include <Goptical/Shape/Disk>
#include <Goptical/Curve/Base>
#include <Goptical/Curve/Flat>

#include <Goptical/Io/Renderer>

namespace _Goptical {

  namespace Sys {

    Stop::Stop(const Math::VectorPair3 &p, const const_ref<Shape::Base> &shape)
      : Surface(p, Curve::flat, shape),
        _intercept_reemit(false)
    {
      _external_radius = shape->max_radius() * 2.0;
    }

    Stop::Stop(const Math::VectorPair3 &p, double r)
      : Surface(p, Curve::flat, ref<Shape::Disk>::create(r)),
        _intercept_reemit(false)
    {
      _external_radius = r * 2.0;
    }

    bool Stop::intersect(const Trace::Params &params,
                         Math::VectorPair3 &intersect,
                         const Math::VectorPair3 &ray) const
    {
      if (!get_curve().intersect(intersect.origin(), ray))
        return false;

      Math::Vector2 v(intersect.origin().project_xy());

      if (v.len() > _external_radius)
        return false;

      bool ir = _intercept_reemit || params.is_sequential();

      if (!ir && get_shape().inside(v))
        return false;

      get_curve().normal(intersect.normal(), intersect.origin());
      if (ray.direction().z() < 0)
        intersect.normal() = -intersect.normal();

      return true;
    }

    inline void Stop::trace_ray_simple(Trace::Result &result, Trace::Ray &incident,
                                       const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const
    {
      Math::Vector2 v = intersect.origin().project_xy();

      bool ir = _intercept_reemit || result.get_params().is_sequential();

      if (ir && get_shape().inside(v))
        {
          // reemit incident ray
          Trace::Ray &r = result.new_ray();

          r.set_wavelen(incident.get_wavelen());
          r.set_intensity(incident.get_intensity());
          r.set_material(incident.get_material());
          r.origin() = intersect.origin();
          r.direction() = incident.direction();
          r.set_creator(this);

          incident.add_generated(&r);
        }
    }

    void Stop::trace_ray_intensity(Trace::Result &result, Trace::Ray &incident,
                                   const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const
    {
      trace_ray_simple(result, incident, local, intersect);
    }

    template <Trace::IntensityMode m>
    inline void Stop::process_rays_(Trace::Result &result,
                                    Trace::rays_queue_t *input) const
    {
      GOPTICAL_FOREACH(i, *input)
        {
          Math::VectorPair3 intersect;
          Trace::Ray  &ray = **i;

          const Math::Transform<3> &t = ray.get_creator()->get_transform_to(*this);
          Math::VectorPair3 local(t.transform_line(ray));

          if (get_curve().intersect(intersect.origin(), local))
            {
              if (intersect.origin().project_xy().len() < _external_radius)
                {
                  get_curve().normal(intersect.normal(), intersect.origin());

                  if (local.direction().z() < 0)
                    intersect.normal() = -intersect.normal();

                  result.add_intercepted(*this, ray);

                  trace_ray<m>(result, ray, local, intersect);
                }
            }
        }
    }

    void Stop::process_rays_simple(Trace::Result &result,
                                   Trace::rays_queue_t *input) const
    {
      process_rays_<Trace::SimpleTrace>(result, input);
    }

    void Stop::process_rays_intensity(Trace::Result &result,
                                      Trace::rays_queue_t *input) const
    {
      process_rays_<Trace::IntensityTrace>(result, input);
    }

    void Stop::process_rays_polarized(Trace::Result &result,
                                      Trace::rays_queue_t *input) const
    {
      process_rays_<Trace::PolarizedTrace>(result, input);
    }

    void Stop::draw_3d_e(Io::Renderer &r, const Element *ref) const
    {
      r.group_begin();
      std::vector<Math::Vector3 > poly;

      DPP_DELEGATE2_OBJ(d, void, (const Math::Vector2 &t),
                        std::vector<Math::Vector3 > &, poly, // _0
                        const Math::Transform<3> &, get_transform_to(ref), // _1
      {
        _0.push_back(_1.transform(Math::Vector3(t, 0.)));
      });

      for (unsigned int i = 0; i < get_shape().get_contour_count(); i++)
        {
          get_shape().get_contour(i, d, r.get_feature_size());
          r.draw_polygon(&poly[0], poly.size(), get_color(r));
          poly.clear();
        }

      r.group_end();
    }

    void Stop::draw_2d_e(Io::Renderer &r, const Element *ref) const
    {
      Math::Vector3 mr(0, _external_radius, 0);
      Math::Vector3 top(0, get_shape().get_outter_radius(Math::vector2_01), 0);
      Math::Vector3 bot(0, -get_shape().get_outter_radius(-Math::vector2_01), 0);

      const Math::Transform<3> t(get_transform_to(ref));

      r.group_begin();
      r.draw_segment(t.transform(top), t.transform(mr), get_color(r));
      r.draw_segment(t.transform(bot), t.transform(-mr), get_color(r));
      r.group_end();
    }

  }

}

