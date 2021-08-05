#ifndef TEXT_H
#define TEXT_H

#include "Shader.h"
#include "types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Text
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

  TextVertices vertices = {

    // positions      texture coords
    0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 1.0f, 0.0f, // Top right
    0.0f, 0.0f, 0.0f, 0.0f, // top left

    0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 0.0f, 1.0f, // bottom left
    0.0f, 0.0f, 0.0f, 0.0f  // top left
  };

  Text( f32 w, f32 h );
  ~Text();

  void setPosition( f32 x, f32 y );
};

#endif
