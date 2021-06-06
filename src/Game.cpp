#include "include/Game.hpp"

Game::Game()
{
  window = new Window( screenWidth, screenHeight );

  images.push_back( new Png( "data/true_colour/baboon.png" ) );

  shaders.push_back( new Shader( "src/Shaders/vertexShader.vert",
                                 "src/Shaders/fragmentShader.frag" ) );

  squareWidth = images[0]->getWidth() / squaresPerRow;
  squareHeight = images[0]->getHeight() / squaresPerColumn;
  bytesPerPixel = images[0]->getColourType() ? 4 : 3;
  squareResolution = ( squareWidth * bytesPerPixel ) * squareHeight;
  imageBuffer = images[0]->getImageBuffer();

  createPanel();
};

void
Game::createPanel()
{
  for ( u8 rows = 0; rows < squaresPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < squaresPerRow; ++columns ) {
      u8* squareBuffer = (u8*)std::malloc( squareResolution );
      for ( u32 y = 0; y < squareHeight; ++y ) {
        for ( u32 x = 0; x < squareWidth * bytesPerPixel; ++x ) {
          u32 positionX = ( columns * squareWidth * bytesPerPixel ) + x;
          u32 positionY = ( ( rows * squareHeight ) + y ) *
                          ( images[0]->getWidth() * bytesPerPixel );
          u32 squarePosition = ( squareWidth * bytesPerPixel * y ) + x;
          squareBuffer[squarePosition] = imageBuffer[positionY + positionX];
        }
      }
      panel.push_back( new Quad( shaders[0],
                                 screenWidth / squaresPerRow,
                                 screenHeight / squaresPerColumn,
                                 screenWidth,
                                 screenHeight ) );
      panel.back()->attachTexture(
        squareBuffer, squareWidth, squareHeight, images[0]->getColourType() );
    }
  }

  // Make a copy of the panel for comparison
  sortedPanel = panel;

  std::random_shuffle( panel.begin(), panel.end() );

  for ( u8 j = 0; j < squaresPerColumn; ++j ) {
    for ( u8 k = 0; k < squaresPerRow; ++k ) {
      panel[( j * squaresPerRow ) + k]->setPosition(
        ( screenWidth / squaresPerRow ) * k,
        ( screenHeight / squaresPerColumn ) * j );
    }
  }

  Quad* last = panel.back();
  delete last;
  panel[panel.size() - 1] = nullptr;
};

void
Game::run()
{
  do {

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    std::for_each( panel.begin(), panel.end(), []( Quad* square ) {
      if ( square )
        square->bindVertexArray();
    } );

    // Swap buffers
    glfwSwapBuffers( window->window );
    glfwPollEvents();

  } while ( glfwGetKey( window->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose( window->window ) == 0 );
}

Game::~Game()
{
  std::for_each(
    images.begin(), images.end(), []( Png* image ) { delete image; } );
  std::for_each(
    shaders.begin(), shaders.end(), []( Shader* shader ) { delete shader; } );
  std::for_each(
    panel.begin(), panel.end(), []( Quad* square ) { delete square; } );

  delete window;
};
