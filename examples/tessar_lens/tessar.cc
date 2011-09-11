/*

      This file is part of the Goptical library.
  
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

/* -*- indent-tabs-mode: nil -*- */

#include <iostream>
#include <fstream>

#include <Goptical/Math/Vector>

#include <Goptical/Material/Abbe>

#include <Goptical/Sys/System>
#include <Goptical/Sys/Lens>
#include <Goptical/Sys/Source>
#include <Goptical/Sys/SourceRays>
#include <Goptical/Sys/SourcePoint>
#include <Goptical/Sys/Image>
#include <Goptical/Sys/Stop>

#include <Goptical/Curve/Sphere>
#include <Goptical/Shape/Disk>

#include <Goptical/Trace/Tracer>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Distribution>
#include <Goptical/Trace/Sequence>
#include <Goptical/Trace/Params>

#include <Goptical/Light/SpectralLine>

#include <Goptical/Analysis/RayFan>
#include <Goptical/Analysis/Spot>
#include <Goptical/Data/Plot>

#include <Goptical/Io/RendererSvg>
#include <Goptical/Io/Rgb>

using namespace Goptical;

int main()
{
  //**********************************************************************
  // Optical system definition

  Sys::System   sys;

  /* anchor lens */
  Sys::Lens     lens(Math::Vector3(0, 0, 0));

  //               roc,            ap.radius, thickness,

  lens.add_surface(1/0.031186861,  14.934638, 4.627804137,
                   ref<Material::AbbeVd>::create(1.607170, 59.5002));

  lens.add_surface(0,              14.934638, 5.417429465);

  lens.add_surface(1/-0.014065441, 12.766446, 3.728230979,
                   ref<Material::AbbeVd>::create(1.575960, 41.2999));

  lens.add_surface(1/0.034678487,  11.918098, 4.417903733);

  lens.add_stop   (                12.066273, 2.288913925);

  lens.add_surface(0,              12.372318, 1.499288597,
                   ref<Material::AbbeVd>::create(1.526480, 51.4000));

  lens.add_surface(1/0.035104369,  14.642815, 7.996205852,
                   ref<Material::AbbeVd>::create(1.623770, 56.8998));

  lens.add_surface(1/-0.021187519, 14.642815, 85.243965130);

  sys.add(lens);
  /* anchor end */

  Sys::Image      image(Math::Vector3(0, 0, 125.596), 5);
  sys.add(image);

  /* anchor sources */
  Sys::SourceRays  source_rays(Math::Vector3(0, 27.5, -1000));

  Sys::SourcePoint source_point(Sys::SourceAtFiniteDistance,
                                Math::Vector3(0, 27.5, -1000));

  // add sources to system
  sys.add(source_rays);
  sys.add(source_point);

  // configure sources
  source_rays.add_chief_rays(sys);
  source_rays.add_marginal_rays(sys, 14);

  source_point.clear_spectrum();
  source_point.add_spectral_line(Light::SpectralLine::C);
  source_point.add_spectral_line(Light::SpectralLine::e);
  source_point.add_spectral_line(Light::SpectralLine::F);
  /* anchor end */

  /* anchor seq */
  Trace::Sequence seq(sys);

  sys.get_tracer_params().set_sequential_mode(seq);
  std::cout << "system:" << std::endl << sys;
  std::cout << "sequence:" << std::endl << seq;
  /* anchor end */

  //**********************************************************************
  // Drawing rays and layout

  {
    /* anchor layout */
    Io::RendererSvg renderer("layout.svg", 800, 400);

#if 1
    // draw 2d system layout
    sys.draw_2d_fit(renderer);
    sys.draw_2d(renderer);
#else
    // draw 2d layout of lens only
    lens.draw_2d_fit(renderer);
    lens.draw_2d(renderer);
#endif

    Trace::Tracer tracer(sys);

    // trace and draw rays from rays source
    sys.enable_single<Sys::Source>(source_rays);
    tracer.get_trace_result().set_generated_save_state(source_rays);

    tracer.trace();
    tracer.get_trace_result().draw_2d(renderer);
    /* anchor end */
  }

  {
    /* anchor spot */
    sys.enable_single<Sys::Source>(source_point);

    sys.get_tracer_params().set_default_distribution(
      Trace::Distribution(Trace::HexaPolarDist, 12));

    Analysis::Spot spot(sys);

    /* anchor end */
    {
    /* anchor spot */
      Io::RendererSvg renderer("spot.svg", 300, 300, Io::rgb_black);

      spot.draw_diagram(renderer);
    /* anchor end */
    }

    {
    /* anchor spot_plot */
      Io::RendererSvg renderer("spot_intensity.svg", 640, 480);

      ref<Data::Plot> plot = spot.get_encircled_intensity_plot(50);

      plot->draw(renderer);
    /* anchor end */
    }
  }

  {
    /* anchor opd_fan */
    sys.enable_single<Sys::Source>(source_point);

    Analysis::RayFan fan(sys);

    /* anchor end */
    {
    /* anchor opd_fan */
      Io::RendererSvg renderer("opd_fan.svg", 640, 480);

      ref<Data::Plot> fan_plot = fan.get_plot(Analysis::RayFan::EntranceHeight,
                                              Analysis::RayFan::OpticalPathDiff);

      fan_plot->draw(renderer);

    /* anchor end */
    }

    {
    /* anchor transverse_fan */
      Io::RendererSvg renderer("transverse_fan.svg", 640, 480);

      ref<Data::Plot> fan_plot = fan.get_plot(Analysis::RayFan::EntranceHeight,
                                              Analysis::RayFan::TransverseDistance);

      fan_plot->draw(renderer);

    /* anchor end */
    }

    {
    /* anchor longitudinal_fan */
      Io::RendererSvg renderer("longitudinal_fan.svg", 640, 480);

      ref<Data::Plot> fan_plot = fan.get_plot(Analysis::RayFan::EntranceHeight,
                                              Analysis::RayFan::LongitudinalDistance);

      fan_plot->draw(renderer);

    /* anchor end */
    }
  }

  return 0;
}

