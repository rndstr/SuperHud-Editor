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
#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

#ifdef __WXMAC__
  #include "OpenGL/gl.h"
  #include "OpenGL/glu.h" // checkcheck
#else
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

class Texture
{
  public:
    Texture() {}
    Texture( const wxString& filepath, int search_where, bool mipmap =false );
    virtual ~Texture() { cleanup(); }

    void cleanup();

    void load( const wxString& fpath, int search_where, bool mipmap = false );

    virtual void glBind() const;

    static GLuint create_texture( wxImage& img, bool mipmap = false );

    bool is_ok() const { return m_texid != 0; }
    
    wxString name() const { return m_name; }

  protected:
    GLuint    m_texid;
    wxString  m_name;

    static bool     is_compliantsize( wxInt32 w, wxInt32 h );
    static wxImage  image_makecompliantsize(const wxImage& image);
};

#endif
