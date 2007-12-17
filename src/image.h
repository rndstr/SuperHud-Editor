#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef __WXMAC__
  #include "OpenGL/gl.h"
#else
  #include <GL/gl.h>
#endif

class Image
{
  public:
    Image( const wxString& filepath );
    virtual ~Image();

    void load( const wxString& filepath );




  protected:
    GLuint    m_texid;
};

#endif // __IMAGE_H__
