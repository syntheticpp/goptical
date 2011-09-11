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


#ifndef GOPTICAL_TRACEDRAY_HXX_
#define GOPTICAL_TRACEDRAY_HXX_

#include <limits>

#include "Goptical/Light/ray.hxx"
#include "Goptical/Math/vector.hxx"

#include "Goptical/Sys/element.hh"
#include "Goptical/Sys/element.hxx"

#include "Goptical/Math/transform.hh"
#include "Goptical/Math/transform.hxx"

namespace _Goptical {

  namespace Trace {

    Ray::Ray()
      : Light::Ray(),
        _len(std::numeric_limits<double>::max()),
        _creator(0),
        _parent(0),
        _child(0),
        _lost(true)
    {
    }

    Ray::Ray(const Light::Ray &r)
      : Light::Ray(r),
        _len(std::numeric_limits<double>::max()),
        _creator(0),
        _parent(0),
        _child(0),
        _lost(true)
    {
    }

    void Ray::add_generated(Ray *r)
    {
      assert(!r->_parent);
      r->_parent = this;
      r->_next = _child;
      _child = r;
    }

    void Ray::set_intercept(const Sys::Element &e, const Math::Vector3 &point)
    {
      _i_element = (Sys::Element*)&e;
      _point = point;
      _lost = false;
    }

    Ray * Ray::get_parent() const
    {
      return _parent;
    }

    Ray * Ray::get_next_child() const
    {
      return _next;
    }

    Ray * Ray::get_first_child() const
    {
      return _child;
    }

    const Math::Vector3 & Ray::get_intercept_point() const
    {
      return _point;
    }

    bool Ray::is_lost() const
    {
      return _lost;
    }

    Sys::Element & Ray::get_intercept_element() const
    {
      return *_i_element;
    }

    Math::Vector3 Ray::get_position(const Sys::Element &e) const
    {
      return _creator->get_transform_to(e).transform(origin());
    }

    Math::Vector3 Ray::get_direction(const Sys::Element &e) const
    {
      return _creator->get_transform_to(e).transform_linear(direction());
    }

    Math::Vector3 Ray::get_position() const
    {
      return _creator->get_global_transform().transform(origin());
    }

    Math::Vector3 Ray::get_direction() const
    {
      return _creator->get_global_transform().transform_linear(direction());
    }

  }
}

#endif

