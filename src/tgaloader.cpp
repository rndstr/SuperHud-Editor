#include "tgaloader.h"

#include <memory.h>

static bool ReadHeader( unsigned char *buffer, tgainfo_t *tga);
static bool LoadRawData( unsigned char *buffer, tgainfo_t *tga ); // Load uncompressed image data
static bool LoadTgaRLEData( unsigned char *buffer, tgainfo_t *tga ); // Load RLE compressed image data
static bool LoadTgaPalette( unsigned char *buffer, tgainfo_t *tga ); // Load a 256 color palette
static void BGRtoRGB( tgainfo_t *tga); // Convert BGR to RGB (or back again)
static void FlipImg( tgainfo_t *tga); // Flips the image vertically (Why store images upside down?)
 
bool load_tga( unsigned char *buffer, tgainfo_t *tga, size_t bufsize )
 {
   wxASSERT( buffer != 0 );
   unsigned char *pPalette=0;
 
  if( !ReadHeader( buffer, tga ) )
    return false;
 
   switch(tga->enctype)
    {
     case 1: // Raw Indexed
      {
        // TODO create image from palette 
        tga->error = IMG_ERR_BAD_FORMAT;
        return false;
        
        /*

       // Check filesize against header values
        if((tga->imgsize+18+buffer[0]+768)>bufsize)
        {
          tga->error = IMG_ERR_BAD_FORMAT;
          return false;
        }
 
       // Double check image type field
        if(buffer[1]!=1)
        {
          tga->error = IMG_ERR_BAD_FORMAT;
          return false;
        }
 
       // Load image data
       if( !LoadRawData( buffer, tga ) )
         return false;
 
       // Load palette
       if( !LoadTgaPalette( buffer, tga ) )
         return false;
 
 */
       break;
      }
 
     case 2: // Raw RGB
      {
       // Check filesize against header values
        if((tga->imgsize+18+buffer[0])>bufsize)
        {
          tga->error = IMG_ERR_BAD_FORMAT;
          return false;
        }
 
       // Double check image type field
        if(buffer[1]!=0)
        {
          tga->error = IMG_ERR_BAD_FORMAT;
          return false;
        }
 
       // Load image data
       if( !LoadRawData( buffer, tga ) )
         return false;
 
       BGRtoRGB( tga ); // Convert to RGB
       break;
      }
 
     case 9: // RLE Indexed
      {
        // TODO create image from palette 
        tga->error = IMG_ERR_BAD_FORMAT;
        return false;

        /*
       // Double check image type field
        if(buffer[1]!=1)
        {
          tga->error = IMG_ERR_BAD_FORMAT;
          return false;
        }
 
       // Load image data
       if( !LoadTgaRLEData( buffer, tga ) )
         return false;

       // Load palette
       if( !LoadTgaPalette( buffer, tga ) )
         return false;
 
 */
       break;
      }
 
     case 10: // RLE RGB
      {
       // Double check image type field
        if(buffer[1]!=0)
        {
          tga->error = IMG_ERR_BAD_FORMAT;
          return false;
        }
 
       // Load image data
       if( !LoadTgaRLEData( buffer, tga ) )
         return false;
 
       BGRtoRGB(tga); // Convert to RGB
       break;
      }
 
     default:
       tga->error = IMG_ERR_UNSUPPORTED;
       return false;
    }
 
  // Check flip bit
   if((buffer[17] & 0x20)==0) 
     FlipImg( tga );
 
  // Release file memory
  //delete [] tga->palette;
  //tga->palette = 0;
 
  return true;
 }
 
 
static bool ReadHeader( unsigned char *buffer, tgainfo_t *tga) // Examine the header and populate our class attributes
 {
   short ColMapStart,ColMapLen;
  short x1,y1,x2,y2;
 
   if(buffer[1]>1)    // 0 (RGB) and 1 (Indexed) are the only types we know about
   {
     tga->error = IMG_ERR_UNSUPPORTED;
     return false;
   }
 
   tga->enctype=buffer[2];     // Encoding flag  1 = Raw indexed image
                      //                2 = Raw RGB
                      //                3 = Raw greyscale
                      //                9 = RLE indexed
                      //               10 = RLE RGB
                      //               11 = RLE greyscale
                      //               32 & 33 Other compression, indexed
 
    if(tga->enctype>11)       // We don't want 32 or 33
    {
      tga->error = IMG_ERR_UNSUPPORTED;
      return false;
    }
 
 
  // Get palette info
  memcpy(&ColMapStart,&buffer[3],2);
  memcpy(&ColMapLen,&buffer[5],2);
 
  // Reject indexed images if not a VGA palette (256 entries with 24 bits per entry)
   if(buffer[1]==1) // Indexed
    {
     if(ColMapStart!=0 || ColMapLen!=256 || buffer[7]!=24)
     {
       tga->error = IMG_ERR_UNSUPPORTED;
       return false;
     }
    }
 
  // Get image window and produce width & height values
  memcpy(&x1,&buffer[8],2);
  memcpy(&y1,&buffer[10],2);
  memcpy(&x2,&buffer[12],2);
  memcpy(&y2,&buffer[14],2);
 
  tga->width=(x2-x1);
  tga->height=(y2-y1);
 
   if(tga->width<1 || tga->height<1)
   {
     tga->error = IMG_ERR_BAD_FORMAT;
     return false;
   }
 
  // Bits per Pixel
  tga->bpp=buffer[16];
 
  // Check flip / interleave byte
   if(buffer[17]>32) // Interleaved data
   {
     tga->error = IMG_ERR_UNSUPPORTED;
     return false;
   }
 
  // Calculate image size
  tga->imgsize = (tga->width * tga->height * (tga->bpp/8));
 
  return true;
 }
 
 
static bool LoadRawData( unsigned char *buffer, tgainfo_t *tga ) // Load uncompressed image data
 {
  short iOffset;
 
  tga->data=new unsigned char[tga->imgsize];
 
   if(tga->data==0)
   {
     tga->error = IMG_ERR_MEM_FAIL;
     return false;
   }
 
  iOffset=buffer[0]+18; // Add header to ident field size
 
   if(buffer[1]==1) // Indexed images
    iOffset+=768;  // Add palette offset
 
   memcpy(tga->data,&buffer[iOffset],tga->imgsize);
 
  return true;
 }
 
 
static bool LoadTgaRLEData( unsigned char *buffer, tgainfo_t *tga ) // Load RLE compressed image data
 {
  short iOffset,iPixelSize;
  unsigned char *pCur;
  unsigned long Index=0;
  unsigned char bLength,bLoop;
 
  // Calculate offset to image data
  iOffset=buffer[0]+18;
 
  // Add palette offset for indexed images
   if(buffer[1]==1)
    iOffset+=768; 
 
  // Get pixel size in bytes
  iPixelSize=tga->bpp/8;
 
  // Set our pointer to the beginning of the image data
  pCur=&buffer[iOffset];
 
  // Allocate space for the image data 
  tga->data=new unsigned char[tga->imgsize ];
 
   if(tga->data==0)
   {
     tga->error = IMG_ERR_MEM_FAIL;
     return false;
   }
 
  // Decode
   while(Index<tga->imgsize) 
    {
      if(*pCur & 0x80) // Run length chunk (High bit = 1)
       {
        bLength=*pCur-127; // Get run length
        pCur++;            // Move to pixel data  
 
        // Repeat the next pixel bLength times
         for(bLoop=0;bLoop!=bLength;++bLoop,Index+=iPixelSize)
          memcpy(&tga->data[Index],pCur,iPixelSize);
  
        pCur+=iPixelSize; // Move to the next descriptor chunk
       }
      else // Raw chunk
       {
        bLength=*pCur+1; // Get run length
        pCur++;          // Move to pixel data
 
        // Write the next bLength pixels directly
         for(bLoop=0;bLoop!=bLength;++bLoop,Index+=iPixelSize,pCur+=iPixelSize)
          memcpy(&tga->data[Index],pCur,iPixelSize);
       }
    }
 
  return true;
 }
 
 
static bool LoadTgaPalette( unsigned char *buffer, tgainfo_t *tga ) // Load a 256 color palette
 {
  unsigned char bTemp;
  short iIndex,iPalPtr;
 
   
 
  // Create space for new palette
  tga->palette=new unsigned char[768];
 
   if(tga->palette==0)
   {
     tga->error = IMG_ERR_MEM_FAIL;
     return false;
   }
 
  // VGA palette is the 768 bytes following the header
  memcpy(tga->palette,&buffer[buffer[0]+18],768);
 
  // Palette entries are BGR ordered so we have to convert to RGB
   for(iIndex=0,iPalPtr=0;iIndex!=256;++iIndex,iPalPtr+=3)
    {
     bTemp=tga->palette[iPalPtr];               // Get Blue value
     tga->palette[iPalPtr]=tga->palette[iPalPtr+2]; // Copy Red to Blue
     tga->palette[iPalPtr+2]=bTemp;             // Replace Blue at the end
    }
 
  return true;
 }
 
 
static void BGRtoRGB( tgainfo_t *tga) // Convert BGR to RGB (or back again)
 {
  unsigned long Index,nPixels;
  unsigned char *bCur;
  unsigned char bTemp;
  short iPixelSize;
 
  // Set ptr to start of image
  bCur=tga->data;
 
  // Calc number of pixels
  nPixels=tga->width * tga->height;
 
  // Get pixel size in bytes
  iPixelSize=tga->bpp/8;
 
   for(Index=0;Index!=nPixels;Index++)  // For each pixel
    {
     bTemp=*bCur;      // Get Blue value
     *bCur=*(bCur+2);  // Swap red value into first position
     *(bCur+2)=bTemp;  // Write back blue to last position
 
     bCur+=iPixelSize; // Jump to next pixel
    }
 
 }
 
 
static void FlipImg( tgainfo_t *tga) // Flips the image vertically (Why store images upside down?)
 {
  unsigned char bTemp;
  unsigned char *pLine1, *pLine2;
  int iLineLen,iIndex;
 
  iLineLen=tga->width*(tga->bpp/8);
  pLine1=tga->data;
  pLine2=&tga->data[iLineLen * (tga->height - 1)];
 
   for( ;pLine1<pLine2;pLine2-=(iLineLen*2))
    {
     for(iIndex=0;iIndex!=iLineLen;pLine1++,pLine2++,iIndex++)
      {
       bTemp=*pLine1;
       *pLine1=*pLine2;
       *pLine2=bTemp;       
      }
    } 
 
 }

