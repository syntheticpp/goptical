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


#ifndef GOPTICAL_DATA_PLOTDATA_HH_
#define GOPTICAL_DATA_PLOTDATA_HH_

#include "Goptical/common.hh"

#include "Goptical/Data/set.hh"
#include "Goptical/Io/rgb.hh"

namespace _Goptical {

  namespace Data {

    /** 
        @short Data set plot
        @header Goptical/Data/PlotData
        @module {Core}
        @main

        This class is used to describe a data set as part of a data
        plot. It contains a pointer to the data set and plotting
        informations (title, color, ...)
     */
    class PlotData
    {
      friend class Plot;

    public:
      /** Create a new data plot descriptor which describe the
          specified dataset. */
      inline PlotData(const const_ref<Set> &s);

      /** Get the described data set */
      inline const Set & get_set() const;

      /** Set data set plotting label */
      inline void set_label(const std::string & title);
      /** Get data set plotting label */
      inline const std::string & get_label() const;

      /** Set data set plotting color */
      inline void set_color(const Io::Rgb & color);
      /** Set data set plotting color */
      inline const Io::Rgb & get_color() const;

      /** Enable a plotting style */
      inline void enable_style(PlotStyleMask style);
      /** Disable a plotting style */
      inline void disable_style(PlotStyleMask style);
      /** Set the plotting style mask */
      inline void set_style(PlotStyleMask style);
      /** Get the plotting style mask */
      inline PlotStyleMask get_style() const;

    private:
      const_ref<Set>    _set;
      Io::Rgb           _color;
      PlotStyleMask     _style;
      std::string       _label;
    };

  }

}

#endif

