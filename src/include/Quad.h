#ifndef QUAD_H
#define QUAD_H

#include "Shader.h"
#include "types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Quad
{
  // Position
  f32 x = 0;
  f32 y = 0;
  f32 width = 0;
  f32 height = 0;

public:
  u32 vertexArray, vertexBuffer, elementBufferObject;

  // Textures
  std::string textureID;

  Vertices vertices = {

    // positions      texture coords
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top right
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
  };
  u32 indices[6] = { 0, 1, 3, 1, 2, 3 };

  Quad( f32 w, f32 h );
  ~Quad();

  void setPosition( f32 x, f32 y );
};

#endif
