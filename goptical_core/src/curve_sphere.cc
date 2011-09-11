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


#include <Goptical/Curve/Sphere>
#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Curve {

    Sphere::Sphere(double roc)
      : ConicBase(roc, 0.0)
    {
    }

    double Sphere::sagitta(double r) const
    {
      double    x = fabs(_roc) - sqrt(Math::square(_roc) - Math::square(r));

      return _roc < 0 ? -x : x;
    }

    double Sphere::derivative(double r) const
    {
      return r / sqrt(Math::square(_roc) - Math::square(r));
    }

    /*

    ligne AB A + t * B
    sphere passant par C(Cx, 0, 0), rayon R

    d = Ax - R - Cx
    (Bz*t+Az)^2+(By*t+Ay)^2+(Bx*t+d)^2=R^2

    t=-(sqrt((Bz^2+By^2+Bx^2)*R^2+(-Bz^2-By^2)*d^2+(2*Az*Bx*Bz+2*Ay*Bx*By)
    *d-Ay^2*Bz^2+2*Ay*Az*By*Bz-Az^2*By^2+(-Az^2-Ay^2)*Bx^2)+Bx*d+Az*Bz+Ay*By)/(Bz^2+By^2+Bx^2),

    t= (sqrt((Bz^2+By^2+Bx^2)*R^2+(-Bz^2-By^2)*d^2+(2*Az*Bx*Bz+2*Ay*Bx*By)
    *d-Ay^2*Bz^2+2*Ay*Az*By*Bz-Az^2*By^2+(-Az^2-Ay^2)*Bx^2)-Bx*d-Az*Bz-Ay*By)/(Bz^2+By^2+Bx^2)

    */

    bool Sphere::intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const
    {
      const double      ax = (ray.origin().x());
      const double      ay = (ray.origin().y());
      const double      az = (ray.origin().z());
      const double      bx = (ray.direction().x());
      const double      by = (ray.direction().y());
      const double      bz = (ray.direction().z());

      // double bz2_by2_bx2 = Math::square(bx) + Math::square(by) + Math::square(bx);
      // == 1.0

      double d = az - _roc;
      double ay_by = ay * by;
      double ax_bx = ax * bx;

      double s =
        + Math::square(_roc) // * bz2_by2_bx2
        + 2.0 * (ax_bx + ay_by) * bz * d
        + 2.0 * ax_bx * ay_by
        - Math::square(ay * bx)
        - Math::square(ax * by)
        - (Math::square(bx) + Math::square(by)) * Math::square(d)
        - (Math::square(ax) + Math::square(ay)) * Math::square(bz)
        ;

      // no sphere/ray colision
      if (s < 0)
        return false;

      s = sqrt(s);

      // there are 2 possible sphere/line colision point, keep the right
      // one depending on ray direction
      if (_roc * bz > 0)
        s = -s;

      double t = (s - (bz * d + ax_bx + ay_by)); // / bz2_by2_bx2;

      // do not colide if line intersection is before ray start position
      if (t <= 0)
        return false;

      // intersection point
      point = ray.origin() + ray.direction() * t;

      return true;
    }

    void Sphere::normal(Math::Vector3 &normal, const Math::Vector3 &point) const
    {
      // normalized vector to sphere center
      normal = point;
      normal.z() -= _roc;
      normal.normalize();
      if (_roc < 0)
        normal = -normal;
    }

  }

}

