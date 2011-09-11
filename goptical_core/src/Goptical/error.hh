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


#ifndef GOPTICAL_ERROR_HH_
#define GOPTICAL_ERROR_HH_

#include <string>
#include <stdexcept>

#include "Goptical/common.hh"

namespace _Goptical {

  /** 
      @short Base class used for Goptical exceptions.
      @header Goptical/Error
      @module {Core}
   */

class Error : public std::runtime_error
{
public:
  Error(const std::string &msg = "")
    : std::runtime_error(msg)
  {
  }
};

}

#endif

