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
#include <Goptical/Sys/Surface>
#include <Goptical/Sys/Image>

#include <Goptical/Analysis/RayFan>

#include <Goptical/Data/Plot>
#include <Goptical/Data/PlotData>
#include <Goptical/Data/DiscreteSet>

#include <Goptical/Io/RendererViewport>
#include <Goptical/Io/RendererAxes>

#include <Goptical/Trace/Tracer>
#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Params>
#include <Goptical/Trace/Ray>

#include <Goptical/Light/SpectralLine>

namespace _Goptical
{

  namespace Analysis
  {

    RayFan::RayFan(const Sys::System &system, enum rayfan_plane_e plane)
      : _tracer(system),
        _processed_trace(false),
        _entrance(0),
        _exit(0),
        _dist(Trace::SagittalDist, 15)
    {
      set_plane(plane);
    }

    void RayFan::set_plane(enum rayfan_plane_e plane)
    {
      if (plane == SagittalAberration)
        _dist.set_pattern(Trace::SagittalDist);
      else
        _dist.set_pattern(Trace::TangentialDist);

      _dist_plane = _ab_plane = plane;

      invalidate();
    }

    void RayFan::process_trace()
    {
      if (!_processed_trace)
        {
          Trace::Result &result = _tracer.get_trace_result();
          const Sys::System &sys = _tracer.get_system();

          if (!_entrance)
            _entrance = &sys.get_entrance_pupil();
          else
            throw Error("no suitable entrance pupil found for analysis");

          if (!_exit && sys.has_exit_pupil())
            _exit = &sys.get_exit_pupil();
          else
            _exit = sys.find<const Sys::Image>();

          if (!_exit)
            throw Error("no suitable exit surface found for analysis");

          result.clear_save_states();
          result.set_intercepted_save_state(*_exit, true);

          _tracer.get_params().set_distribution(*_entrance, _dist);
          _tracer.get_params().set_unobstructed(true);
          _tracer.trace();

          _processed_trace = true;
        }
    }

    void RayFan::invalidate()
    {
      _processed_trace = false;
    }


    ////////////////////////////////////////////////////////////////////////
    // Aberrations evaluation functions

    double RayFan::get_entrance_height(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      const Trace::Ray *ray = &r;

      // walk up to entrance pupil generated ray
      while (ray && ray->get_creator() != _entrance)
        ray = ray->get_parent();

      if (!ray)
        throw Error();

      return ray->origin()[_dist_plane] / _entrance->get_shape()
        .get_outter_radius(Math::Vector2(1-_dist_plane, _dist_plane));
    }

    double RayFan::get_entrance_angle(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      const Trace::Ray *ray = &r;

      // walk up to entrance pupil generated ray
      while (ray && ray->get_creator() != _entrance)
        ray = ray->get_parent();

      // find parent ray once again
      if (ray && (ray = ray->get_parent()))
        return Math::rad2degree(atan(ray->direction()[_dist_plane] / ray->direction().z()));

      throw Error();
    }

    double RayFan::get_transverse_distance(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      return r.get_intercept_point()[_ab_plane] - chief.get_intercept_point()[_ab_plane];
    }

    double RayFan::get_longitudinal_distance(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      if (&r == &chief)
        throw Error();

      return chief.ln_ln_clst_pt_scale(r) - chief.get_len();
    }

    double RayFan::get_exit_angle(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      const Trace::Ray *ray = r.get_first_child();

      if (!ray)
        throw Error();

      return Math::rad2degree(atan(ray->direction()[_ab_plane] / ray->direction().z()));
    }

    double RayFan::get_image_angle(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      return Math::rad2degree(atan(r.direction()[_ab_plane] / r.direction().z()));
    }

    double RayFan::get_optical_path_len(const Trace::Ray &r, const Trace::Ray &chief) const
    {
      const double wl = r.get_wavelen();
      double dist = 0.0;

      for (const Trace::Ray *ray = &r; ray; ray = ray->get_parent())
        dist += ray->get_len() * ray->get_material()->get_refractive_index(wl);

      return dist / (wl * 1e-6); // opl in wave unit
    }


    ////////////////////////////////////////////////////////////////////////
    // Aberrations plot generation

    const Trace::Ray & RayFan::find_chief_ray(const Trace::rays_queue_t &intercepts, double wavelen)
    {
      GOPTICAL_FOREACH(i, intercepts)
        {
          Trace::Ray & ray = **i;

          if (ray.get_wavelen() == wavelen && fabs(get_entrance_height(ray, /* dummy */ ray)) < 1e-8)
            return ray;
        }

      throw Error("unable to find chief ray intercept");
    }

    ref<Data::Plot> RayFan::get_plot(enum rayfan_plot_type_e x,
                                     enum rayfan_plot_type_e y)
    {
      ref<Data::Plot> plot = GOPTICAL_REFNEW(Data::Plot);

      plot->get_axes().set_position(Math::vector3_0);

      // select X axis evaluation function

      get_value_t get_x_value;
      bool single_x_reference = true;

      switch (x)
        {
        case EntranceHeight:
          get_x_value = &RayFan::get_entrance_height;
          plot->get_axes().set_range(Math::range_t(-1.0, 1.0), Io::RendererAxes::X);
          plot->get_axes().set_tics_step(1.0, Io::RendererAxes::X);
          break;

        case EntranceAngle:
          get_x_value = &RayFan::get_entrance_angle;
          break;

        case ImageAngle:
          get_x_value = &RayFan::get_image_angle;
          break;

        case ExitAngle:
          get_x_value = &RayFan::get_exit_angle;
          break;

        default:
          throw Error("bad value type for X plot axis");
        }


      // select Y axis evaluation function

      get_value_t get_y_value;
      bool single_y_reference = true;

      switch (y)
        {
        case EntranceHeight:
          get_y_value = &RayFan::get_entrance_height;
          break;

        case EntranceAngle:
          get_y_value = &RayFan::get_entrance_angle;
          break;

        case TransverseDistance:
          get_y_value = &RayFan::get_transverse_distance;
          break;

        case LongitudinalDistance:
          get_y_value = &RayFan::get_longitudinal_distance;
          break;

        case OpticalPathDiff:
          get_y_value = &RayFan::get_optical_path_len;
          single_y_reference = false;
          break;

        case ImageAngle:
          get_y_value = &RayFan::get_image_angle;
          break;

        case ExitAngle:
          get_y_value = &RayFan::get_exit_angle;
          break;

        default:
          throw Error("bad value type for Y plot axis");
        }

      // process ray tracing

      process_trace();

      Trace::Result &result = _tracer.get_trace_result();
      const Trace::rays_queue_t &intercepts = result.get_intercepted(*_exit);

      if (intercepts.empty() || result.get_ray_wavelen_set().empty())
        throw Error("no raytracing data available for analysis");

      bool first = true;
      double x_ref = 0.0, y_ref = 0.0;

      // extract data for each wavelen

      GOPTICAL_FOREACH(w, result.get_ray_wavelen_set())
        {
          const Trace::Ray & chief_ray = find_chief_ray(intercepts, *w);

          // get chief ray reference values

          if (!single_x_reference || first)
            try {
              x_ref = (this->*get_x_value)(chief_ray, chief_ray);
            } catch (...) {
              x_ref = 0.0;      // no valid data for chief ray
            }

          if (!single_y_reference || first)
            try {
              y_ref = (this->*get_y_value)(chief_ray, chief_ray);
            } catch (...) {
              y_ref = 0.0;      // no valid data for chief ray
            }

          first = false;

          // extract data for each ray

          ref<Data::DiscreteSet> s = GOPTICAL_REFNEW(Data::DiscreteSet);
          s->set_interpolation(Data::Cubic);

          GOPTICAL_FOREACH(i, intercepts)
            {
              Trace::Ray & ray = **i;

              if (ray.get_wavelen() != *w)
                continue;

              double x_val, y_val;

              try {
                x_val = (this->*get_x_value)(ray, chief_ray) - x_ref;
                y_val = (this->*get_y_value)(ray, chief_ray) - y_ref;

              } catch (...) {

                // no valid data for this ray
                continue;
              }

              // add data to plot
              s->add_data(x_val, y_val);
            }

          if (!s->get_count())
            continue;

          Data::PlotData p(*s);
          p.set_color(Light::SpectralLine::get_wavelen_color(*w));
          plot->add_plot_data(p);
        }

      static const struct
      {
        const char *label;
        const char *unit;
        bool prefix;
        int pow10;
      } axis[] =
        {
          { "Entrance ray height (normalized)", "", false },
          { "Entrance ray angle", "degree", false },
          { "Transverse ray aberration", "m", true, -3 },
          { "Longitudinal ray aberration", "m", true, -3 },
          { "Image ray angle", "degree", false },
          { "Exit ray angle", "degree", false },
          { "Optical path difference", "waves", false }
        };

      plot->set_title(axis[y].label + std::string(" fan (") +
                      (_dist_plane == SagittalAberration ?
                       "sagittal)" : "tangential)"));
      plot->get_axes().set_label(axis[x].label, Io::RendererAxes::X);
      plot->get_axes().set_label(axis[y].label, Io::RendererAxes::Y);
      plot->get_axes().set_unit(axis[x].unit, true, axis[x].prefix,
                                axis[x].pow10, Io::RendererAxes::X);
      plot->get_axes().set_unit(axis[y].unit, true, axis[y].prefix,
                                axis[y].pow10, Io::RendererAxes::Y);
      plot->set_style(Data::InterpolatePlot);

      return plot;
    }

  }

}

