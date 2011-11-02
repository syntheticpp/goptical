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


#ifndef GOPTICAL_SOURCE_HXX_
#define GOPTICAL_SOURCE_HXX_

#include <algorithm>

#include "Goptical/Light/spectral_line.hxx"
#include "Goptical/Sys/element.hxx"
#include "Goptical/Sys/surface.hxx"

#include "Goptical/Material/base.hxx"

namespace _Goptical {

  namespace Sys {

    void Source::set_material(const const_ref<Material::Base> &m)
    {
      _mat = m;
    }

    void Source::clear_spectrum()
    {
      _spectrum.clear();
      _max_intensity = _min_intensity = 0.0;
    }

    void Source::single_spectral_line(const Light::SpectralLine & l)
    {
      _spectrum.clear();
      _spectrum.push_back(l);
    }

    void Source::add_spectral_line(const Light::SpectralLine & l)
    {
      _spectrum.push_back(l);
      _max_intensity = std::max(_max_intensity, l.get_intensity());
      _min_intensity = std::min(_min_intensity, l.get_intensity());
    }

    void Source::set_spectral_line(const Light::SpectralLine & l, int index)
    {
      _spectrum[index] = l;
      refresh_intensity_limits();
    }

    double Source::get_max_intensity() const
    {
      return _max_intensity;
    }

    double Source::get_min_intensity() const
    {
      return _min_intensity;
    }

    template <Trace::IntensityMode m>
    void Source::generate_rays(Trace::Result &result,
                               const targets_t &entry) const
    {
      switch (m)
        {
        case Trace::SimpleTrace:
          return generate_rays_simple(result, entry);
        case Trace::IntensityTrace:
          return generate_rays_intensity(result, entry);
        case Trace::PolarizedTrace:
          return generate_rays_polarized(result, entry);
        }
    }

  }
}

#endif

