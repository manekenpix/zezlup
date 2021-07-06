/* date = June 28th 2021 11:50 pm */
#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include <GL/glew.h>

class Texture
{
  GLuint texture;

  s32 width, height;
  u8* buffer;

public:
  Texture( u8* tBuffer, s32 tWidth, s32 tHeight, u8 colourType );
  void bind();
  ~Texture(){};
};

#endif // TEXTURE_H
