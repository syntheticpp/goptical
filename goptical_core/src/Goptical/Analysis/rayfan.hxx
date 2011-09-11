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


#ifndef GOPTICAL_ANALYSIS_RAYFAN_HXX_
#define GOPTICAL_ANALYSIS_RAYFAN_HXX_

#include "Goptical/Io/renderer_axes.hxx"
#include "Goptical/Math/vector.hxx"
#include "Goptical/Trace/tracer.hxx"
#include "Goptical/Trace/distribution.hxx"
#include "Goptical/Data/plot.hxx"

namespace _Goptical
{

  namespace Analysis
  {
    void RayFan::set_entrance_surface(const Sys::Surface &s)
    {
      _entrance = &s;
    }

    void RayFan::set_target_surface(const Sys::Surface &s)
    {
      _exit = &s;
    }

    void RayFan::set_aberration_plane(enum rayfan_plane_e plane)
    {
      assert(plane == 0 || plane == 1);
      _ab_plane = plane;
    }

    Trace::Distribution & RayFan::get_distribution()
    {
      return _dist;
    }

  }

}

#endif

