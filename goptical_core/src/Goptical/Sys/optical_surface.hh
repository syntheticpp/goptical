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


#ifndef GOPTICAL_OPTICALSURFACE_HH_
#define GOPTICAL_OPTICALSURFACE_HH_

#include "Goptical/common.hh"

#include "Goptical/Material/air.hh"
#include "Goptical/Shape/infinite.hh"
#include "Goptical/Curve/flat.hh"
#include "Goptical/Sys/surface.hh"

namespace _Goptical {

  namespace Sys {

    /**
       @short Optical surface element
       @header Goptical/Sys/OpticalSurface
       @module {Core}
       @main

       This class implements optical surfaces.

       Optical surfaces are defined by material on both side of the
       surface, curve and contour shape. This class handle light rays
       refraction and reflection.

       If @ref Material::none is passed as material, it will be
       replaced by system environement @ref Material::Proxy
       {proxy} material when the optical surface becomes part of a
       @ref System.
    */

    class OpticalSurface : public Surface
    {
    public:

      /** Create an optical surface at specified location. */
      OpticalSurface(const Math::VectorPair3 &p,
                     const const_ref<Curve::Base> &curve,
                     const const_ref<Shape::Base> &shape,
                     const const_ref<Material::Base> &left,
                     const const_ref<Material::Base> &right);

      /** Create a circular aperture optical surface at specified
          location.
          @param ap_radius circular aperture radius.
      */
      OpticalSurface(const Math::VectorPair3 &p,
                     const const_ref<Curve::Base> &curve, double ap_radius,
                     const const_ref<Material::Base> &left,
                     const const_ref<Material::Base> &right);

      /** Create a spherical optical surface with circular aperture at
          specified location.
          @param roc spherical radius of curvature. 0 means flat curve.
          @param ap_radius circular aperture radius.
      */
      OpticalSurface(const Math::VectorPair3 &p,
                     double roc, double ap_radius,
                     const const_ref<Material::Base> &left,
                     const const_ref<Material::Base> &right);

      virtual ~OpticalSurface();

      /** Set surface left or right material */
      void set_material(unsigned index, const const_ref<Material::Base> &m);

      /** Get surface left or right material */
      inline const Material::Base & get_material(unsigned id) const;

      /** Get surface natural color from material properties. */
      Io::Rgb get_color(const Io::Renderer &r) const;

    private:

      void trace_ray_simple(Trace::Result &result, Trace::Ray &incident,
                            const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;

      void trace_ray_intensity(Trace::Result &result, Trace::Ray &incident,
                               const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;

      /** @override */
      void system_register(System &s);

      /** @override */
      void system_unregister();

      /** @override */      
      void print(std::ostream &o) const;

      /** compute refracted ray direction according to fresnel law */
      bool refract(const Math::VectorPair3 &ray,
                   Math::Vector3 &direction,
                   const Math::Vector3 &normal,
                   double refract_index) const;

      /** compute reflected ray direction according to fresnel law */
      void reflect(const Math::VectorPair3 &ray,
                   Math::Vector3 &direction,
                   const Math::Vector3 &normal) const;

      const_ref<Material::Base>     _mat[2];
    };

  }
}

#endif

