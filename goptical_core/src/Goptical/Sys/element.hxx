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


#ifndef GOPTICAL_ELEMENT_HXX_
#define GOPTICAL_ELEMENT_HXX_

#include "Goptical/Math/transform.hxx"
#include "Goptical/Math/vector_pair.hxx"
#include "Goptical/Math/vector_pair.hxx"

namespace _Goptical {

  namespace Sys {

    void Element::set_local_position(const Math::Vector3 &v)
    {
      _transform.set_translation(v);
      system_moved();
    }

    void Element::rotate(double x, double y, double z)
    {
      _transform.linear_rotation(Math::Vector3(x, y, z));
      system_moved();
    }

    const Math::Vector3 & Element::get_local_position() const
    {
      return _transform.get_translation();
    }

    const Math::Transform<3> & Element::get_transform() const
    {
      return _transform;
    }

    void Element::set_transform(const Math::Transform<3> &t)
    {
      _transform = t;
      system_moved();
    }

    unsigned int Element::id() const
    {
      return _system_id;
    }

    System * Element::get_system() const
    {
      return _system;
    }

    unsigned int Element::get_version() const
    {
      return _version;
    }

    bool Element::is_enabled() const
    {
      return _enabled;
    }

    void Element::set_enable_state(bool enabled)
    {
      if (_enabled != enabled)
        update_version();
      _enabled = enabled;
    }

    template <Trace::IntensityMode m>
    inline void Element::process_rays(Trace::Result &result,
                                     Trace::rays_queue_t *input) const
    {
      switch (m)
        {
        case Trace::SimpleTrace:
          process_rays_simple(result, input);
          break;

        case Trace::IntensityTrace:
          process_rays_intensity(result, input);
          break;

        case Trace::PolarizedTrace:
          process_rays_polarized(result, input);
          break;
        }
    }

    std::ostream & operator<<(std::ostream &o, const Element &e)
    {
      e.print(o);
      return o;
    }

  }
}

#endif

