#ifndef RENDERER_H
#define RENDERER_H

#include "Png.h"
#include "Shader.h"
#include "Texture.h"
#include "types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Renderer
{
  typedef struct KeyPair
  {
    std::string key;
    int code;

    KeyPair( std::string _key, int _code )
      : key{ _key }
      , code{ _code } {};

  } KeyPair;

  static const u8 totalKeys = 8;
  bool isKeyPressed;
  int pressedKey = -1;
  const KeyPair keys[totalKeys] = {
    KeyPair( "right", GLFW_KEY_RIGHT ), KeyPair( "left", GLFW_KEY_LEFT ),
    KeyPair( "up", GLFW_KEY_UP ),       KeyPair( "down", GLFW_KEY_DOWN ),
    KeyPair( "enter", GLFW_KEY_ENTER ), KeyPair( "m", GLFW_KEY_M ),
    KeyPair( "c", GLFW_KEY_C ),         KeyPair( "esc", GLFW_KEY_ESCAPE ),
  };

  GLFWwindow* window;
  f32 width = 0;
  f32 height = 0;

  // Shaders
  std::unordered_map<std::string, Shader*> shaders;
  std::unordered_map<std::string, Texture*> textures;
  glm::mat4 projectionMatrix;

public:
  Renderer();
  ~Renderer();

  GLFWwindow* createWindow( f32 windowW, f32 windowH );

  void draw( u32* vertexArray,
             u32* vertexBuffer,
             Vertices* vertices,
             std::string texture,
             std::string shader );

  void drawText( u32* vertexArray,
                 u32* vertexBuffer,
                 TextVertices* vertices,
                 std::string texture,
                 std::string shader );

  void swapBuffers();
  void pollEvents();

  void addShader( std::string key, std::string vShader, std::string fShader );
  void addTexture( std::string name,
                   u8* buffer,
                   s32 width,
                   s32 height,
                   u8 colourType );

  void addTexture( std::string name, u8* buffer, s32 width, s32 height );

  std::string getKey();
};

void APIENTRY
glDebugOutput( GLenum source,
               GLenum type,
               unsigned int id,
               GLenum severity,
               GLsizei length,
               const char* message,
               const void* userParam );

#endif
