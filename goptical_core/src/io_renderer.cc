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


#include <Goptical/Io/Renderer>

#include <Goptical/Trace/Ray>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Params>

#include <Goptical/Sys/Image>

#include <Goptical/Math/Triangle>
#include <Goptical/Math/VectorPair>

#include <Goptical/Light/Ray>
#include <Goptical/Light/SpectralLine>

namespace _Goptical {

  namespace Io {

    Renderer::Renderer()
      : _feature_size(20.),
        _ray_color_mode(RayColorWavelen),
        _intensity_mode(IntensityIgnore)
    {
      _styles_color[StyleForeground] = Rgb(1.0, 1.0, 1.0);
      _styles_color[StyleBackground] = Rgb(0.0, 0.0, 0.0);
      _styles_color[StyleRay] = Rgb(1.0, 0.0, 0.0);
      _styles_color[StyleSurface] = Rgb(0.5, 0.5, 1.0);
      _styles_color[StyleGlass] = Rgb(0.8, 0.8, 1.0);      
    }

    void Renderer::group_begin(const std::string &name)
    {
    }

    void Renderer::group_end()
    {
    }

    /**********************************************************************
     * Optical elements drawing
     */

    void Renderer::draw_element_2d(const Sys::Element &e, const Sys::Element *ref)
    {
      group_begin("element");
      e.draw_2d_e(*this, ref);
      group_end();
    }

    void Renderer::draw_element_3d(const Sys::Element &e, const Sys::Element *ref)
    {
      group_begin("element");
      e.draw_3d_e(*this, ref);
      group_end();
    }

    /**********************************************************************
     * Light ray drawing
     */

    template <unsigned D, bool draw_lost>
    bool Renderer::draw_traced_ray_recurs(const Trace::Ray &ray, double lost_len,
                                          const Sys::Element *ref, bool hit_image)
    {
      const Math::Transform<3> &t1 = ray.get_creator()->get_transform_to(ref);
      Math::VectorPair3 p;
      Sys::Element *i_element = 0;

      p[0] = t1.transform(ray.origin());

      if (ray.is_lost())
        {
          if (!draw_lost)
            return false;

          p[1] = t1.transform(ray.origin() + ray.direction() * lost_len);
        }
      else
        {
          i_element = &ray.get_intercept_element();
          const Math::Transform<3> &t2 = i_element->get_transform_to(ref);
          p[1] = t2.transform(ray.get_intercept_point());
        }

      bool done = false;

      for (Trace::Ray *r = ray.get_first_child(); r; r = r->get_next_child())
        done |= draw_traced_ray_recurs<D, true>(*r, lost_len, ref, hit_image);

      if (!done && hit_image && !dynamic_cast<const Sys::Image*>(i_element))
        return false;

      switch (D)
        {
        case 2:
          // skip non tangential rays in 2d mode
          if (fabs(p.x1()) > 1e-6)
            return false;

          draw_ray_line(Math::VectorPair2(p[0].project_zy(), p[1].project_zy()), ray);
          break;

        case 3:
          draw_ray_line(p, ray);
          break;
        }

      return true;
    }

    template <unsigned D>
    void Renderer::draw_trace_result(const Trace::Result &result, const Sys::Element *ref,
                                     bool hit_image)
    {
      const Trace::Result::sources_t &sl = result.get_source_list();
      double lost_len = result.get_params().get_lost_ray_length();

      if (sl.empty())
        throw Error("No source found in trace result");

      _max_intensity = result.get_max_ray_intensity();

      GOPTICAL_FOREACH(s, sl)
        {
          const Trace::rays_queue_t &rl = result.get_generated(*(Sys::Element*)*s);

          GOPTICAL_FOREACH(r, rl)
            {
              group_begin("ray");
              draw_traced_ray_recurs<D, false>(**r, lost_len, ref, hit_image);
              group_end();
            }
        }
    }

    void Renderer::draw_trace_result_2d(const Trace::Result &result, bool hit_image, const Sys::Element *ref)
    {
      group_begin("rays");
      draw_trace_result<2>(result, ref, hit_image);
      group_end();
    }

    void Renderer::draw_trace_result_3d(const Trace::Result &result, bool hit_image, const Sys::Element *ref)
    {
      group_begin("rays");
      draw_trace_result<3>(result, ref, hit_image);
      group_end();
    }

    void Renderer::draw_intercepts(const Trace::Result &result, const Sys::Surface &s)
    {
      _max_intensity = result.get_max_ray_intensity();

      GOPTICAL_FOREACH(i, result.get_intercepted(s))
        {
          const Trace::Ray &ray = **i;
          // dont need global transform here, draw ray intercept points in
          // surface local coordinates.
          draw_point(ray.get_intercept_point().project_xy(), ray_to_rgb(ray));
        }
    }

    const Rgb Renderer::ray_to_rgb(const Light::Ray & ray)
    {
      switch (_ray_color_mode)
        {
        case RayColorWavelen:
          return Light::SpectralLine::get_wavelen_color(ray.get_wavelen());
          
        default:
        case RayColorFixed:
          return get_style_color(StyleRay);
        }
    }

    float Renderer::ray_to_alpha(const Light::Ray & ray) const
    {
      switch (_intensity_mode)
        {
        case IntensityIgnore:
          return 0.0;

        case IntensityShade:
          return 1.0 - std::min(ray.get_intensity() / _max_intensity, 1.0);

        case IntensityLogShade: // FIXME add log
          return 1.0 - std::min(ray.get_intensity() / _max_intensity, 1.0);
        }

      return 0;
    }

    void Renderer::draw_ray_line(const Math::VectorPair2 &l, const Trace::Ray &ray)
    {
      draw_segment(l, ray_to_rgb(ray));
    }

    void Renderer::draw_ray_line(const Math::VectorPair3 &l, const Trace::Ray &ray)
    {
      draw_segment(l, ray_to_rgb(ray));
    }

    /**********************************************************************
     * Misc shapes 2d drawing
     */
 
    void Renderer::draw_polygon(const Math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed)
    {
      unsigned int i;

      if (count < 3)
        return;

      for (i = 0; i + 1 < count; i++)
        draw_segment(Math::VectorPair2(array[i], array[i + 1]), rgb);

      if (closed)
        draw_segment(Math::VectorPair2(array[i], array[0]), rgb);
    }

    void Renderer::draw_circle(const Math::Vector2 &v, double r, const Rgb &rgb, bool filled)
    {
      unsigned int count = std::min(100, std::max(6, (int)(2. * M_PI * r / _feature_size)));
      DPP_VLARRAY(Math::Vector2, count, p);
      double astep = 2. * M_PI / count;
      double a = astep;
      p[0] = Math::Vector2(r, 0);

      for (unsigned int i = 0; i < count; i++, a += astep)
        p[i] = v + Math::Vector2(r * cos(a), r * sin(a));

      draw_polygon(&p[0], count, rgb, filled, true);
    }

    void Renderer::draw_triangle(const Math::Triangle<3> &t, const Rgb &rgb)
    {
      draw_polygon(&t[0], 3, rgb, false, true);
    }

    void Renderer::draw_triangle(const Math::Triangle<3> &t, const Math::Triangle<3> &gradient, const Rgb &rgb)
    {
      draw_triangle(t, rgb);
    }

    void Renderer::draw_polygon(const Math::Vector3 *array, unsigned int count,
                                const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      unsigned int i;

      for (i = 0; i + 1 < count; i++)
        draw_segment(array[i], array[i + 1], rgb);

      if (closed)
        draw_segment(array[i], array[0], rgb);
    }

    void Renderer::draw_box(const Math::VectorPair2 &c, const Rgb &rgb)
    {
      draw_segment(Math::Vector2(c[0].x(), c[0].y()), Math::Vector2(c[1].x(), c[0].y()), rgb);
      draw_segment(Math::Vector2(c[1].x(), c[1].y()), Math::Vector2(c[1].x(), c[0].y()), rgb);
      draw_segment(Math::Vector2(c[1].x(), c[1].y()), Math::Vector2(c[0].x(), c[1].y()), rgb);
      draw_segment(Math::Vector2(c[0].x(), c[0].y()), Math::Vector2(c[0].x(), c[1].y()), rgb);
    }

    void Renderer::draw_triangle(const Math::Triangle<2> &t, bool filled, const Rgb &rgb)
    {
      draw_polygon(&t[0], 3, rgb, filled, true);
    }

  }

}

