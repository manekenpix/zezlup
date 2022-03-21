#ifndef RENDERER_H
#define RENDERER_H

#include "Png.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Renderer
{
public:
  enum class Keys
  {
    left,
    right,
    up,
    down,
    enter,
    c,
    m,
    esc,
    blank
  };

  Renderer();
  ~Renderer();

  GLFWwindow* createWindow( f32 windowW, f32 windowH );

  void draw( std::string quad, std::string texture, std::string shader );

  void swapBuffers();
  void pollEvents();

  void loadShader( std::string key, std::string vShader, std::string fShader );
  void loadTexture( std::string name,
                    u8* buffer,
                    s32 width,
                    s32 height,
                    u8 colourType );

  void loadTexture( std::string name, u8* buffer, s32 width, s32 height );
  void createQuad( std::string key, f32 width, f32 height );
  void setQuadPosition( std::string quad, f32 x, f32 y );
  Keys getKey();

private:
  typedef struct KeyPair
  {
    Keys key;
    int code;

    KeyPair( Keys _key, int _code )
      : key{ _key }
      , code{ _code } {};

  } KeyPair;

  static const u8 totalKeys = 8;
  bool isKeyPressed;
  int pressedKey = -1;
  const KeyPair keys[totalKeys] = {
    KeyPair( Keys::right, GLFW_KEY_RIGHT ),
    KeyPair( Keys::left, GLFW_KEY_LEFT ),
    KeyPair( Keys::up, GLFW_KEY_UP ),
    KeyPair( Keys::down, GLFW_KEY_DOWN ),
    KeyPair( Keys::enter, GLFW_KEY_ENTER ),
    KeyPair( Keys::m, GLFW_KEY_M ),
    KeyPair( Keys::c, GLFW_KEY_C ),
    KeyPair( Keys::esc, GLFW_KEY_ESCAPE ),
  };

  GLFWwindow* window;
  f32 width = 0;
  f32 height = 0;

  std::unordered_map<std::string, Shader*> shaders;
  std::unordered_map<std::string, Texture*> textures;
  std::unordered_map<std::string, Quad*> quads;
  glm::mat4 projectionMatrix;
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
