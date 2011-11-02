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

#include <Goptical/Sys/SourceRays>
#include <Goptical/Sys/Surface>
#include <Goptical/Sys/System>

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>

#include <Goptical/Trace/Ray>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Params>

namespace _Goptical {

  namespace Sys {

    SourceRays::SourceRays(const Math::Vector3 &object)
      : Source(object),
        _rays(_rays_storage)
    {
    }

    void SourceRays::wavelen_ref_inc(double wl)
    {
      wl_map_t::iterator i = _wl_map.insert(
        wl_map_t::value_type(wl, 0)).first;
      i->second++;
    }

    void SourceRays::wavelen_ref_drop(double wl)
    {
      wl_map_t::iterator i = _wl_map.find(wl);
      assert(i != _wl_map.end());
      if (!--i->second)
        _wl_map.erase(i);
    }

    void SourceRays::add_chief_rays(const Sys::System &sys)
    {
      add_chief_rays(sys.get_entrance_pupil());
    }

    void SourceRays::add_chief_rays(const Sys::Surface &s)
    {
      const Math::Vector3 so = s.get_position(*this);

      // ray in source coordinates
      const Math::VectorPair3 r(Math::vector3_0, so.normalized());

      GOPTICAL_FOREACH(l, _spectrum)
        _rays.create(r, l->get_intensity(), l->get_wavelen());
    }

    void SourceRays::add_marginal_rays(const Sys::System &sys, double entrance_height)
    {
      add_marginal_rays(sys.get_entrance_pupil(), entrance_height);
    }

    void SourceRays::add_marginal_rays(const Sys::Surface &s, double entrance_height)
    {
      const Math::Transform<3> &t1 = s.get_transform_to(*this);
      const Math::Transform<3> &t2 = get_transform_to(s);

      // get source up vector direction in surface coordinates
      const Math::Vector2 up = t2.transform_linear(Math::vector3_010).project_xy().normalized();

      // get surface edge point in that direction
      if (entrance_height == 0.)
        entrance_height = s.get_shape().get_outter_radius(up);

      const Math::Vector2 edge2 = up * (entrance_height - 1e-8);
      // account for surface curvature to get z value
      const Math::Vector3 edge3(edge2, s.get_curve().sagitta(edge2));

      // get source point projection on surface axis
      const Math::Vector3 on_axis(0., 0., t2.transform(Math::vector3_0).z());

      // ray in source coordinates
      const Math::VectorPair3 r(t1.transform_line(Math::VectorPair3(on_axis, (edge3 - on_axis).normalized())));

      GOPTICAL_FOREACH(l, _spectrum)
        _rays.create(r, l->get_intensity(), l->get_wavelen());
    }

    void SourceRays::add_rays(const Math::VectorPair3 &ray, const Element *ref)
    {
      GOPTICAL_FOREACH(l, _spectrum)
        {
          Light::Ray r(ray, l->get_intensity(), l->get_wavelen());
          add_ray(r, ref);
        }
    }

    void SourceRays::add_ray(const Light::Ray &ray, const Element *ref)
    {
      Light::Ray &r = _rays.create(ray);
      wavelen_ref_inc(ray.get_wavelen());

      r = ray;

      if (ref != this)
        {
          if (ref)
            r = ref->get_transform_to(*this).transform_line(ray);
          else
            r = ref->get_local_transform().transform_line(ray);
        }
    }

    void SourceRays::clear_rays()
    {
      _rays.clear();
      _wl_map.clear();
    }

    void SourceRays::generate_rays_simple(Trace::Result &result,
                                          const targets_t &entry) const
    {
      const Material::Base *m = _mat.valid()
        ? _mat.ptr() : &get_system()->get_environment_proxy();

      GOPTICAL_FOREACH(w, _wl_map)
        if (w->second)
          result.add_ray_wavelen(w->first);

      GOPTICAL_FOREACH(lr, _rays)
        {
          Trace::Ray &r = result.new_ray(*lr);

          r.set_creator(this);
          r.set_material(m);
        }
    }

    void SourceRays::generate_rays_intensity(Trace::Result &result,
                                             const targets_t &entry) const
    {
      generate_rays_simple(result, entry);      
    }

  }

}

