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

#include <Goptical/Sys/Group>
#include <Goptical/Math/VectorPair>
#include <Goptical/Io/Renderer>

namespace _Goptical {

  namespace Sys {

    Group::~Group()
    {
      remove_all();
    }

    void Group::added(Element &e)
    {
      // register new element if group is part of a system
      if (_system)
        e.system_register(*_system);
    }

    void Group::removed(Element &e)
    {
      // unregister removed element if group is part of a system
      if (_system)
        e.system_unregister();
    }

    void Group::system_register(System &s)
    {
      Element::system_register(s);

      // register all children elements
      GOPTICAL_FOREACH(i, get_element_list())
        (*i)->system_register(s);
    }

    void Group::system_unregister()
    {
      assert(_system);

      // unregister all children elements
      GOPTICAL_FOREACH(i, get_element_list())
        (*i)->system_unregister();

      Element::system_unregister();
    }

    void Group::system_moved()
    {
      GOPTICAL_FOREACH(i, get_element_list())
        (*i)->system_moved();

      Element::system_moved();
    }

    Math::VectorPair3 Group::get_bounding_box() const
    {
      return Container::get_bounding_box();
    }

    void Group::draw_2d_e(Io::Renderer &r, const Element *ref) const
    {
      GOPTICAL_FOREACH(i, get_element_list())
        r.draw_element_2d(**i, ref);
    }

    void Group::draw_3d_e(Io::Renderer &r, const Element *ref) const
    {
      GOPTICAL_FOREACH(i, get_element_list())
        r.draw_element_3d(**i, ref);
    }

  }
}

