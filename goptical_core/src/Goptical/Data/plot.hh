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


#ifndef GOPTICAL_DATA_PLOT_HH_
#define GOPTICAL_DATA_PLOT_HH_

#include <string>

#include "Goptical/common.hh"

#include "Goptical/Io/renderer_axes.hh"
#include "Goptical/Io/rgb.hh"
#include "Goptical/Data/plotdata.hh"
#include "Goptical/Math/vector.hh"

namespace _Goptical {

  namespace Data {

    /**
       @short Data plots container
       @header Goptical/Data/Plot
       @module {Core}
       @main

       This class is used to describe a data plot. It contains a list
       of @ref PlotData objects and describes some plot properties
       (title, range, ...).

       Plots can be built from data sets or obtained directly from
       various analysis functions. They can be rendered on a @ref
       Io::RendererViewport object with the @ref draw function.
     */
    class Plot : public ref_base<Plot>
    {
    public:
      /** Create a new empty plot */
      Plot();

      /** Create and add plot data from specified data set. */
      PlotData & add_plot_data(const Set &data,
                               const Io::Rgb &color = Io::rgb_red,
                               const std::string &label = "data",
                               PlotStyleMask style = InterpolatePlot | PointPlot);

      /** Add plot data */
      void add_plot_data(PlotData &data);

      /** Discard all plot data set */
      void erase_plot_data();

      /** Get plot data set count */
      inline unsigned int get_plot_count() const;

      /** Get plot data set at given index */
      inline PlotData & get_plot_data(unsigned int index);

      /** Get plot data set at given index */
      inline const PlotData & get_plot_data(unsigned int index) const;

      /** Set plot main title */
      void set_title(const std::string & title);

      /** Get plot main title */
      inline const std::string & get_title() const;

      /** Set color for all plots */
      void set_color(const Io::Rgb & color);

      /** Automatically choose different colors for each plot */
      void set_different_colors();

      /** Set plot style for all plot */
      void set_style(PlotStyleMask style);

      /** Swap x and y axis for 2d plots */
      inline void set_xy_swap(bool doswap);

      /** Get x and y axis swap state for 2d plots */
      inline bool get_xy_swap() const;

      /** Set axis position to dataset range */
      void fit_axes_range();

      /** Get plot axes object */
      inline Io::RendererAxes & get_axes();

      /** Get plot axes object */
      inline const Io::RendererAxes & get_axes() const;

      /** Get data sets dimensions, return 0 if inconsistent */
      unsigned int get_dimensions() const;

      /** Get range of x data in sets */
      Math::range_t get_x_data_range(unsigned int dimension = 0) const;

      /** Get range of y data in sets */
      Math::range_t get_y_data_range() const;

      /** draw */
      void draw(Io::RendererViewport &r);

    private:

      double get_tic_step(const Math::range_t &r) const;

      std::string       _title;
      std::vector<PlotData>     _plots;

      Io::RendererAxes  _axes;

      bool              _xy_swap;
    };
  }
}

#endif

