#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include <GL/glew.h>

class Texture
{
  GLuint texture;

public:
  Texture( u8* buffer, s32 width, s32 height, u8 colourType = 255 );
  void bind();
  void remove();
  ~Texture();
};

#endif // TEXTURE_H
