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


#include <Goptical/Sys/System>
#include <Goptical/Sys/Element>

#include <Goptical/Trace/Ray>
#include <Goptical/Trace/Result>

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>

#include <Goptical/Io/Renderer>

namespace _Goptical {

  namespace Trace {

    Result::Result()
      : _rays(),
        _elements(),
        _wavelengths(),
        _generated_queue(0),
        _sources(),
        _bounce_limit_count(0),
        _system(0)
    {
    }

    Result::~Result()
    {
      clear();
    }

    void Result::clear_save_states()
    {
      GOPTICAL_FOREACH(i, _elements)
        {
          i->_save_intercepted_list = false;
          i->_save_generated_list = false;
        }
    }

    void Result::clear()
    {
      GOPTICAL_FOREACH(i, _elements)
        {
          if (i->_intercepted)
            {
              delete i->_intercepted;
              i->_intercepted = 0;
            }

          if (i->_generated)
            {
              delete i->_generated;
              i->_generated = 0;
            }
        }

      _rays.clear();
      _sources.clear();
      _wavelengths.clear();

      _bounce_limit_count = 0;
    }

    void Result::prepare()
    {
      clear();

      GOPTICAL_FOREACH(i, _elements)
        {
          if (i->_save_intercepted_list)
            i->_intercepted = new rays_queue_t;

          if (i->_save_generated_list)
            i->_generated = new rays_queue_t;
        }
    }

    void Result::init(const Sys::System &system)
    {
      static const struct element_result_s er = { 0 };

      if (!_system)
        _system = &system;

      if(_system != &system)
        throw Error("Trace::Result used with multiple Sys::System objects");

      _elements.resize(system.get_element_count(), er);
    }

    void Result::init(const Sys::Element &element)
    {
      const Sys::System *system = element.get_system();

      assert(system != 0);

      init(*system);
    }

    void Result::set_intercepted_save_state(const Sys::Element &e, bool enabled)
    {
      init(e);
      get_element_result(e)._save_intercepted_list = enabled;
    }

    void Result::set_generated_save_state(const Sys::Element &e, bool enabled)
    {
      init(e);
      get_element_result(e)._save_generated_list = enabled;
    }

    bool Result::get_intercepted_save_state(const Sys::Element &e)
    {
      return get_element_result(e)._save_intercepted_list;
    }

    bool Result::get_generated_save_state(const Sys::Element &e)
    {
      return get_element_result(e)._save_generated_list;
    }


    Math::VectorPair3 Result::get_intercepted_window(const Sys::Surface &s) const
    {
      const rays_queue_t & intercepts = get_intercepted(s);

      if (intercepts.empty())
        throw Error("no ray intercepts found on the surface");

      const Math::Vector3 &first = (*intercepts.begin())->get_intercept_point();
      Math::VectorPair3 window(first, first);

      GOPTICAL_FOREACH(i, intercepts)
        {
          const Math::Vector3 &ip = (*i)->get_intercept_point();

          if (window[0].x() > ip.x())
            window[0].x() = ip.x();
          else if (window[1].x() < ip.x())
            window[1].x() = ip.x();

          if (window[0].y() > ip.y())
            window[0].y() = ip.y();
          else if (window[1].y() < ip.y())
            window[1].y() = ip.y();

          if (window[0].z() > ip.z())
            window[0].z() = ip.z();
          else if (window[1].z() < ip.z())
            window[1].z() = ip.z();
        }

      return window;
    }

    Math::Vector3 Result::get_intercepted_center(const Sys::Surface &s) const
    {
      Math::VectorPair3 win = get_intercepted_window(s);

      return (win[0] + win[1]) / 2;
    }

    Math::Vector3 Result::get_intercepted_centroid(const Sys::Surface &s) const
    {
      const rays_queue_t & intercepts = get_intercepted(s);
      unsigned int      count = 0;
      Math::Vector3     center(0, 0, 0);

      if (intercepts.empty())
        throw Error("no ray intercepts found on the surface");
      
      GOPTICAL_FOREACH(i, intercepts)
        {
          center += (*i)->get_intercept_point();
          count++;
        }

      center /= count;

      return center;
    }

    double Result::get_max_ray_intensity() const
    {
      double res = 0;

      GOPTICAL_FOREACH(r, _rays)
        {
          double i = r->get_intensity();

          if (i > res)
            res = i;
        }

      return res;
    }

    void Result::draw_2d(Io::Renderer &r, bool hit_image, const Sys::Element *ref) const
    {
      r.draw_trace_result_2d(*this, hit_image, ref);
    }

    void Result::draw_3d(Io::Renderer &r, bool hit_image, const Sys::Element *ref) const
    {
      r.draw_trace_result_3d(*this, hit_image, ref);
    }

  }
}

