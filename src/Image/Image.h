#ifndef IMAGE_H
#define IMAGE_H

#include "../Renderer/types.h"

class Image
{
public:
  virtual u32 getWidth() = 0;
  virtual u32 getHeight() = 0;
  virtual u8* getImageBuffer() = 0;
  virtual u8 getColourType() = 0;
  virtual ~Image(){};
};

Image*
loadImage( const char* file );

#endif
