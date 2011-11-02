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

#include <Goptical/Math/Vector>
#include <Goptical/Math/Transform>
#include <Goptical/Curve/Composer>

namespace _Goptical {

  namespace Curve {

    Composer::Attributes & Composer::add_curve(const const_ref<Base> &curve)
    {
      Attributes attr;

      attr._curve = curve;
      attr._z_scale = 1.;
      attr._z_offset = 0.;
      attr._transform.reset();
      attr._inv_transform.reset();

      _list.push_back(attr);

      return _list.back();
    }

    Composer::Composer()
      : _list()
    {
    }

    double Composer::sagitta(const Math::Vector2 & xy) const
    {
      double z = 0;

      GOPTICAL_FOREACH(c, _list)
        z += c->_curve->sagitta(c->_inv_transform.transform(xy)) * c->_z_scale + c->_z_offset;

      return z;
    }

    void Composer::derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const
    {
      dxdy.set(0.0);

      GOPTICAL_FOREACH(c, _list)
        {
          Math::Vector2 dtmp;

          c->_curve->derivative(c->_inv_transform.transform(xy), dtmp);

          dxdy += c->_transform.transform_linear(dtmp) * c->_z_scale;
        }
    }

  }
}

