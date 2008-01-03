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


#ifndef FONT_H
#define FONT_H

#include <wx/string.h>
#include <wx/bitmap.h>

#include <wx/glcanvas.h>

const int F_COUNT_X = 16;
const int F_COUNT_Y = 16;
const int F_CHAR_WIDTH = 32; ///< texturewidth / F_COUNT_X
const int F_CHAR_HEIGHT = 32; ///< textureheight / F_COUNT_Y


class IFont
{
  public:
    IFont( const wxString& name );
    virtual ~IFont() { }

    /// loads the chardwidths.
    virtual bool load();
    virtual void print( const wxRect& r, int sizept, const wxString& msg, bool monospace = false, char textalign = 'L' ) const = 0;
    virtual void print( const wxRect& r, int sizex, int sizey, const wxString& msg, bool monospace = false, char textalign = 'L' ) const = 0;
    virtual void cleanup() = 0;

    /// calculates the width in coordinate space.
    virtual int get_width( int sizex, const wxString& msg, bool monospace ) const = 0;

  protected:
    wxString    m_name;
    unsigned char m_widths[F_COUNT_X * F_COUNT_Y];
    
};

class Texture;

class CPMAFont : public IFont
{
  public:
    CPMAFont( const wxString& name );
    virtual ~CPMAFont() { cleanup(); }

    virtual bool load();
    virtual void print( const wxRect& r, int sizept, const wxString& msg, bool monospace = false, char textalign = 'L' ) const;
    virtual void print( const wxRect& r, int sizex, int sizey, const wxString& msg, bool monospace = false, char textalign = 'L' ) const;
    virtual void cleanup();

    
    int       get_width( int sizex, const wxString& msg, bool monospace ) const;
  protected:
    Texture   *m_tex;
};


#endif // FONT_H
