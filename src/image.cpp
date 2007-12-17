#include "image.h"


Image::Image( const wxString& filepath )
{
  load(filepath);
}

Image::~Image()
{
  if( m_texid )
    glDeleteTextures(1, &m_texid);
}

void Image::load( const wxString& filepath )
{


}
