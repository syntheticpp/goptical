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

#include <Goptical/Curve/Rotational>
#include <Goptical/Math/Vector>
#include <Goptical/Math/Vector>

namespace _Goptical {

  namespace Curve {

    Rotational::Rotational()
    {
      gsl_func.function = gsl_func_sagitta;
      gsl_func.params = this;
    }

    void Rotational::normal(Math::Vector3 &normal, const Math::Vector3 &point) const
    {
      const double r = sqrt(Math::square(point.x()) + Math::square(point.y()));

      if (r == 0)
        normal = Math::Vector3(0, 0, -1);
      else
        {
          const double p = derivative(r);

          normal = Math::Vector3(point.x() * p / r, point.y() * p / r, -1.0);
          normal.normalize(); // FIXME simplify ?
        }
    }

    void Rotational::derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const
    {
      const double r = xy.len();

      if (r == 0)
        {
          dxdy.x() = dxdy.y() = 0.0;
          return;
        }

      const double p = derivative(r);

      dxdy = xy * (p / r);
    }

    double Rotational::gsl_func_sagitta(double x, void *params)
    {
      Rotational *c = static_cast<Rotational *>(params);

      return c->sagitta(x);
    }

    double Rotational::derivative(double r) const
    {
      double result, abserr;

      gsl_deriv_central(&gsl_func, r, 1e-4, &result, &abserr);

      return result;
    }

    unsigned int Rotational::get_sample_count() const
    {
      return 0;
    }

    void Rotational::get_sample(unsigned int index, double &r, double &z) const
    {
    }

  }

}

