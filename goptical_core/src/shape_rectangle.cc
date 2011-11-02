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

#include <cstdlib>
#include <cassert>

#include <Goptical/Shape/Rectangle>

#include <Goptical/Trace/Distribution>
#include <Goptical/Math/Triangle>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Shape {

    bool Rectangle::inside(const Math::Vector2 &point) const
    {
      return (fabs(point.x()) <= _halfsize.x() && fabs(point.y()) <= _halfsize.y());
    }

    void Rectangle::get_pattern(const Math::Vector2::put_delegate_t  &f,
                                const Trace::Distribution &d,
                                bool unobstructed) const
    {
      static const double epsilon = 1e-8;
      const Math::Vector2 hs = _halfsize * d.get_scaling();
      const Math::Vector2 step = hs / (double)(d.get_radial_density() / 2);

      switch (d.get_pattern())
        {
        case Trace::MeridionalDist: {

          f(Math::Vector2(0, 0));

          for (double y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
              f(Math::Vector2(0, y));
              f(Math::Vector2(0, -y));
            }
          break;
        }

        case Trace::SagittalDist: {

          f(Math::Vector2(0, 0));

          for (double x = step.x(); x < hs.x() + epsilon; x += step.x())
            {
              f(Math::Vector2(x, 0));
              f(Math::Vector2(-x, 0));
            }
          break;
        }

        case Trace::CrossDist: {

          f(Math::Vector2(0, 0));

          for (double x = step.x(); x < hs.x() + epsilon; x += step.x())
            {
              f(Math::Vector2(x, 0));
              f(Math::Vector2(-x, 0));
            }

          for (double y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
              f(Math::Vector2(0, y));
              f(Math::Vector2(0, -y));
            }
          break;
        }

        case Trace::DefaultDist:
        case Trace::SquareDist: {
          double x, y;

          f(Math::Vector2(0, 0));

          for (x = step.x(); x < hs.x() + epsilon; x += step.x())
            for (y = step.y(); y < hs.y() + epsilon; y += step.y())
              {
                f(Math::Vector2(x, y));
                f(Math::Vector2(-x, y));
                f(Math::Vector2(x, -y));
                f(Math::Vector2(-x, -y));
              }

          for (x = step.x(); x < hs.x() + epsilon; x += step.x())
            {
              f(Math::Vector2(x, 0));
              f(Math::Vector2(-x, 0));
            }

          for (y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
              f(Math::Vector2(0, y));
              f(Math::Vector2(0, -y));
            }
          break;
        }

        default:
          Base::get_pattern(f, d, unobstructed);
        }
    }

    Math::VectorPair2 Rectangle::get_bounding_box() const
    {
      return Math::VectorPair2(-_halfsize, _halfsize);
    }

    unsigned int Rectangle::get_contour_count() const
    {
      return 1;
    }

    inline Math::Vector2 Rectangle::get_step(double resolution) const
    {
      Math::Vector2 step;

      for (unsigned int i = 0; i < 2; i++)
        {
          if (resolution > _halfsize[i])
            step[i] = _halfsize[i];
          else
            step[i] = _halfsize[i] / round(_halfsize[i] / resolution);
        }

      return step;
    }

    void Rectangle::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
    {
      static const double epsilon = 1e-8;
      assert(contour < 1);

      Math::Vector2 step = get_step(resolution);

      double x, y;

      for (x = - _halfsize.x(); x < _halfsize.x() - epsilon; x += step.x())
        f(Math::Vector2(x, - _halfsize.y()));

      for (y = - _halfsize.y(); y < _halfsize.y() - epsilon; y += step.y())
        f(Math::Vector2(_halfsize.x(), y));

      for (x = _halfsize.x(); x > - _halfsize.x() + epsilon; x -= step.x())
        f(Math::Vector2(x, _halfsize.y()));

      for (y = _halfsize.y(); y > - _halfsize.y() + epsilon; y -= step.y())
        f(Math::Vector2(- _halfsize.x(), y));
    }

    void Rectangle::get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const
    {
      static const double epsilon = 1e-8;
      Math::Vector2 step = get_step(resolution);

      for (unsigned int i = 0; i < 2; i++)
        {
          if (resolution > _halfsize[i])
            step[i] = _halfsize[i];
          else
            step[i] = _halfsize[i] / round(_halfsize[i] / resolution);
        }

      for (double x = 0; x < _halfsize.x() - epsilon; x += step.x())
        for (double y = 0; y < _halfsize.y() - epsilon; y += step.y())
          {
            Math::Vector2 a(x, y);
            Math::Vector2 b(x + step.x(), y);
            Math::Vector2 c(x, y + step.y());
            Math::Vector2 d(x + step.x(), y + step.y());

            f(Math::Triangle<2>(b, a, c));
            f(Math::Triangle<2>(d, b, c));
            f(Math::Triangle<2>(-b, -a, -c));
            f(Math::Triangle<2>(-d, -b, -c));

            a.x() = -a.x();
            b.x() = -b.x();
            c.x() = -c.x();
            d.x() = -d.x();

            f(Math::Triangle<2>(a, b, c));
            f(Math::Triangle<2>(b, d, c));
            f(Math::Triangle<2>(-a, -b, -c));
            f(Math::Triangle<2>(-b, -d, -c));
          }
    }

    double Rectangle::get_outter_radius(const Math::Vector2 &dir) const
    {
      const Math::Vector2 &e = (fabs(dir.x() / dir.y()) < _halfsize.x() / _halfsize.y())
        ? Math::vector2_10 : Math::vector2_01;

      return (Math::VectorPair2(_halfsize, e)
              .ln_intersect_ln(Math::VectorPair2(Math::vector2_0, dir))).len();
    }

  }

}
