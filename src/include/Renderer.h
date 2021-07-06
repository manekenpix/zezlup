#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "types.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Renderer
{
  GameWindow* window = nullptr;
  f32 width = 0;
  f32 height = 0;

public:
  // Shaders
  std::vector<Shader*> shaders;
  glm::mat4 projectionMatrix;

  Renderer( GameWindow* window, f32 screenW, f32 screenH );
  ~Renderer();

  void draw( u32* vertexArray,
             u32* vertexBuffer,
             Vertices* vertices,
             Texture* texture,
             bool selected );

  void swapBuffers();
  void pollEvents();
};

#endif
