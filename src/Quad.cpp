#include "include/Quad.h"

Quad::Quad( f32 w, f32 h )
  : width{ w }
  , height{ h }
{
  glGenVertexArrays( 1, &vertexArray );
  glGenBuffers( 1, &vertexBuffer );

  glBindVertexArray( vertexArray );

  // Position
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof( f32 ) * 6 * 4, NULL, GL_STATIC_DRAW );

  // position attribute
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( f32 ), 0 );
};

void
Quad::setPosition( f32 positionX, f32 positionY )
{
  x = positionX;
  y = positionY;

  vertices[0] = x + width;  // Bottom Right X
  vertices[1] = y + height; // Bottom Right Y
  vertices[4] = x + width;  // Top Right X
  vertices[5] = y;          // Top Right Y
  vertices[8] = x;          // Top Left X
  vertices[9] = y;          // Top Left Y

  vertices[12] = x + width;  // Bottom Right X
  vertices[13] = y + height; // Bottom Right Y
  vertices[16] = x;          // Bottom Left X
  vertices[17] = y + height; // Bottom Left Y
  vertices[20] = x;          // Top Left X
  vertices[21] = y;          // Top Left Y
};

Quad::~Quad()
{
  glDeleteVertexArrays( 1, &vertexArray );
  glDeleteBuffers( 1, &vertexBuffer );
};
