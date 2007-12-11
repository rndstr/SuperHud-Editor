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


#ifndef __COLOR4_H__
#define __COLOR4_H__

#include <wx/colour.h>
#include <wx/txtstrm.h>
#include <wx/glcanvas.h>

typedef enum 
{
  COLOR_NONE,
  COLOR_RGBA,
  COLOR_T,
  COLOR_E
} eColorType;

/// A color class including alpha channel (percentage!) and conversion function to wxColour.
/// All color information (RGBA) is stored as float [0-1]
class Color4
{
  public:
    explicit Color4( float _r, float _g, float _b, float _a_percent=100 ) :
        r(_r), g(_g), b(_b), a(_a_percent), type(COLOR_RGBA) {a = (a>100 ? 100:a);}
    explicit Color4( const wxColour& col ) :
        r(col.Red()/255.f), g(col.Green()/255.f), b(col.Blue()/255.f), a(1.f), type(COLOR_RGBA) {}
    Color4() :
        r(1.f), g(1.f), b(1.f), a(1.f), type(COLOR_RGBA) {}

    ~Color4() {}


    void set_type(int t) { type = t; }
    int get_type() const { return type; }
    bool is_special() const { return (type == COLOR_T || type == COLOR_E); }
    bool is_special_t() const { return (type == COLOR_T); }
    bool is_special_e() const { return (type == COLOR_E); }
    bool is_rgba() const { return (type == COLOR_RGBA); }

    unsigned char a255() const
    {
      return static_cast<unsigned char>( a*255.f );
    }
    unsigned char a100() const
    {
      return static_cast<unsigned char>(a*100.f);
    }

    float r1() const { return r; }
    unsigned char r255() const { return static_cast<unsigned char>(r*255.f); }
    float g1() const { return g; }
    unsigned char g255() const { return static_cast<unsigned char>(g*255.f); }
    float b1() const { return b; }
    unsigned char b255() const { return static_cast<unsigned char>(b*255.f); }
    float a1() const { return a; }
    void set_a1(float _a) { a = _a; }


    void set(const wxColour& col)
    {
      r = col.Red()/255.f;
      g = col.Green()/255.f;
      b = col.Blue()/255.f;
    }

    void set_a100( unsigned char _a_percent ) { a = (_a_percent>100 ? 1.f:_a_percent/100.f); }

    void glBind() const
    {
      switch( type )
      {
      case COLOR_T:
        glColor4f( 1.f, .2f, .2f, a );
        break;
      case COLOR_E:
        glColor4f( .2f, .2f, 1.f, a );
        break;
      case COLOR_RGBA:
      default:
        glColor4f( r, g, b, a );
      }
    }

    wxString to_string() const;

    bool from_string( const wxString& str );

    /*
    float r_norm() const { return r; }
    float g_norm() const { return g/255.f; }
    float b_norm() const { return b/255.f; }
    float a_norm() const { return a/100.f; }
    */

    wxColour to_wxColour() const { return wxColour(static_cast<unsigned char>(r*255.f), static_cast<unsigned char>(g*255.f), static_cast<unsigned char>(b*255.f)); }
    operator wxColour() const { return wxColour(static_cast<unsigned char>(r*255.f), static_cast<unsigned char>(g*255.f), static_cast<unsigned char>(b*255.f)); }
    
  protected:
    float         r, g, b, a;
    int           type;
};

wxTextOutputStream& operator<<( wxTextOutputStream& stream, const Color4& c );

#endif

