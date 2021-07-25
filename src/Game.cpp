#include "include/Game.h"

Game::Game()
  : preview{ nullptr }
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
  , displayPreview{ false }
{
  // Files
  files.push_back( "data/lego_small.png" );
  files.push_back( "data/cube_small.png" );
  files.push_back( "data/pieces_small.png" );
  files.push_back( "data/small_windows_small.png" );
  files.push_back( "data/open_window_small.png" );
  files.push_back( "data/cards_small.png" );

  optionsCoords.push_back( Vec2( 100.0f, 200.0f ) );
  optionsCoords.push_back( Vec2( 400.0f, 200.0f ) );
  optionsCoords.push_back( Vec2( 700.0f, 200.0f ) );
  optionsCoords.push_back( Vec2( 100.0f, 600.0f ) );
  optionsCoords.push_back( Vec2( 400.0f, 600.0f ) );
  optionsCoords.push_back( Vec2( 700.0f, 600.0f ) );

  // Renderer
  renderer = new Renderer();
  renderer->createWindow( windowWidth, windowHeight );

  background = new Quad( windowWidth, windowHeight );
  background->setPosition( 0, 0 );

  loadTextures();

  // Grid
  grid = new Grid( gridWidth, gridHeight, windowWidth, windowHeight, empty );

  Shuffle<Quad*> shuffle( grid->cells, gridWidth, gridHeight, empty, 200 );

  grid->cells = shuffle.run();

  grid->setPositions();

  empty = shuffle.getEmpty();

  // Preview
  preview = new Quad( previewWidth, previewHeight );
  preview->textureID = "baboon";

  preview->setPosition( 100.0f, 100.0f );

  renderer->addShader(
    "Grid", "src/Shaders/common.vert", "src/Shaders/grid.frag" );

  renderer->addShader(
    "Outline", "src/Shaders/common.vert", "src/Shaders/outline.frag" );

  renderer->addShader(
    "Blur", "src/Shaders/common.vert", "src/Shaders/gauss.frag" );
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
  if ( key == "right" ) {
    if ( optionSelected < files.size() - 1 ) {
      ++optionSelected;
    }

  } else if ( key == "left" ) {
    if ( optionSelected > 0 ) {
      --optionSelected;
    }

  } else if ( key == "down" ) {
    if ( optionSelected < 3 ) {
      optionSelected += 3;
    }

  } else if ( key == "up" ) {
    if ( optionSelected > 2 ) {
      optionSelected -= 3;
    }

  } else if ( key == "enter" ) {
    loadGridTextures();
    menuMode = false;
  }
}

void
Game::processGameInput()
{
  if ( key == "right" ) {
    if ( selected < gridWidth * gridHeight - 1 ) {
      ++selected;
    }

  } else if ( key == "left" ) {
    if ( selected > 0 ) {
      --selected;
    }

  } else if ( key == "down" ) {
    if ( selected < gridWidth * ( gridHeight - 1 ) ) {
      selected += gridWidth;
    }

  } else if ( key == "up" ) {
    if ( selected > gridWidth - 1 ) {
      selected -= gridWidth;
    }

  } else if ( key == "m" ) {
    s8 distanceBetweenBoxes = abs( selected - empty );

    if ( distanceBetweenBoxes == 1 || distanceBetweenBoxes == gridWidth ) {
      grid->swapCells( selected, empty );
      std::swap( selected, empty );
    }
  } else if ( key == "c" ) {
    displayPreview = !displayPreview;
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

      if ( displayPreview )
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

  if ( displayPreview )
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

    key = renderer->getKey();

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

  } while ( key != "esc" );
}

Game::~Game()
{
  std::for_each(
    images.begin(), images.end(), []( Png* image ) { delete image; } );
  std::for_each(
    options.begin(), options.end(), []( Quad* option ) { delete option; } );
  delete preview;
  delete background;
};
