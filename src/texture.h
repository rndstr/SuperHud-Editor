#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "common.h"
#include "tgaloader.h"

class Texture
{
  public:
    Texture( const wxString& filepath, int search_where, bool mipmap =false );
    virtual ~Texture() { cleanup(); }

    void cleanup();

    void load( const wxString& fpath, int search_where, bool mipmap = false );

    virtual void glBind();

    static GLuint create_texture( wxImage& img, bool mipmap = false );
    static GLuint create_texture( tgainfo_t *tga, bool mipmap =false );
    

  protected:
    GLuint    m_texid;

    static bool     is_compliantsize( wxInt32 w, wxInt32 h );
    static wxImage  image_makecompliantsize(const wxImage& image);
};

#endif // __TEXTURE_H__
