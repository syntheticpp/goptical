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


#include <Goptical/Curve/Parabola>
#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Curve {

    Parabola::Parabola(double roc)
      : ConicBase(roc, -1.0)
    {
    }

    double Parabola::sagitta(double r) const
    {
      return Math::square(r) / (2.0 * _roc);
    }

    double Parabola::derivative(double r) const
    {
      return r / _roc;
    }

    bool Parabola::intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const
    {
      const double      ax = ray.origin().x();
      const double      ay = ray.origin().y();
      const double      az = ray.origin().z();
      const double      bx = ray.direction().x();
      const double      by = ray.direction().y();
      const double      bz = ray.direction().z();

      /*
        find intersection point between conical section and line,
        Telescope optics, page 266
      */
      double a = (Math::square(by) + Math::square(bx));
      double b = ((by * ay + bx * ax) / _roc - bz) * 2.0;
      double c = (Math::square(ay) + Math::square(ax)) / _roc - 2.0 * az;

      double t;

      if (a == 0)
        {
          t = -c / b;
        }
      else
        {
          double d = Math::square(b) - 4.0 * a * c / _roc;

          if (d < 0)
            return false;               // no intersection

          double s = sqrt(d);

          if (a * bz < 0)
            s = -s;

          t = (2 * c) / (s - b);
        }

      if (t <= 0)               // ignore intersection if before ray origin
        return false;

      point = ray.origin() + ray.direction() * t;

      return true;
    }

  }

}

