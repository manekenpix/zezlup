#include "include/Window.h"

// ZEZLUP

GameWindow::GameWindow( f32 w, f32 h )
{
  width = w;
  height = h;

  // Initialise GLFW
  if ( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
    // return -1;
  }
  WindowSetup();

  glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( width, height, "Zezlup", NULL, NULL );

  if ( window == NULL ) {
    std::cout << "Failed to open GLFW window." << std::endl;
    getchar();
    glfwTerminate();
    // return -1;
  }

  glfwMakeContextCurrent( window );

  glViewport( 0, 0, width, height );

  glewExperimental = GL_TRUE;
  glewInit();

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
};

GameWindow::~GameWindow()
{
  // Close OpenGL window and terminate GLFW
  glfwTerminate();
};

void
GameWindow::WindowSetup()
{
  // 4x antialiasing
  glfwWindowHint( GLFW_SAMPLES, 4 );

  // No resizable window
  glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

  // OpenG 3
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

  // To make MacOS happy; should not be needed
  // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

  // Don't use the old OpenGL
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
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
}
