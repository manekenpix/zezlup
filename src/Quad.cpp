#include "include/Quad.h"

Quad::Quad( f32 w, f32 h, f32 screenW, f32 screenH )
  : width{ w }
  , height{ h }
  , screenWidth{ screenW }
  , screenHeight{ screenH }
{
  glGenVertexArrays( 1, &vertexArray );
  glGenBuffers( 1, &vertexBuffer );
  glGenBuffers( 1, &elementBufferObject );

  glBindVertexArray( vertexArray );

  // Position
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData(
    GL_ARRAY_BUFFER, sizeof( vertices ), vertices.data(), GL_STATIC_DRAW );

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

Quad::~Quad()
{
  glDeleteVertexArrays( 1, &vertexArray );
  glDeleteBuffers( 1, &vertexBuffer );
  glDeleteBuffers( 1, &elementBufferObject );

  delete tex;
};
