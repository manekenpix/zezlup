#include "include/Renderer.h"

Renderer::Renderer( GameWindow* w, f32 screenW, f32 screenH )
  : window{ w }
  , width{ screenW }
  , height{ screenH }
{

  projectionMatrix = glm::ortho( 0.0f, width, height, 0.0f );
};

void
Renderer::draw( u32* vertexArray,
                u32* vertexBuffer,
                Vertices* vertices,
                Texture* texture,
                std::string shader )
{
  shaders[shader]->use();
  shaders[shader]->setMatrix4fv( "projection", projectionMatrix );

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

void
Renderer::addShader( std::string key, std::string vShader, std::string fShader )
{
  shaders.insert( { key, new Shader( vShader.c_str(), fShader.c_str() ) } );
};

Renderer::~Renderer()
{
  for ( const auto& [key, v] : shaders ) {
    delete v;
  }
};
