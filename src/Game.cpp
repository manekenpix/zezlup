#include "include/Game.h"

Game::Game()
  : preview{ nullptr }
  , background{ nullptr }
  , gridHeight{ 4 }
  , gridWidth{ 4 }
  , selected{ 0 }
  , optionSelected{ 0 }
  , framerate{ 0 }
  , menuMode{ true }
  , isKeyPressed{ false }
  , key{ Renderer::Keys::blank }
  , isDisplayingPreview{ false }
  , isDisplayingHelp{ false }
{
  // Renderer
  renderer = new Renderer();
  renderer->createWindow( windowWidth, windowHeight );

  loadCoordinates();
  loadFont();
  loadAssets();
  loadTextures();
  loadShaders();

  // Create a background
  renderer->createQuad( "background", backgroundWidth, backgroundHeight );
  renderer->setQuadPosition( "background", 0, 25 );

  // Preview
  renderer->createQuad( "preview", previewWidth, previewHeight );
  renderer->setQuadPosition( "preview", previewX, previewY );

  renderer->createQuad(
    "blank", previewWidth / gridWidth, previewHeight / gridHeight );
  renderer->setQuadPosition( "blank", previewX, previewY );

  renderer->createQuad(
    "gameBlank", backgroundWidth / gridWidth, backgroundHeight / gridHeight );
};

void
Game::createGrid()
{
  grid = new Grid( gridWidth,
                   gridHeight,
                   backgroundWidth,
                   backgroundHeight,
                   initialEmpty,
                   0,
                   25 );
  empty = grid->shuffle( initialEmpty, 20 );
  emptyCell = grid->getCoords( empty );

  for ( auto cell = grid->cells.begin(); cell != grid->cells.end(); ++cell ) {
    if ( *cell ) {
      renderer->createQuad( ( *cell )->id, grid->cellWidth, grid->cellHeight );
      renderer->setQuadPosition( ( *cell )->id, ( *cell )->x, ( *cell )->y );
    }
  }
};

void
Game::removeGrid()
{
  for ( auto cell = grid->cells.begin(); cell != grid->cells.end(); ++cell ) {
    if ( *cell )
      renderer->deleteQuad( ( *cell )->id );
  }

  delete grid;
};

void
Game::loadCoordinates()
{
  optionsCoords.push_back( Vec2( 50.0f, 183.0f ) );
  optionsCoords.push_back( Vec2( 300.0f, 183.0f ) );
  optionsCoords.push_back( Vec2( 550.0f, 183.0f ) );
  optionsCoords.push_back( Vec2( 50.0f, 516.0f ) );
  optionsCoords.push_back( Vec2( 300.0f, 516.0f ) );
  optionsCoords.push_back( Vec2( 550.0f, 516.0f ) );
};

void
Game::loadFont()
{
  const char* fontFile = "data/fonts/Tinos.ttf";
  if ( FT_Init_FreeType( &ft ) ) {
    logger.info( "s", "ERROR::FREETYPE: Could not init FreeType Library\n" );
    throw;
  }

  if ( FT_New_Face( ft, fontFile, 0, &face ) ) {
    logger.info( "s", "ERROR::FREETYPE: Failed to load font\n" );
    throw;
  }

  logger.info( "sss", "Loading font: ", fontFile, "\n" );

  FT_Set_Pixel_Sizes( face, 0, 20 );

  for ( u8 c = '!'; c < '}'; ++c ) {
    if ( FT_Load_Char( face, c, FT_LOAD_RENDER ) ) {
      logger.info( "s", "ERROR::FREETYTPE: Failed to load Glyph\n" );
    }

    font.push_back( { c,
                      static_cast<u32>( ( face->glyph->advance.x ) >> 6 ),
                      static_cast<u32>( ( face->glyph->advance.y ) >> 6 ),
                      static_cast<u32>( face->glyph->bitmap_top ) } );
    renderer->createQuad( std::string( 1, c ),
                          face->glyph->bitmap.width,
                          face->glyph->bitmap.rows );
    renderer->setQuadPosition( std::string( 1, c ), 0, 0 );

    renderer->loadTexture( std::string( 1, c ),
                           face->glyph->bitmap.buffer,
                           face->glyph->bitmap.width,
                           face->glyph->bitmap.rows );
  }
};

void
Game::loadAssets()
{
  assets.push_back( "data/lego_small.png" );
  assets.push_back( "data/cube_small.png" );
  assets.push_back( "data/pieces_small.png" );
  assets.push_back( "data/small_windows_small.png" );
  assets.push_back( "data/open_window_small.png" );
  assets.push_back( "data/cards_small.png" );
};

void
Game::loadShaders()
{
  logger.info( "s", "Loading shaders" );
  renderer->loadShader(
    "Grid", "src/Shaders/common.vert", "src/Shaders/grid.frag" );

  renderer->loadShader(
    "Outline", "src/Shaders/common.vert", "src/Shaders/outline.frag" );

  renderer->loadShader(
    "Blur", "src/Shaders/common.vert", "src/Shaders/gauss.frag" );

  renderer->loadShader(
    "Blank", "src/Shaders/common.vert", "src/Shaders/blank.frag" );

  renderer->loadShader( "Blank_outline",
                        "src/Shaders/common.vert",
                        "src/Shaders/blank_outline.frag" );

  renderer->loadShader(
    "Text", "src/Shaders/text.vert", "src/Shaders/text.frag" );
};

void
Game::loadTextures()
{
  logger.info( "s", "Loading textures" );
  u8 index = 0;
  for ( auto asset = assets.begin(); asset != assets.end(); ++asset, ++index ) {
    images.push_back( new Png( asset->c_str() ) );

    renderer->createQuad( *asset, optionWidth, optionHeight );
    renderer->setQuadPosition(
      *asset, optionsCoords[index].x, optionsCoords[index].y );

    renderer->loadTexture( *asset,
                           images[index]->getImageBuffer(),
                           images[index]->getWidth(),
                           images[index]->getHeight(),
                           images[index]->getColourType() );
  }

  // Top bar
  auto topBar = new Png( "data/top_bar.png" );
  renderer->createQuad( "top_bar", topBar->getWidth(), topBar->getHeight() );
  renderer->setQuadPosition( "top_bar", 0, 0 );
  renderer->loadTexture( "top_bar",
                         topBar->getImageBuffer(),
                         topBar->getWidth(),
                         topBar->getHeight(),
                         topBar->getColourType() );

  auto helpPanel = new Png( "data/help_panel.png" );
  renderer->createQuad( "helpPanel", previewWidth, previewHeight );
  renderer->setQuadPosition( "helpPanel", previewX, previewY );
  renderer->loadTexture( "helpPanel",
                         helpPanel->getImageBuffer(),
                         helpPanel->getWidth(),
                         helpPanel->getHeight(),
                         helpPanel->getColourType() );
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
      renderer->loadTexture( "cell" +
                               std::to_string( rows * gridWidth + columns ),
                             cellBuffer,
                             cellWidth,
                             cellHeight,
                             colourType );
      std::free( cellBuffer );
    }
  }
};

void
Game::removeGridTextures()
{
  u8 cells = gridHeight * gridWidth;
  for ( u8 cell = 0; cell < cells; ++cell ) {
    renderer->deleteTexture( "cell" + std::to_string( cell ) );
  }
};

void
Game::processMenuInput()
{
  if ( key == Renderer::Keys::help )
    isDisplayingHelp = !isDisplayingHelp;

  if ( !isDisplayingHelp )
    // TODO(Josue): Use a map here
    switch ( key ) {
      case Renderer::Keys::right:
        if ( optionSelected < assets.size() - 1 )
          ++optionSelected;
        break;

      case Renderer::Keys::left:
        if ( optionSelected > 0 )
          --optionSelected;
        break;

      case Renderer::Keys::down:
        if ( optionSelected < 3 )
          optionSelected += 3;
        break;

      case Renderer::Keys::up:
        if ( optionSelected > 2 )
          optionSelected -= 3;
        break;

      case Renderer::Keys::enter:
        createGrid();
        loadGridTextures();
        startTime = glfwGetTime();
        menuMode = false;
        break;
    }
};

void
Game::processGameInput()
{
  switch ( key ) {
    case Renderer::Keys::right:
      if ( selected < gridWidth * gridHeight - 1 )
        ++selected;
      break;

    case Renderer::Keys::left:
      if ( selected > 0 )
        --selected;
      break;

    case Renderer::Keys::down:
      if ( selected < gridWidth * ( gridHeight - 1 ) )
        selected += gridWidth;
      break;

    case Renderer::Keys::up:
      if ( selected > gridWidth - 1 )
        selected -= gridWidth;
      break;

    case Renderer::Keys::c:
      isDisplayingPreview = !isDisplayingPreview;
      break;

    case Renderer::Keys::backspace:
      removeGridTextures();
      removeGrid();
      isDisplayingPreview = false;
      menuMode = true;

      break;

    case Renderer::Keys::m:
      s8 distanceBetweenBoxes = abs( selected - empty );

      if ( distanceBetweenBoxes == 1 || distanceBetweenBoxes == gridWidth ) {
        startCoordinates = ( *grid->cells[selected] );
        grid->swapCells( selected, empty );
        std::swap( selected, empty );
        endCoordinates = ( *grid->cells[selected] );
        emptyCell = grid->getCoords( empty );

        ++moves;

        if ( startCoordinates.x > endCoordinates.x )
          direction = Directions::left;
        else if ( startCoordinates.x < endCoordinates.x )
          direction = Directions::right;
        else if ( startCoordinates.y > endCoordinates.y )
          direction = Directions::up;
        else if ( startCoordinates.y < endCoordinates.y )
          direction = Directions::down;

        inProgress = true;
      }
      break;
  }
};

void
Game::displayHelp()
{
  renderer->draw( "helpPanel", "helpPanel", "Grid" );

  print( std::string( "Help" ), 350, 190 );
  print( std::string( "------" ), 350, 200 );

  // Menu
  print( std::string( "Menu" ), 200, 250 );
  print( std::string( "-------" ), 200, 260 );
  print( std::string( "Use the arrow keys to move through images" ), 200, 300 );
  print( std::string( "Enter: Select an image and start the game" ), 200, 350 );

  // Game
  print( std::string( "Game" ), 200, 400 );
  print( std::string( "-------" ), 200, 410 );
  print( std::string( "Use the arrow keys to move the selector" ), 200, 450 );
  print(
    std::string( "m: Shift the selected tile to the empty space" ), 200, 500 );
  print( std::string( "c: Display solution" ), 200, 550 );
  print( std::string( "Delete: Go back to the menu" ), 200, 600 );

  print( std::string( "Press h to close this" ), 285, 675 );
};

void
Game::displayPreview()
{
  renderer->draw( "preview", assets[optionSelected], "Grid" );
  renderer->draw( "blank", "Blank" );
};

void
Game::menu()
{
  renderer->draw( "background", assets[optionSelected], "Blur" );
  u8 index = 0;
  for ( auto asset = assets.begin(); asset != assets.end(); ++asset, ++index ) {
    renderer->draw(
      *asset, *asset, index == optionSelected ? "Outline" : "Grid" );
  }

  print( std::string( "Zezlup " ), 10, 18 );
  print( std::string( "[ Help: h ] " ), 600, 18 );

  if ( isDisplayingHelp )
    displayHelp();
};

void
Game::play()
{
  std::string selectedShader;
  u8 index = 0;
  for ( auto cell = grid->cells.begin(); cell != grid->cells.end();
        ++cell, ++index ) {
    if ( *cell ) {

      if ( isDisplayingPreview )
        selectedShader = "Blur";
      else
        selectedShader = selected == index ? "Outline" : "Grid";

      renderer->draw( ( *cell )->id, ( *cell )->id, selectedShader );
    }
    if ( selected == empty ) {
      renderer->setQuadPosition( "gameBlank", emptyCell->x, emptyCell->y );
      renderer->draw( "gameBlank", "Blank_outline" );
    }
  }

  print( std::string( "Moves: " ), 10, 18 );
  print( std::to_string( moves ), 80, 18 );

  print( std::string( "Time: " ), 150, 18 );
  print(
    std::to_string( static_cast<u32>( glfwGetTime() - startTime ) ), 205, 18 );

  if ( isDisplayingPreview ) {
    displayPreview();
  }
};

void
Game::shiftCell()
{
  switch ( direction ) {
    case Directions::left:
      startCoordinates.x -= shiftOffset;
      inProgress = ( startCoordinates.x > endCoordinates.x );
      break;

    case Directions::right:
      startCoordinates.x += shiftOffset;
      inProgress = ( startCoordinates.x < endCoordinates.x );
      break;

    case Directions::up:
      startCoordinates.y -= shiftOffset;
      inProgress = ( startCoordinates.y > endCoordinates.y );
      break;

    case Directions::down:
      startCoordinates.y += shiftOffset;
      inProgress = ( startCoordinates.y < endCoordinates.y );
      break;
  }

  inProgress ? renderer->setQuadPosition(
                 startCoordinates.id, startCoordinates.x, startCoordinates.y )
             : renderer->setQuadPosition(
                 endCoordinates.id, endCoordinates.x, endCoordinates.y );
};

void
Game::print( std::string s, u32 x, u32 y )
{
  u8 st;
  for ( auto c = s.begin(); c != s.end(); ++c ) {
    if ( *c == 32 )
      x += 10;
    else {
      st = *c - 33;
      renderer->setQuadPosition(
        font[st].character, x, y - font[st].bitmapTop );
      renderer->draw( font[st].character, font[st].character, "Text" );
      x += font[st].advanceX;
    }
  }
};

void
Game::run()
{
  f32 startSeconds = glfwGetTime();
  f32 endSeconds = 0.0f;

  do {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    key = renderer->getKey();

    renderer->draw( "top_bar", "top_bar", "Grid" );
    if ( menuMode ) {
      processMenuInput();
      menu();
    } else {
      play();
      if ( inProgress ) {
        shiftCell();
      } else {
        processGameInput();
      }
    }

    endSeconds = glfwGetTime();
    print( std::string( "FPS: " ), 730, 18 );
    print(
      std::to_string( static_cast<u8>( 1 / ( endSeconds - startSeconds ) ) ),
      775,
      20 );
    startSeconds = glfwGetTime();

    renderer->swapBuffers();
    renderer->pollEvents();

  } while ( key != Renderer::Keys::esc );
};

Game::~Game()
{
  FT_Done_Face( face );
  FT_Done_FreeType( ft );

  std::for_each(
    images.begin(), images.end(), []( Png* image ) { delete image; } );
  std::for_each(
    options.begin(), options.end(), []( Quad* option ) { delete option; } );

  delete preview;
  delete background;
  delete renderer;
};
