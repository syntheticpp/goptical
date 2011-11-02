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


#ifndef GOPTICAL_IMAGE_HH_
#define GOPTICAL_IMAGE_HH_

#include "Goptical/common.hh"

#include "Goptical/Sys/surface.hh"
#include "Goptical/Curve/flat.hh"
#include "Goptical/Shape/infinite.hh"

namespace _Goptical {
  
  namespace Sys {

    /**
       @short Image plane optical element
       @header Goptical/Sys/Image
       @module {Core}
       @main
     */
    class Image : public Surface
    {
    public:

      /** Create a new image plane at given position. */
      Image(const Math::VectorPair3 &position,
            const const_ref<Curve::Base> &curve,
            const const_ref<Shape::Base> &shape);

      /** Create a new flat square image plane at given position with given half width */
      Image(const Math::VectorPair3 &position, double radius);

    private:
      void trace_ray_simple(Trace::Result &result, Trace::Ray &incident,
                            const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;
      void trace_ray_intensity(Trace::Result &result, Trace::Ray &incident,
                               const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;
      void trace_ray_polarized(Trace::Result &result, Trace::Ray &incident,
                               const Math::VectorPair3 &local, const Math::VectorPair3 &intersect) const;
    };

  }
}

#endif

