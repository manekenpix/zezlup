#include "include/Texture.h"

Texture::Texture( u8* buffer, s32 width, s32 height, u8 colourType )
{
  if ( buffer ) {
    GLenum colourTypeSelected;

    if ( colourType == 255 )
      colourTypeSelected = GL_RED;
    else
      colourTypeSelected = colourType ? GL_RGBA : GL_RGB;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  colourTypeSelected,
                  width,
                  height,
                  0,
                  colourTypeSelected,
                  GL_UNSIGNED_BYTE,
                  buffer );

  } else {
    std::cout << "Problem loading the texture" << std::endl;
  }
};

void
Texture::remove()
{
  glDeleteTextures( 1, &texture );
};

void
Texture::bind()
{
  glBindTexture( GL_TEXTURE_2D, texture );
};

Texture::~Texture()
{
  remove();
};
