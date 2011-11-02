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
#include <Goptical/Curve/Array>

namespace _Goptical {

  namespace Curve {

    Array::Array(const const_ref<Base> &curve, double pitch, enum pattern_e p)
      : _curve(curve),
        _pitch(pitch)
    {
      switch (p)
        {
        case Square:
          _transform = &Array::transform_square;
          break;

        case SquareCenter:
          _transform = &Array::transform_square_center;
          break;

        case Hexagonal:
          _transform = &Array::transform_hexagonal;
          break;
        }
    }

    static inline double mod(double x, double n)
    {
      x = x - trunc(x / n) * n;
      return x < 0 ? x + n : x;
    }

    Math::Vector2 Array::transform_square(const Math::Vector2 &v) const
    {
      double h = _pitch / 2.0;

      return Math::Vector2(mod(v.x(), _pitch) - h,
                             mod(v.y(), _pitch) - h);
    }

    Math::Vector2 Array::transform_square_center(const Math::Vector2 &v) const
    {
      double h = _pitch / 2.0;

      return Math::Vector2(mod(v.x() - h, _pitch) - h,
                             mod(v.y() - h, _pitch) - h);
    }

    Math::Vector2 Array::transform_hexagonal(const Math::Vector2 &v) const
    {
      double pitch = _pitch / 2.0;
      double h1 = pitch * 0.86602540378443864676;
      double h2 = pitch * 1.73205080756887729352;
      double h3 = pitch * 1.5;
      double x, y = mod(v.y(), h3 * 2.0);

      if (y > h3)
        {
          y -= h3;
          x = mod(v.x() - h1, h2);
        }
      else
        {
          x = mod(v.x(), h2);
        }

      if (y > pitch && (h3 - y) * h1 < fabs((h1 - x) * pitch / 2.0))
        {
          y -= h3;
          x = x > h1 ? x - h1 : x + h1;
        }

      return Math::Vector2(x - h1, y - pitch / 2.0);
    }

    double Array::sagitta(const Math::Vector2 & xy) const
    {
      return _curve->sagitta((this->*_transform)(xy));
    }

    void Array::derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const
    {
      _curve->derivative((this->*_transform)(xy), dxdy);
    }

  }

}

