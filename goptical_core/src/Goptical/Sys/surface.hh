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


#ifndef GOPTICAL_SURFACE_HH_
#define GOPTICAL_SURFACE_HH_

#include <iostream>

#include "Goptical/common.hh"

#include "Goptical/Sys/element.hh"
#include "Goptical/Shape/base.hh"
#include "Goptical/Curve/base.hh"

namespace _Goptical {

  namespace Sys {

    /**
       @short Base class for surfaces
       @header Goptical/Sys/Surface
       @module {Core}

       This is the base class for surface element. A surface element
       is defined by its position, surface curve and contour shape.

       This class can handle incoming rays and is responsible for
       finding rays intersection with the surface.

       Depending on ray trace intensity mode, it will potentially
       adjust light absorption and then call @ref trace_ray_simple,
       @ref trace_ray_intensity or @ref trace_ray_polarized. These
       functions must be reimplemented in sub classes, default
       implementations will throw.
    */

    class Surface : public Element
    {
    public:
      /** Create a surface element at given location. */
      Surface(const Math::VectorPair3 &p,
              const const_ref<Curve::Base> &curve,
              const const_ref<Shape::Base> &shape);

      virtual ~Surface();

      /** Set surface curve */
      inline void set_curve(const const_ref<Curve::Base> &c);
      /** Get surface curve */
      inline const Curve::Base & get_curve() const;

      /** Set surface shape */
      inline void set_shape(const const_ref<Shape::Base> &s);
      /** Get surface shape */
      inline const Shape::Base & get_shape() const;

      /** Get intersection point and normal to surface at
          intersection. Return false if no intersection occured.
      */
      virtual bool intersect(const Trace::Params &params,
                             Math::VectorPair3 &pt,
                             const Math::VectorPair3 &ray) const;

      /** Get distribution pattern points projected on the surface */
      void get_pattern(const Math::Vector3::put_delegate_t &f,
                       const Trace::Distribution &d,
                       bool unobstructed = false) const;

      /** Trace a single ray through the surface */
      template <Trace::IntensityMode m>
      void trace_ray(Trace::Result &result, Trace::Ray &incident,
                     const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;

      /** Get surface apparent color */
      virtual Io::Rgb get_color(const Io::Renderer &r) const;

      /** Set minimal ray intensity. Incident rays with less intensity
          will be discarded */
      inline void set_discard_intensity(double intensity);
      /** Get minimal ray intensity. */
      inline double get_discard_intensity() const;

      Math::VectorPair3 get_bounding_box() const;

    protected:

      /** This function must be reimplemented by subclasses to handle
          incoming rays and generate new ones when in simple ray trace mode. */
      virtual void trace_ray_simple(Trace::Result &result, Trace::Ray &incident,
                                    const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;

      /** This function must be reimplemented by subclasses to handle
          incoming rays and generate new ones when in intensity ray trace mode. */
      virtual void trace_ray_intensity(Trace::Result &result, Trace::Ray &incident,
                                       const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;

      /** This function must be reimplemented by subclasses to handle
          incoming rays and generate new ones when in polarized ray trace mode. */
      virtual void trace_ray_polarized(Trace::Result &result, Trace::Ray &incident,
                                       const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;

      /** @override */
      void draw_2d_e(Io::Renderer &r, const Element *ref) const;
      /** @override */
      void draw_3d_e(Io::Renderer &r, const Element *ref) const;

    private:

      /** @internal */
      void get_2d_points(Math::Vector2 *array,
                         unsigned int count, double start,
                         double end, const Element *ref) const;

      template <Trace::IntensityMode m>
      inline void process_rays_(Trace::Result &result,
                               Trace::rays_queue_t *input) const;

      virtual void process_rays_simple(Trace::Result &result,
                                       Trace::rays_queue_t *input) const;

      virtual void process_rays_intensity(Trace::Result &result,
                                          Trace::rays_queue_t *input) const;

      virtual void process_rays_polarized(Trace::Result &result,
                                          Trace::rays_queue_t *input) const;

      double                    _discard_intensity;
      const_ref<Curve::Base>   _curve;
      const_ref<Shape::Base>   _shape;
    };

  }
}

#endif

