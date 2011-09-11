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


#ifndef GOPTICAL_ANALYSIS_SPOT_HXX_
#define GOPTICAL_ANALYSIS_SPOT_HXX_

#include "Goptical/Io/renderer_axes.hxx"
#include "Goptical/Math/vector.hxx"
#include "Goptical/Data/plot.hxx"

#include "Goptical/Analysis/pointimage.hxx"

  namespace _Goptical
  {

  namespace Analysis
  {

    void Spot::invalidate()
    {
      _processed_trace = false;
      _processed_analysis = false;
    }

    double Spot::get_max_radius()
    {
      process_analysis();

      return _max_radius;
    }

    double Spot::get_rms_radius()
    {
      process_analysis();

      return _rms_radius;
    }

    double Spot::get_total_intensity()
    {
      process_analysis();

      return _tot_intensity;
    }

    const Math::Vector3 & Spot::get_centroid()
    {
      process_analysis();

      return _centroid;
    }

    void Spot::set_useful_radius(double radius)
    {
      _useful_radius = radius;
    }

    Io::RendererAxes & Spot::get_diagram_axes()
    {
      return _axes;
    }

  }
  }

#endif

