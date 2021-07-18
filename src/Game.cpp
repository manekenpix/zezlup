#include "include/Game.h"

Game::Game()
  : window{ nullptr }
  , preview{ nullptr }
  , background{ nullptr }
  , gridHeight{ 4 }
  , gridWidth{ 4 }
  , selected{ 0 }
  , optionSelected{ 0 }
  , empty{ 0 }
  , framerate{ 0 }
  , menuMode{ true }
  , isKeyPressed{ false }
  , key{ -1 }
  , loaded( false )
{
  // Files
  files.push_back( "data/lego_small.png" );
  files.push_back( "data/pieces_small.png" );
  files.push_back( "data/cards_small.png" );
  files.push_back( "data/cube_small.png" );
  files.push_back( "data/small_windows_small.png" );
  files.push_back( "data/open_window_small.png" );

  optionsCoords.push_back( Vec2( 100.0f, 200.0f ) );
  optionsCoords.push_back( Vec2( 400.0f, 200.0f ) );
  optionsCoords.push_back( Vec2( 700.0f, 200.0f ) );
  optionsCoords.push_back( Vec2( 100.0f, 600.0f ) );
  optionsCoords.push_back( Vec2( 400.0f, 600.0f ) );
  optionsCoords.push_back( Vec2( 700.0f, 600.0f ) );

  // Window
  window = new GameWindow( screenWidth, screenHeight );
  getRefreshRate();

  // Renderer
  renderer = new Renderer( window, screenWidth, screenHeight );
  renderer->addShader(
    "Grid", "src/Shaders/common.vert", "src/Shaders/grid.frag" );

  renderer->addShader(
    "Outline", "src/Shaders/common.vert", "src/Shaders/outline.frag" );

  renderer->addShader(
    "Blur", "src/Shaders/common.vert", "src/Shaders/gauss.frag" );

  background = new Quad( screenWidth, screenHeight );
  background->setPosition( 0, 0 );

  loadTextures();

  // Grid
  grid = new Grid( gridWidth, gridHeight, screenWidth, screenHeight, empty );

  Shuffle<Quad*> shuffle( grid->cells, gridWidth, gridHeight, empty, 200 );

  grid->cells = shuffle.run();

  grid->setPositions();

  empty = shuffle.getEmpty();

  // Preview
  preview = new Quad( previewWidth, previewHeight );
  preview->textureID = "baboon";

  preview->setPosition( 100.0f, 100.0f );

  glfwSetInputMode( window->window, GLFW_STICKY_KEYS, GLFW_TRUE );
};

void
Game::loadTextures()
{
  u8 index = 0;
  for ( auto file = files.begin(); file != files.end(); ++file, ++index ) {
    images.push_back( new Png( file->c_str() ) );

    options.push_back( new Quad( optionWidth, optionHeight ) );
    options.back()->setPosition( optionsCoords[index].x,
                                 optionsCoords[index].y );
    options.back()->textureID = *file;

    renderer->addTexture( *file,
                          images[index]->getImageBuffer(),
                          images[index]->getWidth(),
                          images[index]->getHeight(),
                          images[index]->getColourType() );
  }
};

void
Game::loadGridTextures()
{
  u8* imageBuffer = images[optionSelected]->getImageBuffer();
  u32 imageWidth = images[optionSelected]->getWidth();
  u32 imageHeight = images[optionSelected]->getHeight();
  u8 colourType = images[optionSelected]->getColourType();
  u8 bpp = colourType ? 4 : 3;

  u32 cellWidth = imageWidth / gridWidth;
  u32 cellHeight = imageHeight / gridHeight;

  u32 cellSize = cellWidth * cellHeight * bpp;

  for ( u8 rows = 0; rows < gridHeight; ++rows ) {
    for ( u8 columns = 0; columns < gridWidth; ++columns ) {

      // Extract cells from the source image
      u8* cellBuffer = (u8*)std::malloc( cellSize );
      for ( u32 y = 0; y < cellHeight; ++y ) {
        for ( u32 x = 0; x < cellWidth * bpp; ++x ) {

          u32 positionX = ( columns * cellWidth * bpp ) + x;
          u32 positionY = ( ( rows * cellHeight ) + y ) * ( imageWidth * bpp );
          u32 cellPosition = ( cellWidth * bpp * y ) + x;
          cellBuffer[cellPosition] = imageBuffer[positionY + positionX];
        }
      }
      renderer->addTexture( std::to_string( rows * gridWidth + columns ),
                            cellBuffer,
                            cellWidth,
                            cellHeight,
                            colourType );
    }
  }
  preview->textureID = files[optionSelected];
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
Game::processMenuInput()
{
  // TODO(Josue): Use a map here
  // Press right arrow
  if ( glfwGetKey( window->window, GLFW_KEY_RIGHT ) == GLFW_PRESS &&
       !isKeyPressed ) {
    if ( optionSelected < files.size() - 1 ) {
      ++optionSelected;
      isKeyPressed = true;
      key = GLFW_KEY_RIGHT;
    }

    // Press left arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_LEFT ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( optionSelected > 0 ) {
      --optionSelected;
      isKeyPressed = true;
      key = GLFW_KEY_LEFT;
    }

    // Press down arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_DOWN ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( optionSelected < 3 ) {
      optionSelected += 3;
      isKeyPressed = true;
      key = GLFW_KEY_DOWN;
    }

    // Press up arrow
  } else if ( glfwGetKey( window->window, GLFW_KEY_UP ) == GLFW_PRESS &&
              !isKeyPressed ) {
    if ( optionSelected > 2 ) {
      optionSelected -= 3;
      isKeyPressed = true;
      key = GLFW_KEY_UP;
    }

    // Press enter
  } else if ( glfwGetKey( window->window, GLFW_KEY_ENTER ) == GLFW_PRESS &&
              !isKeyPressed ) {
    isKeyPressed = true;
    key = GLFW_KEY_ENTER;
    loadGridTextures();
    menuMode = false;

  } else if ( glfwGetKey( window->window, GLFW_KEY_ESCAPE ) == GLFW_PRESS &&
              !isKeyPressed ) {
    isKeyPressed = true;
    key = GLFW_KEY_ESCAPE;

    // Release pressed key
  } else if ( glfwGetKey( window->window, key ) == GLFW_RELEASE &&
              isKeyPressed ) {
    isKeyPressed = false;
    key = -1;
  }
}

void
Game::processGameInput()
{
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

    // Pres esc
  } else if ( glfwGetKey( window->window, GLFW_KEY_ESCAPE ) == GLFW_PRESS &&
              !isKeyPressed ) {
    isKeyPressed = true;
    key = GLFW_KEY_ESCAPE;

    // Release pressed key
  } else if ( glfwGetKey( window->window, key ) == GLFW_RELEASE &&
              isKeyPressed ) {
    isKeyPressed = false;
    key = -1;
  }
}

void
Game::menu()
{
  renderer->draw( &( background->vertexArray ),
                  &( background->vertexBuffer ),
                  &background->vertices,
                  files[optionSelected],
                  "Blur" );
  u8 index = 0;
  for ( auto option = options.begin(); option != options.end();
        ++option, ++index ) {
    if ( *option ) {

      renderer->draw( &( ( *option )->vertexArray ),
                      &( ( *option )->vertexBuffer ),
                      &( *option )->vertices,
                      ( *option )->textureID,
                      index == optionSelected ? "Outline" : "Grid" );
    }
  }
};

void
Game::play()
{
  std::string selectedShader;
  u8 index = 0;
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
                      ( *cell )->textureID,
                      selectedShader );
    }
  }

  if ( key == GLFW_KEY_C )
    renderer->draw( &( preview->vertexArray ),
                    &( preview->vertexBuffer ),
                    &preview->vertices,
                    preview->textureID,
                    "Grid" );
};

void
Game::run()
{
  f32 startSeconds = glfwGetTime();
  f32 endSeconds = 0.0f;
  f32 targetElapsedTime = 1000.0f / framerate;

  do {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    if ( menuMode ) {
      processMenuInput();
      menu();
    } else {
      processGameInput();
      play();
    }

    renderer->swapBuffers();
    renderer->pollEvents();

    endSeconds = glfwGetTime();
    startSeconds = glfwGetTime();

  } while ( key != GLFW_KEY_ESCAPE );
}

Game::~Game()
{
  std::for_each(
    images.begin(), images.end(), []( Png* image ) { delete image; } );
  std::for_each(
    options.begin(), options.end(), []( Quad* option ) { delete option; } );
  delete preview;
  delete background;
  delete window;
};
