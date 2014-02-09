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
