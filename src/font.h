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
