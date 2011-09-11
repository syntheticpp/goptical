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

#include <sstream>

#include <Goptical/Io/RendererViewport>

#include <Goptical/Math/Triangle>
#include <Goptical/Math/VectorPair>

#include <Goptical/Data/Set1d>
#include <Goptical/Data/Grid>
#include <Goptical/Data/PlotData>
#include <Goptical/Data/Plot>
#include <Goptical/Io/RendererAxes>

namespace _Goptical {

  namespace Io {

    RendererViewport::RendererViewport()
      : _margin_type(MarginRatio),
        _margin(Math::VectorPair2(0.13, 0.13, 0.13, 0.13)),
        _rows(1), _cols(1),
        _pageid(0),
        _fov(45.),
        _precision(3),
        _format(std::ios::floatfield)
    {
    }

    void RendererViewport::set_2d_size(double width, double height)
    {
      _2d_output_res = Math::Vector2(width, height);
    }

    void RendererViewport::set_window(const Math::Vector2 &center, const Math::Vector2 &size, bool keep_aspect)
    {
      Math::Vector2 s(size);

      if (keep_aspect)
        {
          double out_ratio = (_2d_output_res.x() / _cols) / (_2d_output_res.y() / _rows);
          if (fabs(s.x() / s.y()) < out_ratio)
            s.x() = s.y() * out_ratio;
          else
            s.y() = s.x() / out_ratio;
        }

      _window2d_fit = Math::VectorPair2(center - s / 2., center + s / 2.);

      Math::VectorPair2 ms(s / 2, s / 2);

      switch (_margin_type)
        {
        case MarginLocal:
          ms[0] = ms[0] + _margin[0];
          ms[1] = ms[1] + _margin[1];
          break;
        case MarginRatio:
          ms[0] = ms[0] + s.mul(_margin[0]);
          ms[1] = ms[1] + s.mul(_margin[1]);
          break;
        case MarginOutput:
          ms[0] = ms[0] / (Math::vector2_1 - _margin[0] / _2d_output_res * 2);
          ms[1] = ms[1] / (Math::vector2_1 - _margin[1] / _2d_output_res * 2);
          break;
        }

      _window2d = Math::VectorPair2(center - ms[0], center + ms[1]);

      update_2d_window();
      set_orthographic();
      set_page(_pageid);
    }

    void RendererViewport::update_2d_window()
    {
    }

    void RendererViewport::set_window(const Math::Vector2 &center, double radius, bool keep_aspect)
    {
      Math::Vector2     size(radius);

      set_window(center, size, keep_aspect);
    }

    void RendererViewport::set_window(const Math::VectorPair2 &window, bool keep_aspect)
    {
      Math::Vector2     center((window[0] + window[1]) / 2);
      Math::Vector2     size(window[1].x() - window[0].x(),
                             window[1].y() - window[0].y());

      set_window(center, size, keep_aspect);
    }

    void RendererViewport::set_page_layout(unsigned int cols, unsigned int rows)
    {
      _cols = cols;
      _rows = rows;
      set_page(0);
    }

    void RendererViewport::set_page(unsigned int page)
    {
      if (page >= _cols * _rows)
        throw Error("set_page: no such page number in current layout");

      _pageid = page;
      int row = page / _cols;
      int col = page % _cols;

      Math::Vector2     size(_window2d[1].x() - _window2d[0].x(),
                             _window2d[1].y() - _window2d[0].y());

      Math::Vector2   a(_window2d.x0() - size.x() * col,
                        _window2d.y0() - size.y() * (_rows - 1 - row));

      Math::Vector2   b(a.x() + size.x() * _cols,
                        a.y() + size.y() * _rows);

      _page = Math::VectorPair2(a, b);
    }

    void RendererViewport::draw_pages_grid(const Rgb &rgb)
    {
      Math::Vector2     size(_window2d[1].x() - _window2d[0].x(),
                             _window2d[1].y() - _window2d[0].y());

      for (unsigned int x = 0; x < _cols; x++)
        for (unsigned int y = 0; y < _rows; y++)
          {
            if (y)
              draw_segment(Math::VectorPair2(_page.x0(), _page.y0() + size.y() * y,
                                             _page.x1(), _page.y0() + size.y() * y));

            if (x)
              draw_segment(Math::VectorPair2(_page.x0() + size.x() * x, _page.y0(),
                                             _page.x0() + size.x() * x, _page.y1()));
          }
    }

    void RendererViewport::set_margin(double left, double bottom, double right, double top)
    {
      _margin_type = MarginLocal;
      _margin = Math::VectorPair2(left, bottom, right, top);
      set_window(_window2d_fit, false);
    }

    void RendererViewport::set_margin_ratio(double left, double bottom, double right, double top)
    {
      _margin_type = MarginRatio;
      _margin = Math::VectorPair2(left, bottom, right, top);
      set_window(_window2d_fit, false);
    }

    void RendererViewport::set_margin_output(double left, double bottom, double right, double top)
    {
      _margin_type = MarginOutput;
      _margin = Math::VectorPair2(left, bottom, right, top);
      set_window(_window2d_fit, false);
    }

    void RendererViewport::set_camera_direction(const Math::Vector3 &dir)
    {
      Math::Transform<3> t(get_camera_transform());
      t.set_direction(dir);
      set_camera_transform(t);
    }

    void RendererViewport::set_camera_position(const Math::Vector3 &pos)
    {
      Math::Transform<3> t(get_camera_transform());
      t.set_translation(pos);
      set_camera_transform(t);
    }

    void RendererViewport::clear()
    {
    }

    void RendererViewport::flush()
    {
    }

    /**********************************************************************
     * Plot drawing
     */

    void RendererViewport::set_2d_plot_window(const Data::Plot &plot)
    {
      Math::range_t x_range = plot.get_axes()._axes[0]._range;

      if (x_range.first == x_range.second)
        x_range = plot.get_x_data_range();

      Math::range_t y_range = plot.get_axes()._axes[1]._range;

      if (y_range.first == y_range.second)
        y_range = plot.get_y_data_range();

      set_window(Math::VectorPair2(Math::Vector2(x_range.first, y_range.first),
                                   Math::Vector2(x_range.second, y_range.second)), false);
    }

    void RendererViewport::draw_plot(const Data::Plot &plot)
    {
      switch (plot.get_dimensions())
        {
        case 1:

          set_2d_plot_window(plot);
          draw_axes<2>(plot.get_axes());

          // plot title
          draw_text(Math::Vector2((_window2d[0].x() + _window2d[1].x()) / 2.,
                                  (_window2d_fit[1].y() + _window2d[1].y()) / 2.),
                    Math::vector2_10, plot.get_title(),
                    TextAlignCenter | TextAlignMiddle,
                    18, get_style_color(StyleForeground));

          // plot data
          for (unsigned int i = 0; i < plot.get_plot_count(); i++)
            {
              const Data::PlotData &d = plot.get_plot_data(i);
              draw_plot_data_2d(static_cast<const Data::Set1d&>(d.get_set()), d);
            }

          break;
        }

    }

    void RendererViewport::draw_frame_2d()
    {
      Math::Vector2 fr[4];

      fr[0] = _window2d_fit[0];
      fr[1] = Math::Vector2(_window2d_fit[0].x(), _window2d_fit[1].y());
      fr[2] = _window2d_fit[1];
      fr[3] = Math::Vector2(_window2d_fit[1].x(), _window2d_fit[0].y());

      draw_polygon(fr, 4, get_style_color(StyleForeground), false, true);
    }

    void RendererViewport::draw_plot_data_2d(const Data::Set1d &data, const Data::PlotData &style)
    {
      // spline interpolated curve between points

      if (style.get_style() & Data::InterpolatePlot)
        {
          const double x_step = (_window2d[1].x() - _window2d[0].x()) / _2d_output_res.x();
          Math::range_t xr = data.get_x_range();
          double x_low = std::max(_window2d_fit[0].x(), xr.first);
          double x_high = std::min(_window2d_fit[1].x(), xr.second);
          double y1 = data.interpolate(x_low);

          for (double x = x_low + x_step;
               x < x_high + x_step / 2;
               x += x_step)
            {
              double y2 = data.interpolate(x);

              draw_segment(Math::VectorPair3(Math::Vector3(x - x_step, y1, 0),
                                             Math::Vector3(x, y2, 0)),
                           style.get_color());

              y1 = y2;
            }
        }

      // line plot

      if (style.get_style() & Data::LinePlot)
        {
          std::pair<double, double> p1(data.get_x_value((unsigned)0), data.get_y_value((unsigned)0));

          for (unsigned int j = 1; j < data.get_count(); j++)
            {
              std::pair<double, double> p2(data.get_x_value(j), data.get_y_value(j));

              draw_segment(Math::VectorPair3(Math::Vector3(p1.first, p1.second, 0),
                                             Math::Vector3(p2.first, p2.second, 0)),
                           style.get_color());

              p1 = p2;
            }
        }

      // draw cross tic for each point

      if (style.get_style() & Data::PointPlot)
        {
          for (unsigned int j = 0; j < data.get_count(); j++)
            {
              Math::Vector3 p(data.get_x_value(j), data.get_y_value(j), 0);

              draw_point(p, style.get_color(), PointStyleCross);
            }
        }

      // print value for each point

      if (style.get_style() & Data::ValuePlot)
        {
          for (unsigned int j = 0; j < data.get_count(); j++)
            {
              int a;
              // FIXME remove use of data pair
              std::pair<double, double> p(data.get_x_value(j), data.get_y_value(j));

              double prev = j > 0 ? data.get_y_value(j-1) : p.second;
              double next = j + 1 < data.get_count() ? data.get_y_value(j+1) : p.second;

              if (p.second > prev) // FIXME use derivative to find best text position
                {
                  if (p.second > next)
                    a = TextAlignBottom | TextAlignCenter;
                  else
                    a = TextAlignBottom | TextAlignRight;
                }
              else
                {
                  if (p.second > next)
                    a = TextAlignTop | TextAlignRight;
                  else
                    a = TextAlignBottom | TextAlignLeft;
                }

              std::ostringstream s;

#if 0 // FIXME
              s.setf(_format);
              s.precision(_precision);
#endif
              s << p.second;

              draw_text(Math::Vector2(p.first, p.second),
                        Math::vector2_10, s.str(),
                        (TextAlignMask)a, 12, style.get_color());
            }
        }

    }

    void RendererViewport::draw_plot_data_3d(const Data::Grid &data, const Data::PlotData &style)
    {
      static const double epsilon = 1e-8;
      // FIXME reduce resolution if too high
      double res = get_feature_size();

      if (style.get_style() & Data::InterpolatePlot)
        {
          Math::range_t xr = data.get_x_range(0);
          Math::range_t yr = data.get_x_range(1);

          double xs = (xr.second - xr.first) / ((data.get_count(0) - 1) * res);
          double ys = (yr.second - yr.first) / ((data.get_count(1) - 1) * res);

          for (double x = xr.first; x <= xr.second; x += xs)
            for (double y = yr.first; y <= yr.second; y += ys)
              {
                Math::Vector3 p(x, y, data.interpolate(Math::Vector2(x, y)));

                draw_point(p, Rgb(0, 1, 0), PointStyleCross);
              }
        }

      if (style.get_style() & Data::Filled)
        {
          Math::range_t xr = data.get_x_range(0);
          Math::range_t yr = data.get_x_range(1);

          double xs = (xr.second - xr.first) / ((data.get_count(0) - 1) * res);
          double ys = (yr.second - yr.first) / ((data.get_count(1) - 1) * res);

          for (double x = xr.first; x < xr.second - epsilon; x += xs)
            for (double y = yr.first; y < yr.second - epsilon; y += ys)
              {
                Math::Vector3 g11(data.interpolate_deriv(Math::Vector2(x, y)), -1);
                Math::Vector3 g12(data.interpolate_deriv(Math::Vector2(x, y + ys)), -1);
                Math::Vector3 g21(data.interpolate_deriv(Math::Vector2(x + xs, y)), -1);
                Math::Vector3 g22(data.interpolate_deriv(Math::Vector2(x + xs, y + ys)), -1);

#if 0
                Math::Vector3 p11(x, y, data.interpolate(Math::Vector2(x, y)));
                Math::Vector3 p12(x, y + ys, data.interpolate(Math::Vector2(x, y + ys)));
                Math::Vector3 p21(x + xs, y, data.interpolate(Math::Vector2(x + xs, y)));
                Math::Vector3 p22(x + xs, y + ys, data.interpolate(Math::Vector2(x + xs, y + ys)));
#else
                Math::Vector3 p11(x, y, g11.y() * 10);
                Math::Vector3 p12(x, y + ys, g12.y() * 10);
                Math::Vector3 p21(x + xs, y, g21.y() * 10);
                Math::Vector3 p22(x + xs, y + ys, g22.y() * 10);
#endif
                draw_triangle(Math::Triangle<3>(p11, p12, p22), Math::Triangle<3>(g11, g12, g22), Rgb(0, 1, 0));
                draw_triangle(Math::Triangle<3>(p22, p21, p11), Math::Triangle<3>(g22, g21, g11), Rgb(0, 1, 0));
              }
        }

      if (style.get_style() & Data::LinePlot)
        {
          unsigned int x, y;

          for (x = 0; x < data.get_count(0); x++)
            {
              Math::Vector3 p1(data.get_x_value(x, 0),
                               data.get_x_value(0, 1),
                               data.get_y_value(x, 0));

              for (y = 1; y < data.get_count(1); y++)
                {
                  Math::Vector3 p2(data.get_x_value(x, 0),
                                   data.get_x_value(y, 1),
                                   data.get_y_value(x, y));

                  draw_segment(Math::VectorPair3(p1, p2), Rgb(1, 1, 1));

                  p1 = p2;
                }
            }

          for (y = 0; y < data.get_count(0); y++)
            {
              Math::Vector3 p1(data.get_x_value(0, 0),
                               data.get_x_value(y, 1),
                               data.get_y_value(0, y));

              for (x = 1; x < data.get_count(1); x++)
                {
                  Math::Vector3 p2(data.get_x_value(x, 0),
                                   data.get_x_value(y, 1),
                                   data.get_y_value(x, y));

                  draw_segment(Math::VectorPair3(p1, p2), Rgb(1, 1, 1));

                  p1 = p2;
                }
            }
        }

      if (style.get_style() & Data::PointPlot)
        {
          for (unsigned int x = 0; x < data.get_count(0); x++)
            for (unsigned int y = 0; y < data.get_count(1); y++)
              {
                Math::Vector3 p(data.get_x_value(x, 0),
                                data.get_x_value(y, 1),
                                data.get_y_value(x, y));

                draw_point(p, Rgb(1, 0, 1), PointStyleCross);
              }
        }
    }

    void RendererViewport::draw_axes_2d(const RendererAxes &a)
    {
      draw_axes<2>(a);
    }

    void RendererViewport::draw_axes_3d(const RendererAxes &a)
    {
      //      draw_axes<3>(a, range);
    }

    template <int N>
    void RendererViewport::draw_axes(const RendererAxes &a)
    {
      Math::Vector<N> p(a.get_position(), 0);
      int pow10, max[N], min[N];
      double step[N];

      if (a._frame && N == 2)
        draw_frame_2d();

      for (unsigned int i = 0; i < N; i++)
        {
          const RendererAxes::Axis &ax = a._axes[i];
          Math::range_t r(_window2d_fit[0][i], _window2d_fit[1][i]);

          double s = step[i] = fabs(a.get_tics_step(i, r));

          min[i] = trunc((r.first - p[i]) / s);
          max[i] = trunc((r.second - p[i]) / s);

          pow10 = ax._pow10_scale ? (int)floor(log10(s)) : 0;

          std::string si_unit;

          if (ax._si_prefix)
            {
              static const char *sc[] = { "y", "z", "a", "f", "p", "n", "u", "m",
                                          "", "k", "M", "G", "T", "P", "E", "Z", "Y" };
              int u = (24 + pow10 + ax._pow10) / 3;
              if (u >= 0 && u < 17)
                {
                  si_unit = std::string(sc[u]) + ax._unit;
                  pow10 = (u - 8) * 3 - ax._pow10;
                }
            }

          Math::Vector2 lp;
          Math::Vector2 ld;

          switch (i)
            {
            case 0:
              lp = Math::Vector2((_window2d[0].x() + _window2d[1].x()) / 2.,
                                 (_window2d_fit[0].y() * .50 + _window2d[0].y() * 1.50) / 2.);
              ld = Math::vector2_10;
              break;
            case 1:
              lp = Math::Vector2((_window2d_fit[0].x() * .50 + _window2d[0].x() * 1.50) / 2.,
                                 (_window2d[0].y() + _window2d[1].y()) / 2.);
              ld = Math::vector2_01;
              break;
            }

          // axis label
          {
            std::string lx = ax._label;
            bool useunit = !ax._unit.empty();
            bool usep10 = pow10 != 0;

            if (!si_unit.empty())
              lx += " (" + si_unit + ")";
            else if (useunit || usep10)
              {
                lx += " (";
                if (usep10)
                  lx += fstring("x10^%i", pow10);
                if (useunit && usep10)
                  lx += " ";
                if (useunit)
                  lx += ax._unit;
                lx += ")";                  
              }

            draw_text(lp, ld, lx,
                      TextAlignCenter | TextAlignMiddle,
                      12, get_style_color(StyleForeground));
          }

          // skip out of range axis
          bool oor = false;
          for (unsigned int j = 0; j < N; j++)
            oor |= (j != i &&
                    ((p[j] <= std::min(_window2d_fit[0][j], _window2d_fit[1][j])) ||
                     (p[j] >= std::max(_window2d_fit[0][j], _window2d_fit[1][j]))));

          // draw axis
          if (!oor && ax._axis)
            {
              Math::VectorPair<N> seg(p, p);
              seg[0][i] = r.first;
              seg[1][i] = r.second;

              draw_segment(seg, get_style_color(StyleForeground));
            }

          // draw tics on axis
          if (ax._tics && (ax._axis || a._frame))
            {
              for (int j = min[i]; j <= max[i]; j++)
                draw_axes_tic<N>(a, i, pow10, oor, j * s);
            }
        }
         
      if (a._grid)
        {
          // draw grid
          for (int x = min[0]; x <= max[0]; x++)
            for (int y = min[1]; y <= max[1]; y++)
              {
                switch (N)
                  {
                  case 3:
                    for (int z = min[2]; z <= max[2]; z++)
                      draw_point(Math::Vector3(p[0]+x*step[0], p[1]+y*step[1], p[2]+z*step[2]),
                                 get_style_color(StyleForeground));
                    break;

                  case 2:
                    draw_point(Math::Vector2(p[0]+x*step[0], p[1]+y*step[1]),
                               get_style_color(StyleForeground));
                    break;
                  }
              }
        }
    }

    template <int N>
    void RendererViewport::draw_axes_tic(const RendererAxes &a, unsigned int i,
                                         int pow10, bool oor, double x)
    {
      Math::Vector<N> p(a.get_position(), 0);
      const RendererAxes::Axis &ax = a._axes[i];
      Math::Vector<N> vtic;

      if (!oor && ax._axis)
        {
          vtic = p;
          vtic[i] = x + p[i];
          draw_point(vtic, get_style_color(StyleForeground), PointStyleCross);
        }

      if (a._frame)
        {
          vtic = _window2d_fit[1];
          vtic[i] = x + p[i];
          draw_point(vtic, get_style_color(StyleForeground), PointStyleCross);

          vtic = _window2d_fit[0];
          vtic[i] = x + p[i];
          draw_point(vtic, get_style_color(StyleForeground), PointStyleCross);
        }

      // draw tic value text
      if (ax._values)
        {
          static const TextAlignMask align[3] =
            {
              TextAlignCenter | TextAlignTop,
              TextAlignRight | TextAlignMiddle,
              TextAlignTop | TextAlignCenter,
            };

          std::ostringstream s;

          s.setf(_format);
          s.precision(_precision);

          s << (x + p[i] - a._origin[i]) / pow(10., pow10);

          switch (N)
            {
            case 3:
              // draw_text(vtic, (seg[1] - seg[0]).normalized(), s.str(),
              //           align[i], 12, get_style_color(StyleForeground));
              break;
            case 2:
              draw_text(vtic, Math::vector2_10, s.str(),
                        align[i], 12, get_style_color(StyleForeground));
              break;
            }
        }
    }

  }

}

