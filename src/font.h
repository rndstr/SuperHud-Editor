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


#ifndef FONT_H
#define FONT_H

#include "common.h"

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
