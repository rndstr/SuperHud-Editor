#ifndef TGALOADER_H
#define TGALOADER_H

#include "common.h"

enum 
{
  IMG_OK = 0,
  IMG_ERR_MEM_FAIL = 1<<0,
  IMG_ERR_BAD_FORMAT = 1<<1,
  IMG_ERR_UNSUPPORTED = 1<<2
};

struct tgainfo_t
{
  unsigned char *data;
  GLuint        bpp;
  GLuint        width;
  GLuint        height;
  GLuint        imgsize; ///< = width * height * (bpp/8)
  char          enctype;
  unsigned char *palette;
  unsigned char error;
};

bool load_tga( unsigned char *buffer, tgainfo_t *tga, size_t h );


#endif // TGALOADER_H
