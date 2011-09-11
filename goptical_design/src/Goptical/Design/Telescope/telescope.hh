/*

      This file is part of the Goptical Design library.
  
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


#ifndef GOPTICAL_DESIGN_TELESCOPE_HH_
#define GOPTICAL_DESIGN_TELESCOPE_HH_

#include <Goptical/common.hh>
#include <Goptical/Design/common.hh>

#include <Goptical/Sys/group.hh>

namespace _Goptical {

  namespace Design {

    namespace Telescope {

    /**
       @short Base class for telescope optical designs
       @header Goptical/Design/Telescope/Telescope
       @module {Design}
    */
    class Telescope : public Sys::Group
    {
    public:
      inline Telescope(const Math::VectorPair3 &p);

      /** Set telescope focal length */
      virtual void set_focal(double focal) = 0;
      /** Get telescope focal length */
      virtual double get_focal() const = 0;

      /** Set telescope primary mirror diameter */
      virtual void set_diameter(double diameter) = 0;
      /** Get telescope primary mirror diameter */
      virtual double get_diameter() const = 0;

      /** Set telescope back working distance */
      virtual void set_bwd(double bwd) = 0;
      /** Get telescope back working distance */
      virtual double get_bwd() const = 0;

      /** Set telescope field angle */
      virtual void set_field_angle(double bwd) = 0;
      /** Get telescope field angle */
      virtual double get_field_angle() const = 0;

      /** Get diameter of unvignetted image on focal plane */
      virtual double get_unvignetted_image_diameter() const = 0;

      /** Get telescope focal plane in parent coordinate system */
      virtual Math::VectorPair3 get_focal_plane() const = 0;
    };

    }
  }
}

#endif

