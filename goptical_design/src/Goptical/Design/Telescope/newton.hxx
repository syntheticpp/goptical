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

#ifndef GOPTICAL_DESIGN_TELESCOPE_NEWTON_HXX_
#define GOPTICAL_DESIGN_TELESCOPE_NEWTON_HXX_

#include <algorithm>

#include <Goptical/Sys/mirror.hxx>
#include <Goptical/Shape/disk.hxx>
#include <Goptical/Shape/ellipse.hxx>
#include <Goptical/Curve/flat.hxx>
#include <Goptical/Curve/conic.hxx>

#include "telescope.hxx"

namespace _Goptical {

  namespace Design {

    namespace Telescope {

      double Newton::get_focal() const
      {
        return _focal;
      }

      double Newton::get_diameter() const
      {
        return _diameter;
      }

      double Newton::get_bwd() const
      {
        return _bwd;
      }

      double Newton::get_field_angle() const
      {
        return _field_angle;
      }

      Sys::Mirror & Newton::get_primary()
      {
        return _primary;
      }

      const Sys::Mirror & Newton::get_primary() const
      {
        return _primary;
      }

      Sys::Mirror & Newton::get_secondary()
      {
        return _secondary;
      }

      const Sys::Mirror & Newton::get_secondary() const
      {
        return _secondary;
      }

      double Newton::get_secondary_offset() const
      {
        return _offset;
      }

      double Newton::get_secondary_minor_axis() const
      {
        return _minor_axis;
      }

      double Newton::get_secondary_major_axis() const
      {
        return _major_axis;
      }

      double Newton::get_unvignetted_image_diameter() const
      {
        return _unvignetted_image_size;
      }

    }
  }
}

#endif

