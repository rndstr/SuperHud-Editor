#include "texture.h"
#include "common.h"
#include "pakmanagerbase.h"
#include <wx/mstream.h>
#include "tgaloader.h"


Texture::Texture( const wxString& filepath, int search_where, bool mipmap /*=false*/ ) :
  m_texid(0)
{
  load(filepath, search_where, mipmap);
}


void Texture::cleanup()
{
  if( m_texid )
    glDeleteTextures(1, &m_texid);
}

void Texture::load( const wxString& fpath, int search_where, bool mipmap /*=false*/ )
{
  cleanup();
  m_texid = 0;

  wxLogDebug(wxT("Loading texture: ") + fpath);

  char *buf;
  size_t size;
  if( !wxGetApp().pakmanager()->load( &buf, fpath, search_where, &size ) )
  {
    wxLogError(_("Couldn't find/load file: %s"), fpath.c_str());
    return;
  }
  if( file_ext( fpath ) == wxT("tga") )
  { // wx does not support loading tga *sigh*
    tgainfo_t tga;
    if( !load_tga( (unsigned char*)buf, &tga, size ) )
    {
      wxLogError(_("Failed loading tga image: %s"), fpath.c_str());
      return;
    }

    m_texid = Texture::create_texture( &tga, mipmap );
    delete [] tga.data;
  }
  else
  {
    wxMemoryInputStream mis( buf, size );
    wxImage img;
    if( !img.LoadFile( mis ) )
    {
      wxLogWarning(wxT("Failed loading image: %s"), fpath.c_str());
      return;
    }
    m_texid = Texture::create_texture( img, mipmap );
  }
  // we no longer need the buffer
  wxGetApp().pakmanager()->cleanup_lastloaded();
}

void Texture::glBind()
{
  if( m_texid )
    glBindTexture(GL_TEXTURE_2D, m_texid);
}

GLuint Texture::create_texture( wxImage& img, bool mipmap /*=false */ )
{
  
  GLuint texid;

  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) );

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);

  
  //make the image GL compliant (on 2^ size boundaries)
  if( !is_compliantsize(img.GetWidth(), img.GetHeight()))
    img = image_makecompliantsize(img);

  
  if( mipmap )
    gluBuild2DMipmaps(GL_TEXTURE_2D, (img.HasAlpha() ?  4 : 3), img.GetWidth(), 
    img.GetHeight(), (img.HasAlpha() ?  GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, img.GetData());
  else
    glTexImage2D(GL_TEXTURE_2D, 0, (img.HasAlpha() ?  4 : 3), img.GetWidth(),
    img.GetHeight(), 0, (img.HasAlpha() ?  GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, img.GetData());

  return texid;
}


GLuint Texture::create_texture( tgainfo_t *tga, bool mipmap /*=false*/ )
{
  if( !is_compliantsize(tga->width, tga->height))
  {
    // sigh, width and/or height is not a powerof 2 :/
    if( tga->bpp != 24  && tga->bpp != 32)
    {
      wxLogError( wxT("Cannot create OpenGL texture, width and height must be power of 2 or 24/32bpp format.") );
      return 0;
    }
    
    unsigned char *data = 0;
    unsigned char *alpha = 0;

    // copy data.
    data = static_cast<unsigned char*>( malloc(tga->width * tga->height * 3) );
    if( tga->bpp == 32 )
    {
      unsigned char *src = tga->data;
      unsigned char *dst = data;
      for( unsigned int i=0; i < tga->imgsize; i+=4, src += 4, dst +=3 )
        memcpy( dst, src, 3 );
    }
    else // tga->bpp == 24
      memcpy( data, tga->data, tga->imgsize );

    wxImage img(tga->width, tga->height, data, false);

    // copy alpha.
    if( tga->bpp == 32 )
    {
      alpha = static_cast<unsigned char*>( malloc(tga->width * tga->height) );
      unsigned char *src = tga->data+3;
      unsigned char *dst = alpha;
      for( unsigned int i=0; i < tga->imgsize; i+=4, src += 4, ++dst )
        *dst = *src;
      img.SetAlpha( alpha );
    }

    // now fix the size
    img = image_makecompliantsize(img);
    tga->width = img.GetWidth();
    tga->height = img.GetHeight();
    tga->imgsize = tga->height * tga->width * (tga->bpp/8);

    // and copy back
    delete [] tga->data;
    
    tga->data = new unsigned char[tga->imgsize];
    if( tga->bpp == 32 )
    { // data and alpha
      data = img.GetData();
      alpha = img.GetAlpha();
      unsigned char *dst = tga->data;
      for( unsigned int i=0; i < tga->imgsize; i+=4, dst += 4, data+=3, ++alpha )
      {
        dst[0] = *data;
        dst[1] = *(data+1);
        dst[2] = *(data+2);
        dst[3] = *alpha;
      }
    }
    else // tga->bpp == 24
      // data only.
      memcpy( tga->data, img.GetData(), tga->imgsize );
  }
  
  GLuint texid;

  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) );

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	
  if( mipmap )
    gluBuild2DMipmaps(GL_TEXTURE_2D, (tga->bpp == 24 ? 3 : 4), tga->width, 
     tga->height, (tga->bpp == 24 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, tga->data);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, (tga->bpp == 24 ? GL_RGB : GL_RGBA), tga->width,
    tga->height, 0, (tga->bpp == 24 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, 
      tga->data);

  return texid;

}


/// taken from Cities3D (http://www.s3dconnector.net/)
bool Texture::is_compliantsize( wxInt32 w, wxInt32 h )
{
  //check to see if the width and height are even multiples of two -- 
  //this works by bit shifting the number to the right one place at a 
  //time until a 1 appears in the 1's digit.  If it is a multiple of two,
  //the number will have been bit shifted all the way to its only 1 digit
  //and should now be 1. Otherwise, it is not a multiple of two.
  for(; !(w & 1); w >>= 1);
  for(; !(h & 1); h >>= 1);
  return ((1 == w) && (1 == h));
}

/// taken from Cities3D (http://www.s3dconnector.net/)
wxImage Texture::image_makecompliantsize(const wxImage& image)
{
  wxUint32 w = image.GetWidth();
  wxUint32 h = image.GetHeight();

  //find the next largest multiple of two to scale to
  wxInt32 count;
  for(count = 0; !(w & (1 << 31)); w <<= 1, ++count);
  w = (1 << (31 - count));
  for(count = 0; !(h & (1 << 31)); h <<= 1, ++count);
  h = (1 << (31 - count));

  return image.Scale(w, h);
}
