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

#ifndef GOPTICAL_DESIGN_TELESCOPE_CASSEGRAIN_HXX_
#define GOPTICAL_DESIGN_TELESCOPE_CASSEGRAIN_HXX_

#include <Goptical/Sys/mirror.hxx>
#include <Goptical/Shape/disk.hxx>
#include <Goptical/Shape/ring.hxx>
#include <Goptical/Curve/conic.hxx>

#include "telescope.hxx"

namespace _Goptical {

  namespace Design {

    namespace Telescope {

      template <CassegrainType type>
      double Cassegrain<type>::get_focal() const
      {
        return _f1;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_diameter() const
      {
        return _d1;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_bwd() const
      {
        return _bwd;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_field_angle() const
      {
        return _fa;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_secondary_diameter() const
      {
        return _d2;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_magnification() const
      {
        return _mag;
      }

      template <CassegrainType type>
      const Sys::Mirror & Cassegrain<type>::get_primary() const
      {
        return _primary;
      }

      template <CassegrainType type>
      Sys::Mirror & Cassegrain<type>::get_primary()
      {
        return _primary;
      }

      template <CassegrainType type>
      const Sys::Mirror & Cassegrain<type>::get_secondary() const
      {
        return _secondary;
      }

      template <CassegrainType type>
      Sys::Mirror & Cassegrain<type>::get_secondary()
      {
        return _secondary;
      }

      template <CassegrainType type>
      Math::VectorPair3 Cassegrain<type>::get_focal_plane() const
      {
        return _focal_plane;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_unvignetted_image_diameter() const
      {
        return _id;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_mirrors_separation() const
      {
        return _ms;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_primary_conic() const
      {
        return _k1;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_secondary_conic() const
      {
        return _k2;
      }

    }
  }
}

#endif

