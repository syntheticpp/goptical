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


#ifndef GOPTICAL_CURVE_COMPOSER_HH_
#define GOPTICAL_CURVE_COMPOSER_HH_

#include "Goptical/common.hh"

#include "Goptical/Math/transform.hh"
#include "base.hh"

namespace _Goptical {

  namespace Curve {

    /**
       @short Enable definition of curve as composition of other curves
       @header Goptical/Curve/Composer
       @module {Core}
       @main

       This class allows definition of a surface curve using the sum
       of several base curves. Each base curve involved can be
       translated, rotated and scaled individually.
    */
    class Composer : public Base
    {
    public:

      /**
         @short Curve::Composer base curve attributes.
         @header Goptical/Curve/Composer

         This class enables access to Composer base curve
         transformations. Sagitta (z) scaling and (x, y) affine
         transforms (translation, rotation, scaling) can be applied as
         needed in any order.
       */
      class Attributes
      {
        friend class Composer;

      public:
        /** Apply sagitta (z) offset. default is 0 */
        inline Attributes & z_offset(double zoffset);
        /** Apply sagitta (z) scale factor. default is 1 */
        inline Attributes & z_scale(double zfactor);
        /** Apply scaling affine transform using scale factors (xscale, yscale) */
        inline Attributes & xy_scale(const Math::Vector2 &factor);
        /** Apply rotation affine transform. Angle is in degree. */
        inline Attributes & rotate(double dangle);
        /** Apply translation transform */
        inline Attributes & xy_translate(const Math::Vector2 &offset);

      private:
        const_ref<Base> _curve;
        Math::Transform<2> _transform;
        Math::Transform<2> _inv_transform;

        double _z_scale;
        double _z_offset;
      };

      Composer();

      /** Add a base curve to use for composition. The returned
          Composer::Attributes object may be used to apply base curve
          transformations. */
      Attributes & add_curve(const const_ref<Base> &curve);

      double sagitta(const Math::Vector2 & xy) const;
      void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

    private:
      std::list <Attributes> _list;
    };

  }
}

#endif

