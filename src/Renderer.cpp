#include "include/Renderer.h"

Renderer::Renderer()
  : isKeyPressed{ true }
{
  if ( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
  }

  // 4x antialiasing
  glfwWindowHint( GLFW_SAMPLES, 4 );

  // No resizable window
  glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

  // OpenGL 3
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

  // To make MacOS happy; should not be needed
  // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

  // Don't use the old OpenGL
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );
};

GLFWwindow*
Renderer::createWindow( f32 windowW, f32 windowH )
{
  width = windowW;
  height = windowH;
  projectionMatrix = glm::ortho( 0.0f, width, height, 0.0f );

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( width, height, "Zezlup", NULL, NULL );

  if ( window == NULL ) {
    std::cout << "Failed to open GLFW window." << std::endl;
    getchar();
    glfwTerminate();
  }

  glfwMakeContextCurrent( window );

  glViewport( 0, 0, width, height );

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glfwSetInputMode( window, GLFW_STICKY_KEYS, GLFW_TRUE );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

  int flags;
  glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
  if ( flags & GL_CONTEXT_FLAG_DEBUG_BIT ) {
    // initialize debug output
    glEnable( GL_DEBUG_OUTPUT );
    glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
    glDebugMessageCallback( glDebugOutput, nullptr );
    glDebugMessageControl(
      GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );
  }

  return window;
};

void
Renderer::draw( u32* vertexArray,
                u32* vertexBuffer,
                Vertices* vertices,
                std::string texture,
                std::string shader )
{
  shaders[shader]->use();
  shaders[shader]->setMatrix4fv( "projection", projectionMatrix );

  textures[texture]->bind();
  glBindVertexArray( *vertexArray );
  glBindBuffer( GL_ARRAY_BUFFER, *vertexBuffer );
  glBufferData(
    GL_ARRAY_BUFFER, sizeof( *vertices ), vertices->data(), GL_STATIC_DRAW );

  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
};

void
Renderer::drawText( u32* vertexArray,
                    u32* vertexBuffer,
                    TextVertices* vertices,
                    std::string texture,
                    std::string shader )
{
  shaders[shader]->use();
  shaders[shader]->setMatrix4fv( "projection", projectionMatrix );

  textures[texture]->bind();
  glBindVertexArray( *vertexArray );
  glBindBuffer( GL_ARRAY_BUFFER, *vertexBuffer );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( *vertices ), vertices->data() );
  glDrawArrays( GL_TRIANGLES, 0, 6 );
};

void
Renderer::swapBuffers()
{
  glfwSwapBuffers( window );
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

void
Renderer::addTexture( std::string name,
                      u8* buffer,
                      s32 width,
                      s32 height,
                      u8 colourType )
{
  textures.insert( { name, new Texture( buffer, width, height, colourType ) } );
};

void
Renderer::addTexture( std::string name, u8* buffer, s32 width, s32 height )
{
  textures.insert( { name, new Texture( buffer, width, height ) } );
};

std::string
Renderer::getKey()
{
  u8 index = 0;

  while ( !isKeyPressed && index < totalKeys ) {
    if ( glfwGetKey( window, keys[index].code ) == GLFW_PRESS ) {
      isKeyPressed = true;
      pressedKey = keys[index].code;
      return keys[index].key;
    }
    ++index;
  }

  index = 0;
  while ( isKeyPressed && index < totalKeys ) {
    if ( glfwGetKey( window, pressedKey ) == GLFW_RELEASE ) {
      isKeyPressed = false;
      return "-1";
    }
    ++index;
  }

  return "-1";
};

Renderer::~Renderer()
{
  for ( const auto& [key, v] : shaders ) {
    delete v;
  }

  for ( const auto& [key, v] : textures ) {
    delete v;
  }
  glfwTerminate();
};

void APIENTRY
glDebugOutput( GLenum source,
               GLenum type,
               unsigned int id,
               GLenum severity,
               GLsizei length,
               const char* message,
               const void* userParam )
{
  // ignore non-significant error/warning codes
  if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 )
    return;

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " << message << std::endl;

  switch ( source ) {
    case GL_DEBUG_SOURCE_API:
      std::cout << "Source: API";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      std::cout << "Source: Window System";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      std::cout << "Source: Shader Compiler";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      std::cout << "Source: Third Party";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      std::cout << "Source: Application";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      std::cout << "Source: Other";
      break;
  }
  std::cout << std::endl;

  switch ( type ) {
    case GL_DEBUG_TYPE_ERROR:
      std::cout << "Type: Error";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cout << "Type: Deprecated Behaviour";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cout << "Type: Undefined Behaviour";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cout << "Type: Portability";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cout << "Type: Performance";
      break;
    case GL_DEBUG_TYPE_MARKER:
      std::cout << "Type: Marker";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      std::cout << "Type: Push Group";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      std::cout << "Type: Pop Group";
      break;
    case GL_DEBUG_TYPE_OTHER:
      std::cout << "Type: Other";
      break;
  }
  std::cout << std::endl;

  switch ( severity ) {
    case GL_DEBUG_SEVERITY_HIGH:
      std::cout << "Severity: high";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cout << "Severity: medium";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      std::cout << "Severity: low";
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      std::cout << "Severity: notification";
      break;
  }
  std::cout << std::endl;
  std::cout << std::endl;
};
