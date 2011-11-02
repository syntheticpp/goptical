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

#ifndef GOPTICAL_SYSTEM_HH_
#define GOPTICAL_SYSTEM_HH_

#include <iostream>

#include "Goptical/common.hh"

#include "Goptical/Sys/element.hh"
#include "Goptical/Sys/container.hh"
#include "Goptical/Trace/params.hh"
#include "Goptical/Material/proxy.hh"

namespace _Goptical {

  namespace Sys {

    /**
       @short Optical system
       @header Goptical/Sys/System
       @module {Core}
       @main

       This class is used to describe an optical system. Any optical
       @ref Element {element} may be part of a system. This class handle 3d
       transformation between elements local coordinates.

       @xsee {tuto_system}
    */
    class System : public ref_base<System>, public Container
    {
      friend class Element;

    public:
      /** Create a new empty system. */
      System();
      virtual ~System();

      /** Define an entrance pupil surface used to project source rays */
      inline void set_entrance_pupil(const const_ref<Surface> &entrance);
      /** Discard defined entrance pupil */
      inline void undef_entrance_pupil();
      /** Get defined entrance pupil surface or try to guess it if none defined */
      const Surface &get_entrance_pupil() const;
      /** Test if an entrance pupil has been defined */
      inline bool has_entrance_pupil() const;

      /** Define an exit pupil surface */
      inline void set_exit_pupil(const const_ref<Surface> &exit);
      /** Get exit pupil */
      inline const Surface &get_exit_pupil() const;
      /** Test if an exit pupil has been defined */
      inline bool has_exit_pupil() const;

      /** Get default tracer parameters */
      inline const Trace::Params & get_tracer_params() const;
      /** Get default tracer parameters */
      inline Trace::Params & get_tracer_params();

      /** Get transform between two elements local coordinates */
      inline const Math::Transform<3> & get_transform(const Element &from, const Element &to) const;

      /** Get transform from element local to global coordinates */
      inline const Math::Transform<3> & get_global_transform(const Element &from) const;

      /** Get transform from global to element local coordinates */
      inline const Math::Transform<3> & get_local_transform(const Element &to) const;

      /** Get system version. version is updated each time system or
          associated elements properties are changed */
      inline unsigned int get_version() const;

      /** Get the number of registered elements in the system */
      inline unsigned int get_element_count() const;

      /** Get registered element. first element has index 1 */
      inline Element & get_element(unsigned int index) const;

      /** Increase current system version */
      inline void update_version();

      /** Find surface which colides with the given ray and update intersection point */
      Surface * colide_next(const Trace::Params &params,
                            Math::VectorPair3 &intersect,
                            const Trace::Ray &ray) const;

      /** set environment material */
      void set_environment(const const_ref<Material::Base> &env);

      /** get environment material */
      inline const Material::Base & get_environment() const;

      /** @internal get environment material proxy */
      inline const Material::Base & get_environment_proxy() const;

      /** @internal Dump 3d transforms cache */
      void transform_cache_dump(std::ostream &o) const;

    private:

      /** called be container class when a new element is added */
      void added(Element &e);
      /** called be container class when a new element is removed */
      void removed(Element &e);

      /** get an new element identifier */
      unsigned int index_get(Element &element);
      /** free the identifier associated with the given element */
      void index_put(const Element &element);

      /** Get a reference to cache entry for transform between 2 elements (ids) */
      inline Math::Transform<3> * & transform_cache_entry(unsigned int from, unsigned int to) const;

      /** Compute and get 3d transform between element local and global coordinates */
      const Math::Transform<3> & transform_l2g_cache_update(const Element &e) const;
      /** Compute and get 3d transform between element local and global coordinates */
      const Math::Transform<3> & transform_g2l_cache_update(const Element &e) const;
      /** Compute and get 3d transform between two elements local coordinates */
      const Math::Transform<3> & transform_cache_update(const Element &from, const Element &to) const;

      /** Flush all cached transforms associated with a given element */
      void transform_cache_flush(const Element &element);
      /** Flush all cached transforms */
      void transform_cache_flush();

      /** Resize transform cache size */
      void transform_cache_resize(unsigned int newsize);

      unsigned int              _version;

      const_ref<Surface>        _entrance;
      const_ref<Surface>        _exit;
      Material::Proxy           _env_proxy;
      Trace::Params             _tracer_params;
      unsigned int              _e_count;
      std::vector<Element *>    _index_map;
      // FIXME use transform pool instead of new/delete
      std::vector<Math::Transform<3> *> _transform_cache;
    };

  }
}

#endif

