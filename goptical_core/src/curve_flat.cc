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


#include <Goptical/Curve/Flat>
#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Curve {

    Flat::Flat()
    {
    }

    double Flat::sagitta(double r) const
    {
      return 0;
    }

    double Flat::derivative(double r) const
    {
      return 1.0;
    }

    /*

    intersection d'un plan defini par :

    P(Px, Py, Pz) appartenant au plan
    N(Px, Py, Pz) normal au plan

    avec une droite AB definie par l'ensemble des points tel que:

    A + * t B

    on a :

    t=(Nz*Pz+Ny*Py+Nx*Px-Az*Nz-Ay*Ny-Ax*Nx)/(Bz*Nz+By*Ny+Bx*Nx)

    */

    bool Flat::intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const
    {
      double    s = ray.direction().z();

      if (s == 0)
        return false;

      double    a = -ray.origin().z() / s;

      if (a < 0)
        return false;

      point = ray.origin() + ray.direction() * a;

      return true;
    }

    void Flat::normal(Math::Vector3 &normal, const Math::Vector3 &point) const
    {
      normal = Math::Vector3(0, 0, -1);
    }

    Flat flat;

  }

}

