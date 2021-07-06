#include "include/Renderer.h"

Renderer::Renderer( GameWindow* w, f32 screenW, f32 screenH )
  : window{ w }
  , width{ screenW }
  , height{ screenH }
{
  shaders.push_back(
    new Shader( "src/Shaders/common.vert", "src/Shaders/grid.frag" ) );

  shaders.push_back(
    new Shader( "src/Shaders/common.vert", "src/Shaders/outline.frag" ) );

  projectionMatrix = glm::ortho( 0.0f, width, height, 0.0f );
};

void
Renderer::draw( u32* vertexArray,
                u32* vertexBuffer,
                Vertices* vertices,
                Texture* texture,
                bool selected )
{
  shaders[selected ? 1 : 0]->use();
  shaders[selected ? 1 : 0]->setMatrix4fv( "projection", projectionMatrix );

  texture->bind();
  glBindVertexArray( *vertexArray );
  glBindBuffer( GL_ARRAY_BUFFER, *vertexBuffer );
  glBufferData(
    GL_ARRAY_BUFFER, sizeof( *vertices ), vertices->data(), GL_STATIC_DRAW );

  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
};

void
Renderer::swapBuffers()
{
  glfwSwapBuffers( window->window );
};

void
Renderer::pollEvents()
{
  glfwPollEvents();
};

Renderer::~Renderer()
{
  std::for_each(
    shaders.begin(), shaders.end(), []( Shader* shader ) { delete shader; } );
};
