#include "color4.h"
#include "misc.h"
#include "superhudeditor.h"

wxString Color4::to_string() const
{
  switch( type )
  {
  case COLOR_RGBA:
    return she::pretty_print_float( r ) + wxT(" ") +
      she::pretty_print_float( g ) + wxT(" ") +
      she::pretty_print_float( b ) + wxT(" ") +
      she::pretty_print_float( a );
  
  case COLOR_T:
    if( wxGetApp().is_q4max() )
      return wxT("T ") + she::pretty_print_float(a);
    return wxT("T");

  case COLOR_E:
    if( wxGetApp().is_q4max() )
      return wxT("E ") + she::pretty_print_float(a);
    return wxT("E");
  case COLOR_W:
    if( wxGetApp().is_q4max() )
      return wxT("W ") + she::pretty_print_float(a);
    return wxT("W");
  }
  return wxT("<error>");
}

wxTextOutputStream& operator<<( wxTextOutputStream& stream, const Color4& c )
{ // pretty print Color4.
  
  stream << c.to_string();
  return stream;
}

#include <wx/sstream.h>
#include <wx/txtstrm.h>

bool Color4::from_string( const wxString& str )
{
  float _r, _g, _b, _a;

  wxStringInputStream sis(str);
  wxTextInputStream tis(sis);

  // FIXME use wxTextInputStream
  /*
  _r = tis.ReadDouble();
  _g = tis.ReadDouble();
  _b = tis.ReadDouble();
  _a = tis.ReadDouble();
  */
  if( 4 != sscanf( str.mb_str(), "%f %f %f %f", &_r, &_g, &_b, &_a ) )
  {
    char special;
    _a = 1.f;
    if( 0 == sscanf( str.mb_str(), "%c %f", &special, &_a ) )
      return false;
    else
    {
      if( toupper(special) == 'T' )
        type = COLOR_T;
      else if( toupper(special) == 'E' )
        type = COLOR_E;
      else if( toupper(special) == 'W' )
        type = COLOR_W;
      else
        return false;
      a = _a;
      return true;
    }
  }
  r = _r;
  g = _g;
  b = _b;
  a = _a;
  type = COLOR_RGBA;
  wxcolisvalid = false;
  return true;
}
