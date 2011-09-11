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


#ifndef GOPTICAL_DATA_PLOTDATA_HXX_
#define GOPTICAL_DATA_PLOTDATA_HXX_

#include "Goptical/Data/set.hxx"

namespace _Goptical {

  namespace Data {

    PlotData::PlotData(const const_ref<Set> &s)
      : _set(s),
        _color(Io::rgb_red),
        _style(InterpolatePlot | PointPlot),
        _label("")
    {
    }

    const Set & PlotData::get_set() const
    {
      return *_set;
    }

    void PlotData::set_label(const std::string & label)
    {
      _label = label;
    }

    const std::string & PlotData::get_label() const
    {
      return _label;
    }

    void PlotData::set_color(const Io::Rgb & color)
    {
      _color = color;
    }

    const Io::Rgb & PlotData::get_color() const
    {
      return _color;
    }

    void PlotData::enable_style(PlotStyleMask style)
    {
      _style = _style | style;
    }

    void PlotData::disable_style(PlotStyleMask style)
    {
      _style = _style & ~style;
    }

    void PlotData::set_style(PlotStyleMask style)
    {
      _style = style;
    }

    PlotStyleMask PlotData::get_style() const
    {
      return _style;
    }
  }
}

#endif

