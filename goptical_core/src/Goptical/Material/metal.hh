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


#ifndef GOPTICAL_MATERIAL_METAL_HH_
#define GOPTICAL_MATERIAL_METAL_HH_

#include "Goptical/common.hh"

#include "Goptical/Data/discrete_set.hh"
#include "Goptical/Material/solid.hh"

namespace _Goptical {

  namespace Material {

    /**
       @short Metal optical material model.
       @header Goptical/Material/Metal
       @module {Core}
       @main

       This class models optical properties of metal
       materials. Extinction and refractive index are stored as
       interpolated data sets.
     */

    class Metal : public Solid
    {
    public:
      Metal();

      bool is_opaque() const;
      bool is_reflecting() const;

      double get_absolute_refractive_index(double wavelen) const;
      double get_refractive_index(double wavelen) const;
      double get_extinction_coef(double wavelen) const;

      /** Get refractive index dataset object */
      inline Data::DiscreteSet & get_refractive_index_dataset();
      /** Get extinction dataset object */
      inline Data::DiscreteSet & get_extinction_coef_dataset();

    protected:
      Data::DiscreteSet _extinction;
      Data::DiscreteSet _refractive_index;
    };

  }
}

#endif

