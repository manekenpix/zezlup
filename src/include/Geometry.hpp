#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "types.hpp"

class Geometry
{
  // Textures
  s32 textureWidth, textureHeight;
  u8* textureBuffer;

  // Generate texture
  GLuint texture;

  // Vertex
  u32 vertexArray, vertexBuffer, elementBufferObject;
  f32 vertices[20] = {

    // positions      texture coords
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom right
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top right
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
  };
  u32 indices[6] = { 0, 1, 3, 1, 2, 3 };

  const f32 width = 1280, height = 960;

  // Shader
  Shader* shaders;
  glm::mat4 projectionMatrix =
    glm::ortho( 0.0f, (f32)width, (f32)height, 0.0f );

public:
  Geometry( u8* buffer,
            s32 tWidth,
            s32 tHeight,
            u8 colourType,
            f32 width,
            f32 height );

  ~Geometry();

  void loadTexture( u8* buffer,
                    s32 tWidth,
                    s32 tHeight,
                    u8 colourType,
                    f32 width,
                    f32 height );

  void scaleTexture( f32 width, f32 height );

  void bindVertexArray();
};