#include "include/Texture.h"

Texture::Texture( u8* buffer, s32 width, s32 height, u8 colourType )
{
  if ( buffer ) {
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  colourType ? GL_RGBA : GL_RGB,
                  width,
                  height,
                  0,
                  colourType ? GL_RGBA : GL_RGB,
                  GL_UNSIGNED_BYTE,
                  buffer );

  } else {
    std::cout << "Problem loading the texture" << std::endl;
  }
};

Texture::Texture( u8* buffer, s32 width, s32 height )
{
  if ( buffer ) {
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  GL_RED,
                  width,
                  height,
                  0,
                  GL_RED,
                  GL_UNSIGNED_BYTE,
                  buffer );

  } else {
    std::cout << "Problem loading the texture" << std::endl;
  }
};

void
Texture::bind()
{
  glBindTexture( GL_TEXTURE_2D, texture );
};
