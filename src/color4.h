// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.



#ifndef COLOR4_H
#define COLOR4_H

#include "misc.h"

#include <wx/colour.h>
#include <wx/txtstrm.h>
#include <wx/glcanvas.h>

typedef enum 
{
  COLOR_NONE,
  COLOR_RGBA,
  COLOR_T,
  COLOR_E,
  COLOR_W // q4max
} eColorType;


/// A color class including alpha channel (percentage!) and conversion function to wxColour.
/// All color information (RGBA) is stored as float [0-1]
///
/// There is a conversion problem when we get it as wxColour and then set it again.
/// So whenever we get the value we actually convert it, but only set it if it's different
/// to the get oO
class Color4
{
  public:
    explicit Color4( float _r, float _g, float _b, int _a_percent=100 ) :
        r(_r), g(_g), b(_b), a(_a_percent), type(COLOR_RGBA), wxcolisvalid(false)
    {
      set_a100(_a_percent);
    }
    /*
    explicit Color4( const wxColour& col ) :
        r(col.Red()/255.f), g(col.Green()/255.f), b(col.Blue()/255.f), a(1.f), type(COLOR_RGBA) {}
    Color4( const Color4& c4 ) :
      r(c4.r), g(c4.g), b(c4.b), type(c4.type) {}
      */
    Color4() :
        r(1.f), g(1.f), b(1.f), a(1.f), type(COLOR_RGBA), wxcolisvalid(false) {}

    ~Color4() {}


    void set_type(int t) { type = t; }
    int get_type() const { return type; }
    bool is_special() const { return (type == COLOR_T || type == COLOR_E || type == COLOR_W); }
    bool is_special_t() const { return (type == COLOR_T); }
    bool is_special_e() const { return (type == COLOR_E); }
    bool is_special_w() const { return (type == COLOR_W); }
    bool is_rgba() const { return (type == COLOR_RGBA); }

    int a255() const
    {
      return static_cast<int>( a*255.f );
    }
    int a100() const
    {
      return static_cast<int>(a*100.f);
    }

    float r1() const { return r; }
    unsigned char r255() const { return static_cast<unsigned char>(r*255.f); }
    float g1() const { return g; }
    unsigned char g255() const { return static_cast<unsigned char>(g*255.f); }
    float b1() const { return b; }
    unsigned char b255() const { return static_cast<unsigned char>(b*255.f); }
    float a1() const { return a; }
    void set_a1(float _a) { a = _a; }


    /// @return True if the color was different than already set
    bool set(const wxColour& col)
    {
      if( wxcolisvalid && col == tmpwxcol )
        return false; // it's already fine.. this is important to happen if there shouldn't be any conversion errors
      wxcolisvalid = true;
      tmpwxcol = col;
      r = col.Red()/255.f;
      g = col.Green()/255.f;
      b = col.Blue()/255.f;
      return true;
    }

    bool set(const wxString& str )
    {
      return from_string(str);
    }

    void set_a100( int _a_percent )
    { 
      she::clamp(_a_percent, 0, 100);
      a = _a_percent/100.f;
    }

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
      case COLOR_W:
        glColor4f( 1.f, 1.f, 1.f, a );
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

    wxColour to_wxColour() const
    { 
      if( !wxcolisvalid )
      {
        wxcolisvalid = true;
        tmpwxcol = wxColour(static_cast<unsigned char>(r*255.f), static_cast<unsigned char>(g*255.f), static_cast<unsigned char>(b*255.f));
      }
      return tmpwxcol;
    }
//    operator wxColour() const { return wxColour(static_cast<unsigned char>(r*255.f), static_cast<unsigned char>(g*255.f), static_cast<unsigned char>(b*255.f)); }
    
  protected:
    float         r, g, b, a;
    int           type;
    mutable bool          wxcolisvalid;
    mutable wxColour      tmpwxcol; ///< whenever 
};

wxTextOutputStream& operator<<( wxTextOutputStream& stream, const Color4& c );

#endif

