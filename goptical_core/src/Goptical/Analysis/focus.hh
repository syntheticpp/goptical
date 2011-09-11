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


#ifndef GOPTICAL_ANALYSIS_FOCUS_HH_
#define GOPTICAL_ANALYSIS_FOCUS_HH_

#include "Goptical/common.hh"

#include "Goptical/Math/vector_pair.hh"

#include "Goptical/Analysis/pointimage.hh"

namespace _Goptical
{

  namespace Analysis
  {

    /**
       @short Best focus point analysis
       @header Goptical/Analysis/Focus
       @module {Core}
       @main

       This class is designed to find the best point of focus of
       an optical system.
    */
    class Focus : public PointImage
    {
    public:
      Focus(Sys::System &system);

      inline void invalidate();

      /** Get best point of focus in system global coordinates. */
      inline const Math::VectorPair3 & get_best_focus();

    private:
      void process_focus();

      bool              _processed_focus;
      Math::VectorPair3 _best_focus;
    };

  }
}

#endif

