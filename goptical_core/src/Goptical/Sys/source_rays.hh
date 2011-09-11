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


#ifndef GOPTICAL_SOURCE_RAYS_HH_
#define GOPTICAL_SOURCE_RAYS_HH_

#include <map>

#include "Goptical/common.hh"

#include "Goptical/Sys/source.hh"
#include "Goptical/Math/vector_pair.hh"
#include "Goptical/Light/ray.hh"

namespace _Goptical {

  namespace Sys {

      /**
         @short Custom rays light source
         @header Goptical/Sys/SourceRays
         @module {Core}
         @main

         This class implement a light source which generates user
         defined rays.

         Default wavelen list contains a single 550nm entry.
      */

    class SourceRays : public Source
    {
    public:
      /** Create a rays source. Source object position is used by some
          of the @tt add_* functions and may be specified. */
      SourceRays(const Math::Vector3 &object = Math::vector3_0);

      /** Add chief rays to system entrance pupil for all defined wavelengths. */
      void add_chief_rays(const Sys::System &sys);
      /** Add chief rays to specified surface for all defined wavelengths. */
      void add_chief_rays(const Sys::Surface &s);

      /** Add marginal rays to system entrance pupil for all defined
          wavelengths. @see __add_marginal_rays2__ */
      void add_marginal_rays(const Sys::System &sys, double entrance_height = 0.);

      /** Add marginal rays to specified surface for all defined wavelengths.
       @pb Source direction (rotation) is used to determine tangential plane.
       @pb Entrance height is set to surface aperture radius when 0.
       @alias add_marginal_rays2 */
      void add_marginal_rays(const Sys::Surface &s, double entrance_height = 0.);

      /** Creates light rays from position and direction for all
          defined wavelengths. @pb Ray origin and direction are
          expressed in specified @tt ref element coordinates or global
          coordinates if none is specified. */
      void add_rays(const Math::VectorPair3 &ray, const Element *ref = 0);

      /** Add a single light ray; rays properties are copied from
          specified light ray. @pb Ray origin and direction are
          expressed in specified @tt ref element coordinates or global
          coordinates if none is specified.*/
      void add_ray(const Light::Ray &ray, const Element *ref = 0);

      /** Discard all defined rays  */
      void clear_rays();

    private:

      void generate_rays_simple(Trace::Result &result,
                                const targets_t &entry) const;

      void generate_rays_intensity(Trace::Result &result,
                                   const targets_t &entry) const;

      void wavelen_ref_inc(double wl);
      void wavelen_ref_drop(double wl);

      /* storage for first 32 rays */
      vector_pool<Light::Ray, 32>::block_type _rays_storage;
      /* rays allocation pool */
      vector_pool<Light::Ray, 32>       _rays;
      /* wavelen use count map */
      typedef std::map<double, size_t> wl_map_t;
      wl_map_t          _wl_map;
    };

  }
}

#endif

