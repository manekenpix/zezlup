#include "include/Geometry.hpp"

Geometry::Geometry( u8* buffer,
                    s32 tWidth,
                    s32 tHeight,
                    u8 colourType,
                    f32 width,
                    f32 height )
{

  shaders = new Shader( "src/Shader/vertexShader.vert",
                        "src/Shader/fragmentShader.frag" );
  loadTexture( buffer, tWidth, tHeight, colourType, width, height );

  /* TRIANGLES */
  // Let's draw a rectangle using 2 triangles
  glGenVertexArrays( 1, &vertexArray );
  glGenBuffers( 1, &vertexBuffer );
  glGenBuffers( 1, &elementBufferObject );

  glBindVertexArray( vertexArray );

  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementBufferObject );
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

  // position attribute
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( f32 ), (void*)0 );
  glEnableVertexAttribArray( 0 );
  // texture attribute
  glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( f32 ), (void*)( 3 * sizeof( f32 ) ) );
  glEnableVertexAttribArray( 1 );
};

void
Geometry::loadTexture( u8* buffer,
                       s32 tWidth,
                       s32 tHeight,
                       u8 colourType,
                       f32 width,
                       f32 height )
{

  textureWidth = tWidth;
  textureHeight = tHeight;
  textureBuffer = buffer;
  scaleTexture( width, height );

  // Generate texture
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );

  // Wrapping parameters
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  // set texture filtering parameters
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  if ( textureBuffer ) {
    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  colourType ? GL_RGBA : GL_RGB,
                  textureWidth,
                  textureHeight,
                  0,
                  colourType ? GL_RGBA : GL_RGB,
                  GL_UNSIGNED_BYTE,
                  textureBuffer );
  } else {
    std::cout << "Problem loading the texture" << std::endl;
  }
};

void
Geometry::scaleTexture( f32 width, f32 height )
{
  f32 widthOffset = 0;
  f32 heightOffset = 0;

  if ( (f32)textureHeight / textureWidth < height / width ) {
    heightOffset =
      ( height - ( ( textureHeight * width ) / textureWidth ) ) / 2;
  } else if ( (f32)textureHeight / textureWidth > height / width ) {
    widthOffset = ( width - ( ( textureWidth * height ) / textureHeight ) ) / 2;
  }

  vertices[0] = width - widthOffset;    // Bottom Right X
  vertices[1] = height - heightOffset;  // Bottom Right Y
  vertices[5] = width - widthOffset;    // Top Right X
  vertices[6] = heightOffset;           // Top Right Y
  vertices[10] = widthOffset;           // Top Left X
  vertices[11] = heightOffset;          // Top Left Y
  vertices[15] = widthOffset;           // Bottom Left X
  vertices[16] = height - heightOffset; // Bottom Left Y
};

void
Geometry::bindVertexArray()
{
  shaders->use();
  shaders->setMatrix4fv( "projection", projectionMatrix );
  glBindVertexArray( vertexArray );
  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 );
};

Geometry::~Geometry()
{
  delete shaders;
  glDeleteVertexArrays( 1, &vertexArray );
  glDeleteBuffers( 1, &vertexBuffer );
  glDeleteBuffers( 1, &elementBufferObject );
};
