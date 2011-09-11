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


#ifndef GOPTICAL_TRACER_HXX_
#define GOPTICAL_TRACER_HXX_

#include "Goptical/Trace/result.hh"

namespace _Goptical {

  namespace Trace {

    void Tracer::set_trace_result(Result &res)
    {
      _result_ptr = &res;
    }

    Trace::Result & Tracer::get_trace_result() const
    {
      return *_result_ptr;
    }

    Trace::Result & Tracer::set_default_trace_result()
    {
      return *(_result_ptr = &_result);
    }

    const Sys::System & Tracer::get_system() const
    {
      return *_system;
    }

    const Params & Tracer::get_params() const
    {
      return _params;
    }

    Params & Tracer::get_params()
    {
      return _params;
    }

  }
}

#endif

