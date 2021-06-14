#include "include/Game.h"

Game::Game()
  : window{ nullptr }
  , imageBuffer{ nullptr }
  , squaresPerRow{ 4 }
  , squaresPerColumn{ 4 }
  , squareWidth{ 0 }
  , squareHeight{ 0 }
  , bytesPerPixel{ 0 }
  , squareResolution{ 0 }
  , selected{ 0 }
  , empty{ 0 }
  , framerate{ 0 }
  , isKeyPressed{ false }
  , key{ -1 }
{
  window = new GameWindow( screenWidth, screenHeight );
  getRefreshRate();

  images.push_back( new Png( "data/true_colour/baboon.png" ) );

  shaders.push_back(
    new Shader( "src/Shaders/common.vert", "src/Shaders/grid.frag" ) );

  shaders.push_back(
    new Shader( "src/Shaders/common.vert", "src/Shaders/outline.frag" ) );

  squareWidth = images[0]->getWidth() / squaresPerRow;
  squareHeight = images[0]->getHeight() / squaresPerColumn;
  bytesPerPixel =
    images[0]->getColourType() ? BytesPerPixel::RGBA : BytesPerPixel::RGB;
  squareResolution = ( squareWidth * bytesPerPixel ) * squareHeight;
  imageBuffer = images[0]->getImageBuffer();

  createPanel();

  glfwSetInputMode( window->window, GLFW_STICKY_KEYS, GLFW_TRUE );
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
      panel.push_back( new Quad( shaders,
                                 screenWidth / squaresPerRow,
                                 screenHeight / squaresPerColumn,
                                 screenWidth,
                                 screenHeight ) );
      panel.back()->attachTexture(
        squareBuffer, squareWidth, squareHeight, images[0]->getColourType() );
    }
  }

  Quad* last = panel.back();
  delete last;
  panel[panel.size() - 1] = nullptr;

  // Make a copy of the panel for comparison
  sortedPanel = panel;

  std::random_shuffle( panel.begin(), panel.end() );

  f32 boxPositionX, boxPositionY;
  for ( u8 j = 0; j < squaresPerColumn; ++j ) {
    for ( u8 k = 0; k < squaresPerRow; ++k ) {
      boxPositionX = ( screenWidth / squaresPerRow ) * k;
      boxPositionY = ( screenHeight / squaresPerColumn ) * j;
      coords.push_back( new Vec2( boxPositionX, boxPositionY ) );
      if ( panel[( j * squaresPerRow ) + k] ) {
        panel[( j * squaresPerRow ) + k]->setPosition( boxPositionX,
                                                       boxPositionY );
      }
    }
  }
};

void
Game::getRefreshRate()
{
  Display* display = XOpenDisplay( NULL );
  Window defaultWindow = XDefaultRootWindow( display );

  XRRScreenResources* screenResources =
    XRRGetScreenResources( display, defaultWindow );

  RRMode activeModeID = 0;
  for ( u8 i = 0; i < screenResources->ncrtc; ++i ) {
    XRRCrtcInfo* crtcInfo =
      XRRGetCrtcInfo( display, screenResources, screenResources->crtcs[i] );
    // If None, then is not displaying the screen contents
    if ( crtcInfo->mode != None ) {
      activeModeID = crtcInfo->mode;
    }
  }

  for ( u8 i = 0; i < screenResources->nmode; ++i ) {
    XRRModeInfo modeInfo = screenResources->modes[i];
    if ( modeInfo.id == activeModeID ) {
      framerate = (double)modeInfo.dotClock /
                  ( (double)modeInfo.hTotal * (double)modeInfo.vTotal );
    }
  }

  std::cout << "Active rate is: " << framerate << std::endl;
}

void
Game::processKeyboardInput()
{
  // TODO(Josue): Use a map here
  // Press right arrow
  if ( glfwGetKey( window->window, GLFW_KEY_RIGHT ) == GLFW_PRESS &&
       !isKeyPressed ) {
    if ( selected < squaresPerRow * squaresPerColumn - 1 ) {
      ++selected;
      isKeyPressed = true;
      key = GLFW_KEY_RIGHT;
    }

    // Press left arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_LEFT ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( selected > 0 ) {
      --selected;
      isKeyPressed = true;
      key = GLFW_KEY_LEFT;
    }

    // Press down arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_DOWN ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( selected < squaresPerRow * ( squaresPerColumn - 1 ) ) {
      selected += squaresPerRow;
      isKeyPressed = true;
      key = GLFW_KEY_DOWN;
    }

    // Press up arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_UP ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( selected > squaresPerRow - 1 ) {
      selected -= squaresPerRow;
      isKeyPressed = true;
      key = GLFW_KEY_UP;
    }

    // Press 'm'
  } else if ( glfwGetKey( window->window, GLFW_KEY_M ) == GLFW_PRESS &&
              !isKeyPressed ) {
    s8 distanceBetweenBoxes = abs( selected - empty );
    isKeyPressed = true;
    key = GLFW_KEY_M;

    if ( distanceBetweenBoxes == 1 || distanceBetweenBoxes == squaresPerRow ) {
      panel[empty] = panel[selected];
      panel[selected] = nullptr;
      std::swap( selected, empty );

      panel[selected]->setPosition( coords[selected]->x, coords[selected]->y );
    }

    // Release pressed key
  } else if ( glfwGetKey( window->window, key ) == GLFW_RELEASE &&
              isKeyPressed ) {
    isKeyPressed = false;
    key = -1;
  }
}

void
Game::run()
{

  u8 index;

  f32 startSeconds = glfwGetTime();
  f32 endSeconds = 0.0f;
  f32 targetElapsedTime = 1000.0f / framerate;

  do {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    processKeyboardInput();

    index = 0;
    for ( auto box = panel.begin(); box != panel.end(); ++box ) {
      if ( *box )
        ( *box )->draw( selected == index );
      else
        empty = index;

      ++index;
    }

    // Swap buffers
    glfwSwapBuffers( window->window );
    glfwPollEvents();

    endSeconds = glfwGetTime();
    startSeconds = glfwGetTime();

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
  std::for_each(
    coords.begin(), coords.end(), []( Vec2* coord ) { delete coord; } );

  delete window;
};
