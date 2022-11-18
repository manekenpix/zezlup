#include "Renderer.h"

Renderer::Renderer()
  : isKeyPressed{ false }
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
    logger.error( "s", "Failed to open GLFW window." );
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

  auto icon = loadImage( "data/images/logo.png" );

  GLFWimage images[1];

  images[0].width = icon->getWidth();
  images[0].height = icon->getHeight();
  images[0].pixels = icon->getImageBuffer();

  glfwSetWindowIcon( window, 1, images );

  return window;
};

bool
Renderer::windowShouldClose() const
{
  return glfwWindowShouldClose( window );
};

void
Renderer::draw( std::string quad,
                std::string texture,
                std::string shader,
                std::array<f32, 3> colour )
{
  shaders[shader]->use();
  shaders[shader]->setMatrix4fv( "projection", projectionMatrix );
  shaders[shader]->set3vf( "passed_colour", colour );
  shaders[shader]->setBool( "has_border", true );

  if ( texture != "Blank" ) {
    shaders[shader]->setBool( "has_texture", true );
    textures[texture]->bind();
  } else
    shaders[shader]->setBool( "has_texture", false );

  quads[quad]->bind();
};

void
Renderer::draw( std::string quad, std::string texture, std::string shader )
{
  shaders[shader]->use();
  shaders[shader]->setMatrix4fv( "projection", projectionMatrix );
  shaders[shader]->setBool( "has_border", false );

  if ( texture != "Blank" ) {
    shaders[shader]->setBool( "has_texture", true );
    textures[texture]->bind();
  } else
    shaders[shader]->setBool( "has_texture", false );

  quads[quad]->bind();
};

void
Renderer::draw( std::string quad, std::array<f32, 3> colour )
{
  shaders["Solid"]->use();
  shaders["Solid"]->setMatrix4fv( "projection", projectionMatrix );
  shaders["Solid"]->set3vf( "passed_colour", colour );

  quads[quad]->bind();
};

void
Renderer::clearColor( const std::array<f32, 3> colour ) const
{
  glClearColor( colour[0], colour[1], colour[2], 1.0f );
};

void
Renderer::clear() const
{
  glClear( GL_COLOR_BUFFER_BIT );
};

void
Renderer::swapBuffers() const
{
  glfwSwapBuffers( window );
};

void
Renderer::pollEvents() const
{
  glfwPollEvents();
};

f32
Renderer::getTime() const
{
  return glfwGetTime();
};

void
Renderer::loadShader( std::string key,
                      std::string vShader,
                      std::string fShader )
{
  shaders.insert( { key, new Shader( vShader.c_str(), fShader.c_str() ) } );
};

void
Renderer::loadTexture( std::string name,
                       u8* buffer,
                       s32 width,
                       s32 height,
                       u8 colourType )
{
  textures.insert( { name, new Texture( buffer, width, height, colourType ) } );
};

void
Renderer::loadTexture( std::string name, u8* buffer, s32 width, s32 height )
{
  textures.insert( { name, new Texture( buffer, width, height ) } );
};

void
Renderer::deleteTexture( std::string texture )
{
  delete textures[texture];
  textures.erase( texture );
};

void
Renderer::createQuad( std::string name, f32 width, f32 height )
{
  quads.insert( { name, new Quad( width, height ) } );
};

void
Renderer::deleteQuad( std::string name )
{
  delete quads[name];
  quads.erase( name );
};

void
Renderer::setQuadPosition( std::string key, f32 x, f32 y )
{
  quads[key]->setPosition( x, y );
};

Renderer::Keys
Renderer::getKey()
{
  u8 index = 0;

  for ( ; !isKeyPressed && index < totalKeys; ++index ) {
    if ( glfwGetKey( window, keys[index].code ) == GLFW_PRESS ) {
      isKeyPressed = true;
      pressedKey = keys[index].code;
      return keys[index].key;
    }
  }

  if ( isKeyPressed && glfwGetKey( window, pressedKey ) == GLFW_RELEASE )
    isKeyPressed = false;

  return Keys::blank;
};

void
Renderer::getMouseState()
{
  double x, y;
  glfwGetCursorPos( window, &x, &y );

  mouse.isCoordsChanged = !( mouse.x == x && mouse.y == y );
  mouse.isLeftPressed =
    glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS;
  mouse.isRightPressed =
    glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS;

  if ( mouse.isCoordsChanged ) {
    mouse.x = x;
    mouse.y = y;
  }
};

Renderer::Mouse*
Renderer::getMouse()
{
  return &mouse;
};

Renderer::~Renderer()
{
  for ( const auto& [key, v] : shaders ) {
    delete v;
  }

  for ( const auto& [key, v] : textures ) {
    delete v;
  }

  for ( const auto& [key, v] : quads ) {
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
  Logger logger;
  // ignore non-significant error/warning codes
  if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 )
    return;

  logger.error( "sds", "Debug message (", id, "): " );

  switch ( source ) {
    case GL_DEBUG_SOURCE_API:
      logger.error( "s", "Source: API" );
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      logger.error( "s", "Source: Window System" );
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      logger.error( "s", "Source: Shader Compiler" );
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      logger.error( "s", "Source: Third Party" );
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      logger.error( "s", "Source: Application" );
      break;
    case GL_DEBUG_SOURCE_OTHER:
      logger.error( "s", "Source: Other" );
      break;
  }

  switch ( type ) {
    case GL_DEBUG_TYPE_ERROR:
      logger.error( "s", "Type: Error" );
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      logger.error( "s", "Type: Deprecated Behaviour" );
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      logger.error( "s", "Type: Undefined Behaviour" );
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      logger.error( "s", "Type: Portability" );
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      logger.error( "s", "Type: Performance" );
      break;
    case GL_DEBUG_TYPE_MARKER:
      logger.error( "s", "Type: Marker" );
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      logger.error( "s", "Type: Push Group" );
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      logger.error( "s", "Type: Pop Group" );
      break;
    case GL_DEBUG_TYPE_OTHER:
      logger.error( "s", "Type: Other" );
      break;
  }

  switch ( severity ) {
    case GL_DEBUG_SEVERITY_HIGH:
      logger.error( "s", "Severity: high" );
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      logger.error( "s", "Severity: medium" );
      break;
    case GL_DEBUG_SEVERITY_LOW:
      logger.error( "s", "Severity: low" );
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      logger.error( "s", "Severity: notification" );
      break;
  }
};
