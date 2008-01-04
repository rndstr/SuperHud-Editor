// Copyright (C) 2006 Roland Schilter <rolansch@student.ethz.ch>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#include "color4.h"
#include "misc.h"

wxString Color4::to_string() const
{
  switch( type )
  {
  case COLOR_RGBA:
    return she::pretty_print_float( r ) + wxT(" ") +
      she::pretty_print_float( g ) + wxT(" ") +
      she::pretty_print_float( b ) + wxT(" ") +
      she::pretty_print_float( a ) + wxT(" ");
  
  case COLOR_T:
    return wxT("T");

  case COLOR_E:
    return wxT("E");
  }
  return wxT("<error>");
}

wxTextOutputStream& operator<<( wxTextOutputStream& stream, const Color4& c )
{ // pretty print Color4.
  
  stream << c.to_string();
  return stream;
}

bool Color4::from_string( const wxString& str )
{
  float _r, _g, _b, _a;

  // FIXME use wxTextInputStream
  if( 4 != sscanf( str.mb_str(), "%f %f %f %f", &_r, &_g, &_b, &_a ) )
  {
    char special;
    if( 1 != sscanf( str.mb_str(), "%c", &special ) )
      return false;
    else
    {
      if( toupper(special) == 'T' )
        type = COLOR_T;
      else if( toupper(special) == 'E' )
        type = COLOR_E;
      else
        return false;

      return true;
    }
  }
  r = _r;
  g = _g;
  b = _b;
  a = _a;
  type = COLOR_RGBA;
  return true;
}
