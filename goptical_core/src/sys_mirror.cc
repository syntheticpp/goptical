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

#include <Goptical/Shape/Disk>
#include <Goptical/Curve/Conic>
#include <Goptical/Curve/Flat>

#include <Goptical/Sys/Mirror>
#include <Goptical/Sys/OpticalSurface>

namespace _Goptical {

  namespace Sys {

    Mirror::Mirror(const Math::VectorPair3 &p,
                   const const_ref<Curve::Base> &curve,
                   const const_ref<Shape::Base> &shape,
                   bool light_from_left,
                   const const_ref<Material::Base> &metal,
                   const const_ref<Material::Base> &air)
      : OpticalSurface(p, curve, shape,
                       light_from_left ? air : metal,
                       light_from_left ? metal : air)
    {
    }

    Mirror::Mirror(const Math::VectorPair3 &p,
                   const const_ref<Curve::Base> &curve,
                   double ap_radius,
                   bool light_from_left,
                   const const_ref<Material::Base> &metal,
                   const const_ref<Material::Base> &air)
      : OpticalSurface(p, curve, ap_radius,
                       light_from_left ? air : metal,
                       light_from_left ? metal : air)
    {
    }

    Mirror::Mirror(const Math::VectorPair3 &p,
                   double roc, double sc,
                   double ap_radius,
                   bool light_from_left,
                   const const_ref<Material::Base> &metal,
                   const const_ref<Material::Base> &air)
      : OpticalSurface(p, roc == 0. ? const_ref<Curve::Base>(Curve::flat)
                                    : const_ref<Curve::Base>(ref<Curve::Conic>::create(roc, sc)),
                       ref<Shape::Disk>::create(ap_radius),
                       light_from_left ? air : metal,
                       light_from_left ? metal : air)
    {
    }

  }

}

