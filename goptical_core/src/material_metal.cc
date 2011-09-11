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


#include <Goptical/Data/DiscreteSet>
#include <Goptical/Material/Metal>

namespace _Goptical {

  namespace Material {

    Metal::Metal()
      : _extinction(),
        _refractive_index()
    {
      _extinction.set_interpolation(Data::Cubic);
      _refractive_index.set_interpolation(Data::Cubic);
    }

    bool Metal::is_opaque() const
    {
      return true;
    }

    bool Metal::is_reflecting() const
    {
      return true;
    }

    double Metal::get_absolute_refractive_index(double wavelen) const
    {
      return _refractive_index.interpolate(wavelen);
    }

    double Metal::get_refractive_index(double wavelen) const
    {
      // FIXME find a formula
      return _refractive_index.interpolate(wavelen);
    }

    double Metal::get_extinction_coef(double wavelen) const
    {
      return _extinction.interpolate(wavelen);
    }

  }

}

