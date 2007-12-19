#include "image.h"
#include "common.h"


Texture::Texture( const wxString& filepath )
{
  load(filepath);
}

Texture::~Texture()
{
  if( m_texid )
    glDeleteTextures(1, &m_texid);
}

void Texture::load( const wxString& fpath, int search_where )
{
  char *buf;
  size_t size;
  if( !wxGetApp().pakmanager()->load( &buf, fpath &size) )
  {
    wxLogError(_("Couldn't find/load file: %s"), fpath.c_str());
    return;
  }





}
