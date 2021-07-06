#ifndef QUAD_H
#define QUAD_H

#include "Shader.h"
#include "Texture.h"
#include "types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Quad
{
  // Position
  f32 x = 0;
  f32 y = 0;
  f32 width = 0;
  f32 height = 0;
  f32 screenWidth = 0;
  f32 screenHeight = 0;

public:
  u32 vertexArray, vertexBuffer, elementBufferObject;
  Vertices vertices = {

    // positions      texture coords
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top right
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
  };
  u32 indices[6] = { 0, 1, 3, 1, 2, 3 };

  Quad( f32 w, f32 h, f32 screenW, f32 screenH );
  // Textures
  Texture* tex;

  ~Quad();

  void setPosition( f32 x, f32 y );
  f32 getX() { return x; };
  f32 getY() { return y; };
};

#endif
