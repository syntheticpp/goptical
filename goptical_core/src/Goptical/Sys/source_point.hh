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


#ifndef GOPTICAL_SOURCE_POINT_HH_
#define GOPTICAL_SOURCE_POINT_HH_

#include "Goptical/common.hh"

#include "Goptical/Sys/source.hh"

namespace _Goptical {

  namespace Sys {

      /**
         @short Point light source
         @header Goptical/Sys/SourcePoint
         @module {Core}
         @main

         This class implement a point light source. The source can
         either be infinitely far away with a direction vector, or
         located at a given position but without direction.

         A ray is generated for each defined spectrum line for each
         distribution pattern point on target surface.

         Default wavelen list contains a single 550nm entry.
      */

    class SourcePoint : public Source
    {
    public:
      /** Create a point source with given mode. A direction vector
          must be provided when source is in infinity mode. If not in
          infinity mode, a position vector must be provided
          instead. */
      SourcePoint(SourceInfinityMode m, const Math::Vector3 &pos_dir);

      /** Set point source mode to infinity and adjust source direction vector */
      void set_infinity_direction(const Math::Vector3 &dir);

      /** Set point source mode to positioned and adjust source position vector */
      void set_position(const Math::Vector3 &pos);

      /** Change current point source infinity mode */
      inline void set_mode(SourceInfinityMode mode);

    private:

      void generate_rays_simple(Trace::Result &result,
                                const targets_t &entry) const;
      void generate_rays_intensity(Trace::Result &result,
                                   const targets_t &entry) const;

      template <SourceInfinityMode mode>
      inline void get_lightrays_(Trace::Result &result,
                                 const Element &target) const;

      SourceInfinityMode _mode;
    };

  }
}

#endif

