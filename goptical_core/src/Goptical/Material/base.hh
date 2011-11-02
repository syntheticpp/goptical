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


#ifndef GOPTICAL_MATERIAL_BASE_HH_
#define GOPTICAL_MATERIAL_BASE_HH_

#include "Goptical/common.hh"

namespace _Goptical {

  namespace Material {

    /**
       @short Optical material base class.
       @header Goptical/Material/Base
       @module {Core}
       @main

       This class is the base class for all material classes.  It
       desrcibes physical properties of materials which interact with
       light.
     */

    class Base : public ref_base<Base>
    {
    public:
      /** Material base constuctor. Set material temperature to 20. */
      Base();

      virtual ~Base();

      /** Return true if material must be considered opaque for ray
          tracing */
      virtual bool is_opaque() const = 0;

      /** Return true if material may reflect most of the light and
          must be considered as a mirror when ignoring ray intensity
          computation during ray tracing. */
      virtual bool is_reflecting() const = 0;

      /** Get material internal transmittance for thickness in
          mm. Subclasses _must_ provide this function or the
          get_extinction_coef() function. */
      virtual double get_internal_transmittance(double wavelen, double thickness) const;

      /** Get material internal transmittance for 1mm thickness. */
      inline double get_internal_transmittance(double wavelen) const;

      /** Get material absolute refractive index at specified wavelen in @em nm. */
      virtual double get_refractive_index(double wavelen) const = 0;

      /** Get material relative refractive index in given medium at specified wavelen in @em nm. */
      inline double get_refractive_index(double wavelen, const Base &env) const;

      /** Get extinction coefficient. Subclasses _must_ provide this
          function or the get_internal_transmittance() function. */
      virtual double get_extinction_coef(double wavelen) const;

      /** Get reflectance at normal incidence */
      virtual double get_normal_reflectance(const Base *from, double wavelen) const;

      /** Get transmittance at normal incidence */
      virtual double get_normal_transmittance(const Base *from, double wavelen) const;

      /** Set material temperature in celcius */
      void set_temperature(double temp);
      /** Get material temperature in celcius */
      virtual double get_temperature() const;

      /** Get material color and alpha */
      virtual Io::Rgb get_color() const;

    protected:
      double            _temperature; // celcius
    };

    /** Material null pointer */
    static const const_ref<Base> none;

  }

}

#endif

