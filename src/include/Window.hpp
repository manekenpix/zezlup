#ifndef GLWINDOW_HPP
#define GLWINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Geometry.hpp"
#include "Shader.hpp"
#include "types.hpp"

class GlWindow
{
  // Window
  GLFWwindow* window;
  const f32 width = 1280, height = 960;

  void GlWindowSetup();

public:
  GlWindow();
  void GlLoadTexture( u8*, s32, s32, u8 );
  void GlRender( Geometry geometries );

  ~GlWindow();
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
