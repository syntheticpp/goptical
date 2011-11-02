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


#ifndef GOPTICAL_SOURCE_HH_
#define GOPTICAL_SOURCE_HH_

#include "Goptical/common.hh"

#include "Goptical/Light/spectral_line.hh"
#include "Goptical/Sys/element.hh"
#include "Goptical/Sys/surface.hh"

#include "Goptical/Material/base.hh"

namespace _Goptical {

  namespace Sys {

    /**
       @short Base class for light sources
       @header Goptical/Sys/Source
       @module {Core}

       This class is the base class for all light sources
       implementations.
     */
    class Source : public Element
    {
    public:
      typedef std::vector<const Sys::Element *> targets_t;

      /** Create a source at given position. */
      Source(const Math::VectorPair3 &position);

      /** Set material where light rays are generated. system
          environment material is used by default. */
      inline void set_material(const const_ref<Material::Base> &m);

      /** Add a new wavelen for ray generation */
      inline void add_spectral_line(const Light::SpectralLine & l);

      /** Set a wavelen in list for ray generation */
      inline void set_spectral_line(const Light::SpectralLine & l,
                                    int index = 0);

      /** Clear ray wavelen list and set a single wavelen */
      inline void single_spectral_line(const Light::SpectralLine & l);

      /** Clear wavelen list */
      inline void clear_spectrum();

      /** Get maximal spectral line intensity */
      inline double get_max_intensity() const;

      /** Get minimal spectral line intensity */
      inline double get_min_intensity() const;

      /** Generate light rays from source */
      template <Trace::IntensityMode m>
      inline void generate_rays(Trace::Result &result,
                                const targets_t &entry) const;

    protected:

      /** This function generate light rays from source. Target entry
          surfaces may be used depending on source model. It must be
          reimplemented by subclasses. */
      virtual void generate_rays_simple(Trace::Result &result,
                                        const targets_t &entry) const;

      /** This function process incoming light rays. It must be
          reimplemented by subclasses if the element can interact with
          light in intensity raytrace mode. */
      virtual void generate_rays_intensity(Trace::Result &result,
                                           const targets_t &entry) const;

      /** This function process incoming light rays. It must be
          reimplemented by subclasses if the element can interact with
          light in polarized raytrace mode. */
      virtual void generate_rays_polarized(Trace::Result &result,
                                           const targets_t &entry) const;

      void refresh_intensity_limits();

      std::vector<Light::SpectralLine>  _spectrum;
      double                            _min_intensity, _max_intensity;
      const_ref<Material::Base>     _mat;
    };
  }
}

#endif

