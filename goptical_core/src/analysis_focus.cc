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


#include <Goptical/Math/VectorPair>
#include <Goptical/Analysis/Focus>
#include <Goptical/Trace/Tracer>
#include <Goptical/Sys/Image>
#include <Goptical/Trace/Ray>
#include <Goptical/Trace/Result>
#include <Goptical/Trace/Distribution>

namespace _Goptical
{

  namespace Analysis
  {

    Focus::Focus(Sys::System &system)
      : PointImage(system),
        _processed_focus(false)
    {
    }

    void Focus::process_focus()
    {
      if (_processed_focus)
        return;

      trace();

      // find beam average vector

      double count = (double)_intercepts->size();
      Math::VectorPair3 beam_avg(Math::Vector3(0, 0, 0),
                                 Math::Vector3(0, 0, 0));

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          Trace::Ray    &ray = **i;

          beam_avg.origin() += ray.get_position();
          beam_avg.direction() += ray.get_direction();
        }

      beam_avg.origin() /= count;
      beam_avg.direction().normalize();

      _best_focus.direction() = beam_avg.direction();

      // find the closest point on the average beam line for each ray

      std::vector<double>       list;

      list.reserve(_intercepts->size());

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          Trace::Ray    &ray = **i;

          const Math::Vector3   &u = beam_avg.direction();
          const Math::Vector3   &v = ray.get_direction();
          const Math::Vector3   w = beam_avg.origin() - ray.get_position();

          double        b = u * v;
          double        c = v * v;
          double        k = (u * u) * c - Math::square(b);

          if (k <= 1e-10)       // skip if beam line and ray almost parallel
            continue;

          double        s = (b * (v * w) - c * (u * w)) / k;

          if (s < 0)            // skip non convergent rays
            continue;

          list.push_back(s);
        }

      if (list.empty())
        throw Error("no convergent ray found for focus analysis");

      double    average = 0;

      GOPTICAL_FOREACH(i, list)
        average += /*square*/(*i);

      average /= (double)list.size();

      //      average = sqrt(average);

      _best_focus.origin() = beam_avg.origin() + beam_avg.direction() * average;

      _processed_focus = true;
    }
  }
}

