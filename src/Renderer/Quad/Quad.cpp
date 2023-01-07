#include "Quad.h"

Quad::Quad( f32 w, f32 h )
  : width{ w }
  , height{ h }
{
  glGenVertexArrays( 1, &vertexArray );
  glBindVertexArray( vertexArray );

  glGenBuffers( 1, &vertexBuffer );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof( f32 ) * 6 * 4, NULL, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
};

void
Quad::setPosition( Vec2 position )
{
  x = position.x;
  y = position.y;

  vertices[0] = vertices[12] = x + width;  // Bottom Right X
  vertices[1] = vertices[13] = y + height; // Bottom Right Y
  vertices[8] = vertices[20] = x;          // Top Left X
  vertices[9] = vertices[21] = y;          // Top Left Y

  vertices[4] = x + width;   // Top Right X
  vertices[5] = y;           // Top Right Y
  vertices[16] = x;          // Bottom Left X
  vertices[17] = y + height; // Bottom Left Y
};

Vec2
Quad::getSize()
{
  return Vec2( width, height );
};

void
Quad::bind()
{
  glBindVertexArray( vertexArray );
  glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
  glBufferData(
    GL_ARRAY_BUFFER, sizeof( vertices ), vertices.data(), GL_STATIC_DRAW );
  glDrawArrays( GL_TRIANGLES, 0, 6 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
};

void
Quad::remove()
{
  glDeleteVertexArrays( 1, &vertexArray );
  glDeleteBuffers( 1, &vertexBuffer );
};

Quad::~Quad()
{
  remove();
};
