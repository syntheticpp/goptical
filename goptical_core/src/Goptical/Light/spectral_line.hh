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


#ifndef GOPTICAL_SPECTRALLINE_HH_
#define GOPTICAL_SPECTRALLINE_HH_

#include "Goptical/common.hh"

#include "Goptical/Io/rgb.hh"

namespace _Goptical {

  namespace Light {

    /**
       @short Describe a spectral line
       @header Goptical/Light/SpectralLine
       @module {Core}

       This class can be used to describe a spectral line. It contains
       constants for wave length of standard rays
    */

    class SpectralLine
    {
    public:
      /** Create a spectral line with specified wavelen and
          intensity. */
      SpectralLine(double wavelen, double intensity = 1.0);

      /** get spectral line wavelen */
      inline double get_wavelen() const;
      /** get spectral line intensity */
      inline double get_intensity() const;
      /** get rgb color associated spectral line, ignore intensity */
      inline Io::Rgb get_color() const;
      /** get rgb color associated with wavelen */
      static Io::Rgb get_wavelen_color(double wl);

      /** @showvalue red colored line */
      static const double red = 645.0;
      /** @showvalue green colored line */
      static const double green = 510.0;
      /** @showvalue blue colored line */
      static const double blue = 440.0;

      /** @showvalue infrared mercury line */
      static const double t = 1013.98;
      /** @showvalue infrared cesium line */
      static const double s = 852.11;
      /** @showvalue red helium line */
      static const double r = 706.5188;
      /** @showvalue red hydrogen line */
      static const double C = 656.2725;
      /** @showvalue red cadmium line */
      static const double C_ = 643.8469; // C'
      /** @showvalue yellow sodium line */
      static const double D = 589.2938;
      /** @showvalue yellow helium line */
      static const double d = 587.5618;
      /** @showvalue green mercury line */
      static const double e = 546.074;
      /** @showvalue blue hydrogen line */
      static const double F = 486.1327;
      /** @showvalue blue cadmium line */
      static const double F_ = 479.9914; // F'
      /** @showvalue blue mercury line */
      static const double g = 435.8343;
      /** @showvalue violet mercury line */
      static const double h = 404.6561;
      /** @showvalue ultraviolet mercury line */
      static const double i = 365.0146;

    private:
      double    _wavelen;
      double    _intensity;
    };
  }
}

#endif

