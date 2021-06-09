#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "types.h"

class Window
{
  // Window
  f32 width, height;

  void WindowSetup();

public:
  GLFWwindow* window;
  Window( f32 w, f32 h );
  ~Window();
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
