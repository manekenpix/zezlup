#include "include/Png.hpp"
#include "include/Quad.hpp"
#include "include/Shader.hpp"
#include "include/Window.hpp"

int
main( int argc, char* argv[] )
{
  if ( argc < 3 ) {
    std::cout << "Missing argument" << std::endl;
    return 1;
  }

  PNG image( argv[1] );

  const f32 screenWidth = 1000.0;
  const f32 screenHeight = 1000.0;

  Window window( screenWidth, screenHeight );

  Shader* normalShaders = new Shader( "src/Shaders/vertexShader.vert",
                                      "src/Shaders/fragmentShader.frag" );

  Quad puzzle(
    normalShaders, screenWidth, screenHeight, screenWidth, screenHeight );
  puzzle.attachTexture( image.getImageBuffer(),
                        image.getWidth(),
                        image.getHeight(),
                        image.getColourType() );

  puzzle.setPosition( 0.0, 0.0 );

  // Split the image into 16 squares
  u8 squaresPerRow = 8;
  u8 squaresPerColumn = 4;
  u8 numberOfSquares = squaresPerRow * squaresPerColumn;
  u32 squareWidth = image.getWidth() / squaresPerRow;
  u32 squareHeight = image.getHeight() / squaresPerColumn;
  u8 bytesPerPixel = image.getColourType() ? 4 : 3;
  u32 squareResolution = ( squareWidth * bytesPerPixel ) * squareHeight;

  std::cout << "squareHeight: " << squareHeight << std::endl;
  std::cout << "squareWidth: " << squareWidth << std::endl;
  std::cout << "squareResolution: " << squareResolution << std::endl;
  std::cout << "ColourType: " << (u32)image.getColourType() << std::endl;

  Quad* squares[numberOfSquares];
  u8* imageBuffer = image.getImageBuffer();

  for ( u8 rows = 0; rows < squaresPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < squaresPerRow; ++columns ) {
      u8* squareBuffer = (u8*)std::malloc( squareResolution );
      for ( u32 y = 0; y < squareHeight; ++y ) {
        for ( u32 x = 0; x < squareWidth * bytesPerPixel; ++x ) {
          u32 positionX = ( columns * squareWidth * bytesPerPixel ) + x;
          u32 positionY = ( ( rows * squareHeight ) + y ) *
                          ( image.getWidth() * bytesPerPixel );
          u32 squarePosition = ( squareWidth * bytesPerPixel * y ) + x;
          squareBuffer[squarePosition] = imageBuffer[positionY + positionX];
        }
      }
      squares[rows * squaresPerRow + columns] =
        new Quad( normalShaders,
                  screenWidth / squaresPerRow,
                  screenHeight / squaresPerColumn,
                  screenWidth,
                  screenHeight );
      squares[rows * squaresPerRow + columns]->attachTexture(
        squareBuffer, squareWidth, squareHeight, image.getColourType() );
    }
  }

  for ( u8 j = 0; j < squaresPerColumn; ++j ) {
    for ( u8 k = 0; k < squaresPerRow; ++k ) {
      std::cout << "X position: " << ( screenWidth / squaresPerRow ) * k
                << std::endl;
      std::cout << "Y position: " << ( screenHeight / squaresPerColumn ) * j
                << std::endl;
      squares[( j * squaresPerRow ) + k]->setPosition(
        ( screenWidth / squaresPerRow ) * k,
        ( screenHeight / squaresPerColumn ) * j );
    }
  }

  do {

    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    for ( u8 l = 0; l < squaresPerColumn; ++l ) {
      for ( u8 m = 0; m < squaresPerRow; ++m ) {
        if ( ( l * squaresPerRow + m ) % 2 == 0 )
          squares[( l * squaresPerRow ) + m]->bindVertexArray();
      }
    }

    // Swap buffers
    glfwSwapBuffers( window.window );
    glfwPollEvents();
  } // Check if the ESC key was pressed or the window was closed
  while ( glfwGetKey( window.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose( window.window ) == 0 );

  return 0;
};
