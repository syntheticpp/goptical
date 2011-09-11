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


#ifndef GOPTICAL_LIGHTRAY_HXX_
#define GOPTICAL_LIGHTRAY_HXX_

#include "Goptical/Math/vector_pair.hxx"

namespace _Goptical {

  namespace Light {

    Ray::Ray()
    {
    }

    Ray::Ray(const Math::VectorPair3 &ray,
             double intensity, double wavelen)
      : Math::VectorPair3(ray),
        _wavelen(wavelen),
        _intensity(intensity)
    {
    }

    double Ray::get_intensity() const
    {
      return _intensity;
    }

    void Ray::set_intensity(double intensity)
    {
      _intensity = intensity;
    }

    double Ray::get_wavelen() const
    {
      return _wavelen;
    }

    void Ray::set_wavelen(double wavelen)
    {
      _wavelen = wavelen;
    }

  }
}

#endif

