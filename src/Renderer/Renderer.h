#ifndef RENDERER_H
#define RENDERER_H

#include "../Image/Image.h"
#include "../Logger/Logger.h"
#include "Font/Font.h"
#include "Quad/Quad.h"
#include "Shader.h"
#include "Texture/Texture.h"
#include "types.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

class Renderer
{
public:
  const static Colour WHITE;
  const static Colour BLACK;
  const static Colour RED;
  const static Colour GREEN;
  const static Colour BLUE;

  Mouse mouse;

  Renderer();
  ~Renderer();

  GLFWwindow* createWindow( f32 windowW, f32 windowH );

  void draw( std::string quad,
             std::string texture,
             std::string shader,
             Colour colour,
             f32 alpha = 1.0f );
  void draw( std::string quad,
             std::string texture,
             std::string shader,
             f32 alpha = 1.0f );
  void draw( std::string quad, Colour colour );

  void clearColor( const Colour colour ) const;
  void clear() const;
  void swapBuffers();
  void pollEvents() const;
  f32 getTime() const;
  bool windowShouldClose() const;

  void loadShader( std::string key, std::string vShader, std::string fShader );
  void loadTexture( std::string name,
                    u8* buffer,
                    s32 width,
                    s32 height,
                    u8 colourType );

  void loadTexture( std::string name, u8* buffer, s32 width, s32 height );
  void deleteTexture( std::string texture );
  void createQuad( std::string name, f32 width, f32 height );
  void deleteQuad( std::string name );
  void setQuadPosition( const std::string quad, const Vec2 position );
  Keys getKey();
  Mouse* getMouse();
  void getMouseState();
  void loadFont( const std::string& id, const std::string& fontPath );
  void print( const std::string s, Vec2 position, const std::string fontId );

private:
  typedef struct KeyPair
  {
    Keys key;
    int code;

    KeyPair( Keys _key, int _code )
      : key{ _key }
      , code{ _code } {};

  } KeyPair;

  Logger logger;
  std::unordered_map<std::string, Font*> fonts;

  bool isKeyPressed;
  int pressedKey = -1;

  static const u8 totalKeys = 10;
  const std::array<KeyPair, totalKeys> keys = {
    KeyPair( Keys::right, GLFW_KEY_RIGHT ),
    KeyPair( Keys::left, GLFW_KEY_LEFT ),
    KeyPair( Keys::up, GLFW_KEY_UP ),
    KeyPair( Keys::down, GLFW_KEY_DOWN ),
    KeyPair( Keys::enter, GLFW_KEY_ENTER ),
    KeyPair( Keys::x, GLFW_KEY_X ),
    KeyPair( Keys::z, GLFW_KEY_Z ),
    KeyPair( Keys::help, GLFW_KEY_H ),
    KeyPair( Keys::esc, GLFW_KEY_ESCAPE ),
    KeyPair( Keys::backspace, GLFW_KEY_BACKSPACE ),
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
