#ifndef RENDERER_H
#define RENDERER_H

#include "Png.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Renderer
{

  GameWindow* window = nullptr;
  f32 width = 0;
  f32 height = 0;

  // Shaders
  std::unordered_map<std::string, Shader*> shaders;
  std::unordered_map<std::string, Texture*> textures;
  glm::mat4 projectionMatrix;

public:
  Renderer( GameWindow* window, f32 screenW, f32 screenH );
  ~Renderer();

  void draw( u32* vertexArray,
             u32* vertexBuffer,
             Vertices* vertices,
             std::string selectedTexture,
             std::string selectedShader );

  void swapBuffers();
  void pollEvents();

  void addShader( std::string key, std::string vShader, std::string fShader );
  void addTexture( std::string name,
                   u8* buffer,
                   s32 width,
                   s32 height,
                   u8 colourType );
};

#endif
