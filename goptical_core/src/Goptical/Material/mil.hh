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
  
      Copyright (C) 2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/


#ifndef GOPTICAL_MATERIAL_MIL_HH_
#define GOPTICAL_MATERIAL_MIL_HH_

#include "Goptical/common.hh"

#include "Goptical/Material/abbe.hh"

namespace _Goptical {

  namespace Material {

    /**
       @short Mil glass code material model
       @header Goptical/Material/Mil
       @module {Core}
       @main

       This class is able to extract @em nd index and @em vd Abbe
       number values from glass code in order to model mil glass
       material. Glass code format is described in "@em{MIL-G-174B}"
       standard.

       @see Abbe
     */

    class Mil : public AbbeVd
    {
    public:

      /** Create a glass model based on glass code with "@em{nnnnvvv}" format. */
      inline Mil(unsigned int code);
    };

  }
}

#endif

