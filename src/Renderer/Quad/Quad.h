#ifndef QUAD_H
#define QUAD_H

#include "../types.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Quad
{
  typedef std::array<f32, 24> Vertices;

  f32 x;
  f32 y;

  f32 width;
  f32 height;

public:
  u32 vertexArray, vertexBuffer;

  std::string textureID;

  Vertices vertices = {

    // positions texture coords
    0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 1.0f, 0.0f, // Top right
    0.0f, 0.0f, 0.0f, 0.0f, // top left

    0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 0.0f, 1.0f, // bottom left
    0.0f, 0.0f, 0.0f, 0.0f  // top left
  };

  Quad( f32 w, f32 h );
  void remove();
  void bind();
  ~Quad();

  void setPosition( Vec2 position );
};

#endif
