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


#include <Goptical/Sys/Surface>
#include <Goptical/Sys/Element>
#include <Goptical/Material/Base>

#include <Goptical/Shape/Base>
#include <Goptical/Shape/Ring>

#include <Goptical/Curve/Base>

#include <Goptical/Math/Vector>
#include <Goptical/Math/Triangle>

#include <Goptical/Light/Ray>

#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Ray>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Params>

#include <Goptical/Io/Renderer>
#include <Goptical/Io/Rgb>


namespace _Goptical {

  namespace Sys {

    Surface::Surface(const Math::VectorPair3 &p,
                     const const_ref<Curve::Base> &curve,
                     const const_ref<Shape::Base> &shape)
      : Element(p),
        _discard_intensity(0),
        _curve(curve),
        _shape(shape)
    {
    }

    Surface::~Surface()
    {
    }

    void Surface::get_pattern(const Math::Vector3::put_delegate_t &f,
                              const Trace::Distribution &d,
                              bool unobstructed) const
    {
        DPP_DELEGATE2_OBJ(de, void, (const Math::Vector2 &v2d),
                          const Math::Vector3::put_delegate_t &, f, // _0
                          const const_ref<Curve::Base> &, _curve,    // _1
        {
          _0(Math::Vector3(v2d, _1->sagitta(v2d)));
        });

      // get distribution from shape
      _shape->get_pattern(de, d, unobstructed);
    }

    void Surface::trace_ray_simple(Trace::Result &result, Trace::Ray &incident,
                                   const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const
    {
      throw Error("simple ray trace not handled by this surface class");
    }

    void Surface::trace_ray_intensity(Trace::Result &result, Trace::Ray &incident,
                                      const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const
    {
      throw Error("intensity ray trace not handled by this surface class");
    }

    void Surface::trace_ray_polarized(Trace::Result &result, Trace::Ray &incident,
                                      const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const
    {
      throw Error("polarized ray trace not handled by this surface class");
    }

    bool Surface::intersect(const Trace::Params &params, Math::VectorPair3 &pt, const Math::VectorPair3 &ray) const
    {
      if (!_curve->intersect(pt.origin(), ray))
        return false;

      if (!params.get_unobstructed() &&
          !_shape->inside(pt.origin().project_xy()))
        return false;

      _curve->normal(pt.normal(), pt.origin());
      if (ray.direction().z() < 0)
        pt.normal() = -pt.normal();

      return true;
    }

    template <Trace::IntensityMode m>
    void Surface::trace_ray(Trace::Result &result, Trace::Ray &incident,
                            const Math::VectorPair3 &local, const Math::VectorPair3 &pt) const
    {
      incident.set_len((pt.origin() - local.origin()).len());
      incident.set_intercept(*this, pt.origin());

      if (m == Trace::SimpleTrace)
        {
          incident.set_intercept_intensity(1.0);
          return trace_ray_simple(result, incident, local, pt);
        }
      else
        {
          // apply absorbtion from current material
          double i_intensity = incident.get_intensity() *
            incident.get_material()->get_internal_transmittance(
                          incident.get_wavelen(), incident.get_len());

          incident.set_intercept_intensity(i_intensity);

          if (i_intensity < _discard_intensity)
            return;

          if (m == Trace::IntensityTrace)
            return trace_ray_intensity(result, incident, local, pt);
          else if (m == Trace::PolarizedTrace)
            return trace_ray_polarized(result, incident, local, pt);
          else
            abort();
        }
    }

    template void Surface::trace_ray<Trace::SimpleTrace>(Trace::Result &result, Trace::Ray &incident,
                                                         const Math::VectorPair3 &local, const Math::VectorPair3 &pt) const;
    template void Surface::trace_ray<Trace::IntensityTrace>(Trace::Result &result, Trace::Ray &incident,
                                                         const Math::VectorPair3 &local, const Math::VectorPair3 &pt) const;
    template void Surface::trace_ray<Trace::PolarizedTrace>(Trace::Result &result, Trace::Ray &incident,
                                                         const Math::VectorPair3 &local, const Math::VectorPair3 &pt) const;

    template <Trace::IntensityMode m>
    inline void Surface::process_rays_(Trace::Result &result,
                                       Trace::rays_queue_t *input) const
    {
      const Trace::Params &params = result.get_params();

      GOPTICAL_FOREACH(i, *input)
        {
          Math::VectorPair3 pt;
          Trace::Ray  &ray = **i;

          const Math::Transform<3> &t = ray.get_creator()->get_transform_to(*this);
          Math::VectorPair3 local(t.transform_line(ray));

          if (intersect(params, pt, local))
            {
              result.add_intercepted(*this, ray);

              trace_ray<m>(result, ray, local, pt);
            }
        }
    }

    void Surface::process_rays_simple(Trace::Result &result,
                                      Trace::rays_queue_t *input) const
    {
      process_rays_<Trace::SimpleTrace>(result, input);
    }

    void Surface::process_rays_intensity(Trace::Result &result,
                                         Trace::rays_queue_t *input) const
    {
      process_rays_<Trace::IntensityTrace>(result, input);
    }

    void Surface::process_rays_polarized(Trace::Result &result,
                                         Trace::rays_queue_t *input) const
    {
      process_rays_<Trace::PolarizedTrace>(result, input);
    }

    Io::Rgb Surface::get_color(const Io::Renderer &r) const
    {
      return r.get_style_color(Io::StyleSurface);
    }

    Math::VectorPair3 Surface::get_bounding_box() const
    {
      Math::VectorPair2 sb = _shape->get_bounding_box();

      // FIXME we assume curve is symetric here
      double z = 0;
      double ms = _curve->sagitta(Math::Vector2(_shape->max_radius()));

      if (z > ms)
        std::swap(z, ms);

      return Math::VectorPair3(Math::Vector3(sb[0].x(), sb[0].y(), z),
                                 Math::Vector3(sb[1].x(), sb[1].y(), ms));
    }

    void Surface::draw_3d_e(Io::Renderer &r, const Element *ref) const
    {
      Io::Rgb color = get_color(r);

      std::vector<Math::Triangle<2> >   mesh;
      delegate_push<typeof(mesh)>       mesh_push(mesh);
      _shape->get_triangles(mesh_push, r.get_feature_size());

      const Math::Transform<3> &tr = get_global_transform();

      GOPTICAL_FOREACH(t, mesh)
        {
          Math::Triangle<3> pts, nrm;

          for (unsigned int i = 0; i < 3; i++)
            {
              pts[i].x() = (*t)[i].x();
              pts[i].y() = (*t)[i].y();
              pts[i].z() = _curve->sagitta((*t)[i]);

              _curve->normal(nrm[i], pts[i]);

              pts[i] = tr.transform(pts[i]);
              nrm[i] = tr.transform_linear(nrm[i]);
            }

          r.draw_triangle(pts, nrm, color);
        }
    }

    void Surface::get_2d_points(Math::Vector2 *array,
                                unsigned int count,
                                double start, double end,
                                const Element *ref) const
    {
      assert(count > 1);

      double    y1 = start;
      double    step = (end - start) / (count - 1);
      int i;

      const Math::Transform<3> &t = get_transform_to(ref);

      for (i = 0; i < (int)count; i++)
        {
          Math::Vector3 v(0., y1, 0.);
          v.z() = _curve->sagitta(v.project_xy());

          array[i] = t.transform(v).project_zy();
          y1 += step;
        }
    }

    void Surface::draw_2d_e(Io::Renderer &r, const Element *ref) const
    {
      double top_edge = _shape->get_outter_radius(Math::vector2_01);
      double top_hole = _shape->get_hole_radius(Math::vector2_01);

      double bot_edge = -_shape->get_outter_radius(-Math::vector2_01);
      double bot_hole = -_shape->get_hole_radius(-Math::vector2_01);

      unsigned int res = std::max(100, std::min(4, (int)(fabs(top_edge - bot_edge) / r.get_feature_size())));

      if (fabs(bot_hole - top_hole) > 1e-6)
        {
          DPP_VLARRAY(Math::Vector2, res / 2, p);

          get_2d_points(&p[0], res / 2, top_edge, top_hole, ref);
          r.draw_polygon(&p[0], res / 2, get_color(r), false, false);
          get_2d_points(&p[0], res / 2, bot_hole, bot_edge, ref);
          r.draw_polygon(&p[0], res / 2, get_color(r), false, false);
        }
      else
        {
          DPP_VLARRAY(Math::Vector2, res, p);

          get_2d_points(&p[0], res, top_edge, bot_edge, ref);
          r.draw_polygon(&p[0], res, get_color(r), false, false);
        }
    }

  }

}



