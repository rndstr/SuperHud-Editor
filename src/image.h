#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#ifdef __WXMAC__
  #include "OpenGL/gl.h"
#else
  #include <GL/gl.h>
#endif

class Texture
{
  public:
    Texture( const wxString& filepath );
    virtual ~Texture();

    bool load( const wxString& fpath );


  protected:
    GLuint    m_texid;
};

#endif // __IMAGE_H__
