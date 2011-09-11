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


#ifndef GOPTICAL_ANALYSIS_SPOT_HH_
#define GOPTICAL_ANALYSIS_SPOT_HH_

#include "Goptical/common.hh"

#include "Goptical/Io/renderer_axes.hh"
#include "Goptical/Math/vector.hh"
#include "Goptical/Data/plot.hh"

#include "Goptical/Analysis/pointimage.hh"

namespace _Goptical
{

  namespace Analysis
  {

    /**
       @short Spot diagram analysis
       @header Goptical/Analysis/Spot
       @module {Core}
       @main

       This class is designed to plot spot diagram and perform
       related analysis.

       @xsee {tuto_spot1, tuto_spot2}
    */
    class Spot : public PointImage
    {
    public:
      Spot(Sys::System &system);

      inline void invalidate();

      /** Get spot maximum radius */
      inline double get_max_radius();

      /** Get spot root mean square radius */
      inline double get_rms_radius();

      /** Get amount of light intensity in the whole spot */
      inline double get_total_intensity();

      /** Get spot centroid */
      inline const Math::Vector3 & get_centroid();

      /** Get spot window center */
      Math::Vector3 get_center();

      /** Get amount of light intensity which falls in given radius from spot center */
      double get_encircled_intensity(double radius);

      /** Get encircled energy plot */
      ref<Data::Plot> get_encircled_intensity_plot(int zones = 100);

      /** Set radius used for diagram drawing and encircled
          plots. Updated with spot max radius on ray trace */
      inline void set_useful_radius(double radius);

      /** draw the rays intersection points only */
      void draw_spot(Io::RendererViewport &renderer);
      /** draw the spot diagram */
      void draw_diagram(Io::RendererViewport &renderer,
                        bool centroid_origin = true);

      /** Get a reference to axes object rendered with spot
          diagram. @ref Io::RendererAxes coordinates are updated on
          ray trace. */
      inline Io::RendererAxes & get_diagram_axes();

    private:
      void process_trace();
      void process_analysis();

      Math::Vector3 _centroid;

      bool      _processed_analysis;
      double    _max_radius;
      double    _rms_radius;
      double    _tot_intensity;
      double    _useful_radius;

      Io::RendererAxes _axes;
    };

  }
}

#endif

