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

#ifndef GOPTICAL_DESIGN_TELESCOPE_NEWTON_HH_
#define GOPTICAL_DESIGN_TELESCOPE_NEWTON_HH_

#include <Goptical/common.hh>

#include <Goptical/Sys/mirror.hh>
#include <Goptical/Shape/disk.hh>
#include <Goptical/Shape/ellipse.hh>
#include <Goptical/Curve/flat.hh>
#include <Goptical/Curve/conic.hh>

#include "telescope.hh"

namespace _Goptical {

  namespace Design {

    namespace Telescope {

      /**
         @short Newton telescope optical design
         @header Goptical/Design/Telescope/Newton
         @module {Design}

         This class is a group of optical elements forming a Newton
         telescope. Optical elements are adjusted as needed from
         prescribed telescope parameters.

         @xsee {tuto_newton}
      */
      class Newton : public Telescope
      {
      public:
        Newton(const Math::VectorPair3 &p, double focal, double diameter,
               double bwd = 100, double field_angle = 1.);

        /** Get telescope primary mirror component */
        inline Sys::Mirror & get_primary();
        /** Get telescope primary mirror component */
        inline const Sys::Mirror & get_primary() const;

        /** Get telescope secondary mirror component */
        inline Sys::Mirror & get_secondary();
        /** Get telescope secondary mirror component */
        inline const Sys::Mirror & get_secondary() const;

        /** Get secondary mirror offset */
        inline double get_secondary_offset() const;

        /** Get secondary mirror minor axis length */
        inline double get_secondary_minor_axis() const;
        /** Set telescope secondary mirror diameter */
        void set_secondary_minor_axis(double diameter);

        /** Get secondary mirror major axis length */
        inline double get_secondary_major_axis() const;

        /** @override */
        void set_focal(double focal);
        /** @override */
        inline double get_focal() const;

        /** @override */
        void set_diameter(double diameter);
        /** @override */
        inline double get_diameter() const;

        /** @override */
        void set_bwd(double bwd);
        /** @override */
        inline double get_bwd() const;

        /** @override */
        void set_field_angle(double fa);
        /** @override */
        inline double get_field_angle() const;

        /** @override */
        inline double get_unvignetted_image_diameter() const;

        /** @override */
        Math::VectorPair3 get_focal_plane() const;

      private:

        void update();

        double calc_field_angle();
        double calc_secondary();
        double calc_unvignetted_image_size();

        double _focal;
        double _diameter;
        double _bwd;
        double _field_angle;
        double _unvignetted_image_size;
        double _offset;
        double _minor_axis;
        double _major_axis;

        Shape::Disk     _primary_shape;
        Curve::Conic    _primary_curve;
        Sys::Mirror     _primary;
        Shape::Ellipse  _secondary_shape;
        Sys::Mirror     _secondary;
        Math::VectorPair3       _focal_plane;
      };

    }
  }
}

#endif

