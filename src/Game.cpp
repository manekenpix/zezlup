#include "include/Game.h"

Game::Game()
  : window{ nullptr }
  , preview{ nullptr }
  , gridHeight{ 4 }
  , gridWidth{ 4 }
  , selected{ 0 }
  , empty{ 0 }
  , framerate{ 0 }
  , isKeyPressed{ false }
  , key{ -1 }
{
  window = new GameWindow( screenWidth, screenHeight );
  getRefreshRate();

  renderer = new Renderer( window, screenWidth, screenHeight );
  renderer->addShader(
    "Grid", "src/Shaders/common.vert", "src/Shaders/grid.frag" );

  renderer->addShader(
    "Outline", "src/Shaders/common.vert", "src/Shaders/outline.frag" );

  renderer->addShader(
    "Blur", "src/Shaders/common.vert", "src/Shaders/gauss.frag" );

  images.push_back( new Png( "data/true_colour/baboon.png" ) );

  grid = new Grid( images[0], gridWidth, gridHeight, 1000.0f, 1000.0f, empty );

  Shuffle<Quad*> shuffle( grid->cells, gridWidth, gridHeight, empty, 200 );

  grid->cells = shuffle.run();

  grid->setPositions();

  empty = shuffle.getEmpty();

  // Create the preview
  preview = new Quad( previewWidth, previewHeight, screenWidth, screenHeight );
  preview->tex = new Texture( images[0]->getImageBuffer(),
                              images[0]->getWidth(),
                              images[0]->getHeight(),
                              images[0]->getColourType() );

  preview->setPosition( 100.0f, 100.0f );

  glfwSetInputMode( window->window, GLFW_STICKY_KEYS, GLFW_TRUE );
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
    if ( selected < gridWidth * gridHeight - 1 ) {
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
    if ( selected < gridWidth * ( gridHeight - 1 ) ) {
      selected += gridWidth;
      isKeyPressed = true;
      key = GLFW_KEY_DOWN;
    }

    // Press up arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_UP ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( selected > gridWidth - 1 ) {
      selected -= gridWidth;
      isKeyPressed = true;
      key = GLFW_KEY_UP;
    }

    // Press 'm'
  } else if ( glfwGetKey( window->window, GLFW_KEY_M ) == GLFW_PRESS &&
              !isKeyPressed ) {
    s8 distanceBetweenBoxes = abs( selected - empty );
    isKeyPressed = true;
    key = GLFW_KEY_M;

    if ( distanceBetweenBoxes == 1 || distanceBetweenBoxes == gridWidth ) {
      grid->swapCells( selected, empty );
      std::swap( selected, empty );
    }

    // Press 'c'
  } else if ( glfwGetKey( window->window, GLFW_KEY_C ) == GLFW_PRESS &&
              !isKeyPressed ) {
    isKeyPressed = true;
    key = GLFW_KEY_C;

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

  std::string selectedShader;

  do {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    processKeyboardInput();

    index = 0;
    for ( auto cell = grid->cells.begin(); cell != grid->cells.end();
          ++cell, ++index ) {
      if ( *cell ) {

        if ( key == GLFW_KEY_C )
          selectedShader = "Blur";
        else
          selectedShader = selected == index ? "Outline" : "Grid";

        renderer->draw( &( ( *cell )->vertexArray ),
                        &( ( *cell )->vertexBuffer ),
                        &( *cell )->vertices,
                        ( *cell )->tex,
                        selectedShader );

      } else
        empty = index;

      // ++index;
    }

    if ( key == GLFW_KEY_C )
      renderer->draw( &( preview->vertexArray ),
                      &( preview->vertexBuffer ),
                      &preview->vertices,
                      preview->tex,
                      "Grid" );

    renderer->swapBuffers();
    renderer->pollEvents();

    endSeconds = glfwGetTime();
    startSeconds = glfwGetTime();

  } while ( glfwGetKey( window->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose( window->window ) == 0 );
}

Game::~Game()
{
  std::for_each(
    images.begin(), images.end(), []( Png* image ) { delete image; } );
  delete preview;
  delete window;
};
