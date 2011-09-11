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


#ifndef GOPTICAL_ANALYSIS_POINTIMAGE_HH_
#define GOPTICAL_ANALYSIS_POINTIMAGE_HH_

#include "Goptical/common.hh"

#include "Goptical/Trace/tracer.hh"
#include "Goptical/Sys/system.hh"

namespace _Goptical
{

  namespace Analysis
  {

    /**
       @short Point image analysis base class
       @header Goptical/Analysis/PointImage
       @module {Core}
       @internal
    */
    class PointImage
    {
    public:
      PointImage(Sys::System &system);
      virtual ~PointImage();

      /** set Image which collect rays for analysis */
      inline void set_image(Sys::Image *image);

      /** return tracer object used for ray tracing. This will
          invalidate current analysis data */
      inline Trace::Tracer & get_tracer();

      /** return tracer object used for ray tracing */
      inline const Trace::Tracer & get_tracer() const;

      /** invalidate current analysis data */
      virtual void invalidate() = 0;

    protected:
      void get_default_image();
      void trace();

      Sys::System &     _system;
      Trace::Tracer     _tracer;
      bool              _processed_trace;
      Sys::Image *      _image;
      const Trace::rays_queue_t *_intercepts;

    private:
    };

  }
}

#endif

