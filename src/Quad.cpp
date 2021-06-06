#include "include/Quad.hpp"

Quad::Quad( Shader* shader, f32 w, f32 h, f32 screenW, f32 screenH )
{
  width = w;
  height = h;
  screenWidth = screenW;
  screenHeight = screenH;

  projectionMatrix =
    glm::ortho( 0.0f, (f32)screenWidth, (f32)screenHeight, 0.0f );

  shaders = shader;
  glGenVertexArrays( 1, &vertexArray );
  glGenBuffers( 1, &vertexBuffer );
  glGenBuffers( 1, &elementBufferObject );

  glBindVertexArray( vertexArray );

  // Position
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

  // position attribute
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( f32 ), (void*)0 );
  glEnableVertexAttribArray( 0 );

  // Texture
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementBufferObject );
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

  // texture attribute
  glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( f32 ), (void*)( 3 * sizeof( f32 ) ) );
  glEnableVertexAttribArray( 1 );
};

void
Quad::attachTexture( u8* buffer, s32 tWidth, s32 tHeight, u8 colourType )
{

  if ( buffer ) {
    textureWidth = tWidth;
    textureHeight = tHeight;
    textureBuffer = buffer;

    // Generate texture
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    // Wrapping parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // set texture filtering parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
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
Quad::setPosition( f32 positionX, f32 positionY )
{
  x = positionX;
  y = positionY;

  vertices[0] = x + width;   // Bottom Right X
  vertices[1] = y + height;  // Bottom Right Y
  vertices[5] = x + width;   // Top Right X
  vertices[6] = y;           // Top Right Y
  vertices[10] = x;          // Top Left X
  vertices[11] = y;          // Top Left Y
  vertices[15] = x;          // Bottom Left X
  vertices[16] = y + height; // Bottom Left Y
};

void
Quad::bindVertexArray()
{
  shaders->use();
  shaders->setMatrix4fv( "projection", projectionMatrix );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
  glBindTexture( GL_TEXTURE_2D, texture );
  glBindVertexArray( vertexArray );
  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 );
};

Quad::~Quad()
{
  glDeleteVertexArrays( 1, &vertexArray );
  glDeleteBuffers( 1, &vertexBuffer );
  glDeleteBuffers( 1, &elementBufferObject );
};
