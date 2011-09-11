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

#ifndef GOPTICAL_DESIGN_TELESCOPE_CASSEGRAIN_HH_
#define GOPTICAL_DESIGN_TELESCOPE_CASSEGRAIN_HH_

#include <Goptical/common.hh>

#include <Goptical/Sys/mirror.hh>
#include <Goptical/Shape/disk.hh>
#include <Goptical/Shape/ring.hh>
#include <Goptical/Curve/conic.hh>

#include "telescope.hh"

namespace _Goptical {

  namespace Design {

    namespace Telescope {

    /**
       @short Cassegrain telescopes model
       @header Goptical/Design/Telescope/Cassegrain
       @module {Design}

       This class is a group of optical elements forming a Cassegrain
       telescope. Optical elements are adjusted as needed from
       prescribed telescope parameters.

       The @ref CassegrainType enum specifies type of Cassegrain
       telescope.
    */
      template <CassegrainType type>
      class Cassegrain : public Telescope
      {
      public:
        Cassegrain(const Math::VectorPair3 &p, double focal,
                   double diameter, double magnification = 3.,
                   double bwd = 100, double field_angle = 1.);

        /** Set telescope secondary mirror diameter */
        void set_secondary_diameter(double d2);
        /** Get telescope secondary mirror diameter */
        inline double get_secondary_diameter() const;

        /** Set telescope secondary mirror magnification */
        void set_magnification(double m);
        /** Get telescope secondary mirror magnification */
        inline double get_magnification() const;

        /** Get telescope primary mirror component */
        inline const Sys::Mirror & get_primary() const;
        /** Get telescope primary mirror component */
        inline Sys::Mirror & get_primary();

        /** Get telescope secondary mirror component */
        inline const Sys::Mirror & get_secondary() const;
        /** Get telescope secondary mirror component */
        inline Sys::Mirror & get_secondary();

        /** Get primary mirror schwarzschild constant */
        inline double get_primary_conic() const;
        /** Get secondary mirror schwarzschild constant */
        inline double get_secondary_conic() const;

        /** Get distance between mirrors */
        inline double get_mirrors_separation() const;

        /** @override */
        void set_focal(double f);
        /** @override */
        inline double get_focal() const;

        /** @override */
        void set_diameter(double d1);
        /** @override */
        inline double get_diameter() const;

        /** @override */
        void set_bwd(double e);
        /** @override */
        inline double get_bwd() const;

        /** @override */
        void set_field_angle(double fa);
        /** @override */
        inline double get_field_angle() const;

        /** @override */
        inline double get_unvignetted_image_diameter() const;

        /** @override */
        inline Math::VectorPair3 get_focal_plane() const;

      private:

        double calc();
        void update();

        double _f;              // cassegrain focal
        double _f1;             // primary focal
        double _d1;             // primary diameter
        double _bwd;            // back working distance
        double _fa;             // field angle
        double _f2;             // secondary focal
        double _d2;             // secondary diameter
        double _mag;            // secondary magnification
        double _ms;             // mirrors separation
        double _k1;             // primary conic
        double _k2;             // secondary conic
        double _hd;             // primary hole diameter
        double _id;

        Shape::Ring     _primary_shape;
        Curve::Conic    _primary_curve;
        Sys::Mirror     _primary;

        Shape::Disk     _secondary_shape;
        Curve::Conic    _secondary_curve;
        Sys::Mirror     _secondary;

        Math::VectorPair3       _focal_plane;
      };

    }
  }
}

#endif

