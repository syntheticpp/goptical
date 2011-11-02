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

#define DPP_DELEGATE_ARGC 5

#include <limits>

#include <Goptical/Math/Vector>

#include <Goptical/Sys/System>
#include <Goptical/Sys/SourcePoint>
#include <Goptical/Sys/Surface>

#include <Goptical/Trace/Ray>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Params>

namespace _Goptical {

  namespace Sys {

    SourcePoint::SourcePoint(SourceInfinityMode m, const Math::Vector3 &pos_dir)
      : Source(m == SourceAtInfinity
         // position of infinity source is only used for Trace::Sequence sort
               ? Math::VectorPair3(pos_dir * -1e9, pos_dir)
               : Math::VectorPair3(pos_dir, Math::vector3_001)),
        _mode(m)
    {
    }

    template <SourceInfinityMode mode>
    void SourcePoint::get_lightrays_(Trace::Result &result,
                                     const Element &target) const
    {
      const Surface *starget = dynamic_cast<const Surface*>(&target);

      if (!starget)
        return;

      double rlen = result.get_params().get_lost_ray_length();

      const Trace::Distribution &d = result.get_params().get_distribution(*starget);

      DPP_DELEGATE5_OBJ(de, void, (const Math::Vector3 &i),

                        // _0
                        const SourcePoint *, this,

                        // _1 ray aiming at target surface origin in source coordinates
                        const Math::VectorPair3 &,
                        Math::VectorPair3(starget->get_position(*this) -
                                            Math::vector3_001 * rlen, Math::vector3_001),

                        // _2 transform from target to source coordinates
                        const Math::Transform<3> &,
                        starget->get_transform_to(*this),

                        // _3
                        const Material::Base *,
                        _mat.valid() ? _mat.ptr() : &get_system()->get_environment_proxy(),

                        // _4
                        Trace::Result &, result,

      {
        Math::Vector3 r = _2.transform(i);  // pattern point on target surface
        Math::Vector3 direction;
        Math::Vector3 position;

        switch (mode)
          {
          case (SourceAtFiniteDistance):
            position = Math::vector3_0;
            direction = r.normalized();
            break;

          case (SourceAtInfinity):
            direction = Math::vector3_001;
            position = _1.pl_ln_intersect(Math::VectorPair3(r, direction));
            break;
          }

        GOPTICAL_FOREACH(l, _0->_spectrum)
          {
            Trace::Ray &r = _4.new_ray();

            // generated rays use source coordinates
            r.direction() = direction;
            r.origin() = position;

            r.set_creator(_0);
            r.set_intensity(l->get_intensity()); // FIXME depends on distance from source and pattern density
            r.set_wavelen(l->get_wavelen());
            r.set_material(_3);
          }
      });

      starget->get_pattern(de, d, result.get_params().get_unobstructed());
    }
        
    void SourcePoint::generate_rays_simple(Trace::Result &result,
                                           const targets_t &entry) const
    {
      GOPTICAL_FOREACH(l, _spectrum)
        result.add_ray_wavelen(l->get_wavelen());

      switch (_mode)
        {
        case SourceAtFiniteDistance:
          GOPTICAL_FOREACH(target, entry)
            get_lightrays_<SourceAtFiniteDistance>(result, **target);
          return;

        case SourceAtInfinity:
          GOPTICAL_FOREACH(target, entry)
            get_lightrays_<SourceAtInfinity>(result, **target);
          return;
        }
    }

    void SourcePoint::generate_rays_intensity(Trace::Result &result,
                                              const targets_t &entry) const
    {
      generate_rays_simple(result, entry);
    }

  }

}

