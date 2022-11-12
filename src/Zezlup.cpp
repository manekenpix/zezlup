#include "Zezlup.h"

Zezlup::Zezlup()
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
  , isDisplayingPicker{ false }
{
  logger.info( "s", "Zezlup!" );

  // Read config file
  parser = new Parser( '-', '#' );
  parser->loadFile( "config.ini" );
  parser->parse();

  // Renderer
  renderer = new Renderer();
  renderer->createWindow( windowWidth, windowHeight );

  mouse = renderer->getMouse();

  loadMenuAssets();
  loadFont();
  loadAssets();
  loadTextures();
  loadShaders();
  createColourPicker();

  // TODO(Josue): This shouldn't be here. Move this to a place where all the
  // game stuff is created/loaded
  renderer->createQuad(
    "blank", previewWidth / gridWidth, previewHeight / gridHeight );
  renderer->setQuadPosition( "blank", previewX, previewY );

  renderer->createQuad(
    "gameBlank", backgroundWidth / gridWidth, backgroundHeight / gridHeight );

  // Top bar
  renderer->createQuad( "top_bar", 800, 25 );
  renderer->setQuadPosition( "top_bar", 0, 0 );
  std::copy( PURPLE.cbegin(), PURPLE.cend(), selectedColour.begin() );

  // Help panel
  renderer->createQuad( "helpPanel", previewWidth, previewHeight );
  renderer->setQuadPosition( "helpPanel", previewX, previewY );

  // Selected Colour
  renderer->createQuad(
    "selectedColour", selectedColourWidth, selectedColourHeight );
  renderer->setQuadPosition(
    "selectedColour", selectedColourX, selectedColourY );
};

void
Zezlup::createGrid()
{
  grid = new Grid( gridWidth,
                   gridHeight,
                   backgroundWidth,
                   backgroundHeight,
                   initialEmpty,
                   0,
                   25 );
  empty = grid->shuffle( initialEmpty, initialMoves );
  emptyCell = grid->getCoords( empty );

  renderer->createQuad( "cellQuad", grid->cellWidth, grid->cellHeight );
};

void
Zezlup::loadMenuAssets()
{
  logger.info( "s", "Loading menu assets..." );
  vector<string> coordsFromConfig = parser->getSectionData( COORDS );

  std::vector<std::string> coord;
  for ( string& coords : coordsFromConfig ) {
    coord = splitString( coords, ',' );
    optionsCoords.push_back(
      Vec2( std::stof( coord[0] ), std::stof( coord[1] ) ) );
  }

  // Create a background
  renderer->createQuad( "background", backgroundWidth, backgroundHeight );
  renderer->setQuadPosition( "background", 0, 25 );

  // Preview
  renderer->createQuad( "preview", previewWidth, previewHeight );
  renderer->setQuadPosition( "preview", previewX, previewY );
  logger.info( "s", "Loading menu assets: done" );
};

void
Zezlup::removeGrid()
{
  for ( const auto& cell : grid->cells ) {
    if ( cell->id != grid->Empty )
      renderer->deleteQuad( cell->id );
  }

  delete grid;
};

void
Zezlup::loadFont()
{
  logger.info( "s", "Loading fonts..." );

  vector<string> fonts = parser->getSectionData( FONTS );
  font = new Font( fonts[0].c_str() );

  for ( u8 c = '!'; c < '}'; ++c ) {
    renderer->createQuad(
      std::string( 1, c ), font->getWidth( c ), font->getHeight( c ) );

    renderer->setQuadPosition( std::string( 1, c ), 0, 0 );

    renderer->loadTexture( std::string( 1, c ),
                           font->getBuffer( c ),
                           font->getWidth( c ),
                           font->getHeight( c ) );
  }
  logger.info( "s", "Loading fonts: done" );
};

void
Zezlup::loadAssets()
{
  logger.info( "s", "Loading assets..." );
  vector<string> assetPaths = parser->getSectionData( ASSETS );

  for ( string& assetPath : assetPaths )
    assets.push_back( assetPath );

  logger.info( "s", "Loading assets: done" );
};

void
Zezlup::loadShaders()
{
  logger.info( "s", "Loading shaders..." );
  vector<string> shadersPaths = parser->getSectionData( SHADERS );

  std::vector<std::string> shader;
  for ( string& shaderPath : shadersPaths ) {
    shader = splitString( shaderPath, ',' );
    renderer->loadShader( shader[0], shader[1], shader[2] );
  }

  logger.info( "s", "Loading shaders: done" );
};

void
Zezlup::loadTextures()
{
  renderer->createQuad( "optionQuad", optionWidth, optionHeight );

  u8 index = 0;
  for ( const auto& asset : assets ) {
    images.push_back( loadImage( asset.c_str() ) );

    renderer->loadTexture( asset,
                           images[index]->getImageBuffer(),
                           images[index]->getWidth(),
                           images[index]->getHeight(),
                           images[index]->getColourType() );
    ++index;
  }
};

void
Zezlup::loadGridTextures()
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
Zezlup::removeGridTextures()
{
  u8 cells = gridHeight * gridWidth;
  for ( u8 cell = 0; cell < cells; ++cell ) {
    renderer->deleteTexture( "cell" + std::to_string( cell ) );
  }
};

void
Zezlup::createColourPicker()
{
  pickerHeight = 127 * 5;
  pickerWidth = 25;
  pickerX = 600;
  pickerY = 40;
  selectedColourWidth = 25;
  selectedColourHeight = 15;
  selectedColourX = 600;
  selectedColourY = 5;

  picker = new ColourPicker( pickerWidth, pickerHeight );

  renderer->createQuad( "colourPicker", pickerWidth, pickerHeight );
  renderer->setQuadPosition( "colourPicker", pickerX, pickerY );

  renderer->loadTexture(
    "colourPicker", picker->getBuffer(), pickerWidth, pickerHeight, 0 );
};

bool
Zezlup::getOptionSelectedWithMouse()
{
  s8 xPos = -1, yPos = -1;
  u8 index = 0;
  for ( auto it = optionsCoords.cbegin(); index < optionColumns; ++index, ++it )
    if ( mouse->x >= it->x && mouse->x < it->x + optionWidth )
      xPos = index;

  index = 0;
  for ( auto it = optionsCoords.cbegin(); index < optionRows * optionColumns;
        index += optionColumns, it += optionColumns )
    if ( mouse->y >= it->y && mouse->y < it->y + optionHeight )
      yPos = index;

  if ( xPos != -1 && yPos != -1 ) {
    optionSelected = yPos + xPos;
    return true;
  }

  return false;
};

void
Zezlup::selectOptionWithMouseClick()
{
  if ( getOptionSelectedWithMouse() )
    initializeGameplay();

  mouse->isLeftPressed = false;
};

void
Zezlup::initializeGameplay()
{
  createGrid();
  loadGridTextures();
  moves = 0;
  menuMode = false;
  startTime = glfwGetTime();
};

bool
Zezlup::isSelectedColourClicked()
{
  return mouse->x >= selectedColourX &&
         mouse->x <= ( selectedColourX + selectedColourWidth ) &&
         mouse->y >= selectedColourY &&
         mouse->y <= ( selectedColourY + selectedColourHeight );
};

bool
Zezlup::isColourPickerClicked()
{
  return mouse->x >= pickerX && mouse->x <= ( pickerX + pickerWidth ) &&
         mouse->y >= pickerY && mouse->y <= ( pickerY + pickerHeight );
};

void
Zezlup::processMenuInput()
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
        initializeGameplay();
        break;
    }

  if ( mouse->isCoordsChanged )
    getOptionSelectedWithMouse();

  if ( mouse->isLeftPressed ) {
    if ( !isDisplayingPicker && isSelectedColourClicked() ) {
      isDisplayingPicker = true;
    } else if ( isDisplayingPicker && isColourPickerClicked() ) {
      isDisplayingPicker = false;
      std::array<f32, 3> newColour =
        picker->getColour( mouse->x - pickerX, mouse->y - pickerY );
      std::copy( newColour.cbegin(), newColour.cend(), selectedColour.begin() );
    } else {
      isDisplayingPicker = false;
      selectOptionWithMouseClick();
    }
  }
};

bool
Zezlup::getCellSelectedWithMouse()
{
  s8 xPos = -1, yPos = -1;

  std::vector<Vec2*> cellsCoords;

  for ( u8 cells = 0; cells < gridWidth * gridHeight; ++cells ) {
    cellsCoords.push_back( grid->getCoords( cells ) );
  }

  u32 cellWidth = cellsCoords[1]->x;
  u32 cellHeight = cellsCoords[gridWidth]->y;

  u8 index = 0;
  for ( auto it = cellsCoords.cbegin(); index < gridWidth; ++index, ++it )
    if ( mouse->x >= ( *it )->x && mouse->x < ( *it )->x + cellWidth )
      xPos = index;

  index = 0;
  for ( auto it = cellsCoords.cbegin(); index < gridWidth * gridHeight;
        index += gridWidth, it += gridWidth )
    if ( mouse->y >= ( *it )->y && mouse->y < ( *it )->y + cellHeight )
      yPos = index;

  if ( xPos != -1 && yPos != -1 ) {
    selected = yPos + xPos;
    return true;
  }

  return false;
};

void
Zezlup::selectCellWithMouseClick()
{
  if ( getCellSelectedWithMouse() )
    shiftSelectedCell();

  mouse->isLeftPressed = false;
};

void
Zezlup::shiftSelectedCell()
{
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
};

void
Zezlup::processGameInput()
{
  if ( isPuzzleCompleted() ) {
    removeGridTextures();
    removeGrid();
    isDisplayingPreview = false;
    menuMode = true;
  }

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
      if ( !isDisplayingPreview )
        shiftSelectedCell();
      break;
  }

  if ( mouse->isCoordsChanged )
    getCellSelectedWithMouse();

  if ( mouse->isLeftPressed && !isDisplayingPreview ) {
    selectCellWithMouseClick();
  }
};

bool
Zezlup::isPuzzleCompleted()
{
  for ( u8 i = 1; i < gridWidth * gridHeight; ++i ) {
    std::string s = grid->getId( i );
    if ( s == grid->Empty || s.substr( 4 ) != std::to_string( i ) )
      return false;
  }

  return true;
}

void
Zezlup::displayHelp()
{
  renderer->draw( "helpPanel", BLUE );

  print( std::string( "Help" ), headerOffsetX, 190 );
  print( std::string( "----" ), headerOffsetX, 200 );

  // Menu
  print( std::string( "Menu" ), bodyOffsetX, 250 );
  print( std::string( "----" ), bodyOffsetX, 260 );
  print( std::string( "Use the arrow keys to move through images" ),
         bodyOffsetX,
         300 );
  print( std::string( "Enter: Select an image and start the game" ),
         bodyOffsetX,
         350 );

  // Game
  print( std::string( "Game" ), bodyOffsetX, 400 );
  print( std::string( "----" ), bodyOffsetX, 410 );
  print( std::string( "Use the arrow keys to move the selector" ),
         bodyOffsetX,
         450 );
  print( std::string( "m: Shift the selected tile to the empty space" ),
         bodyOffsetX,
         500 );
  print( std::string( "c: Display solution" ), bodyOffsetX, 550 );
  print( std::string( "Delete: Go back to the menu" ), bodyOffsetX, 600 );

  print( std::string( "Press h to close this" ), footerOffsetX, 675 );
};

void
Zezlup::displayPreview()
{
  renderer->draw( "preview", assets[optionSelected], "Grid" );
  renderer->draw( "blank", "Blank", "Grid" );
};

void
Zezlup::menu()
{
  renderer->draw( "background", assets[optionSelected], "Blur" );
  u8 index = 0;

  for ( auto asset = assets.begin(); asset != assets.end(); ++asset, ++index ) {
    renderer->setQuadPosition(
      "optionQuad", optionsCoords[index].x, optionsCoords[index].y );

    if ( index == optionSelected )
      renderer->draw( "optionQuad", *asset, "Grid", selectedColour );
    else
      renderer->draw( "optionQuad", *asset, "Grid" );
  }

  renderer->draw( "selectedColour", selectedColour );

  if ( isDisplayingPicker )
    renderer->draw( "colourPicker", "colourPicker", "Grid" );

  print( std::string( "[ Help: h ]" ), 10, 18 );
  print( std::string( "v0.0.1" ), 735, 18 );

  if ( isDisplayingHelp )
    displayHelp();
};

void
Zezlup::renderActiveCell()
{
  std::string selectedShader = isDisplayingPreview ? "Blur" : "Grid";
  renderer->setQuadPosition(
    "cellQuad", grid->cells[selected]->x, grid->cells[selected]->y );

  // TODO(Josue): We should create a static member in Grid to define the empty
  // state, like string has npos
  if ( grid->cells[selected]->id != grid->Empty )
    renderer->draw(
      "cellQuad", grid->cells[selected]->id, selectedShader, selectedColour );
  else
    renderer->draw( "cellQuad", "Blank", "Grid", selectedColour );
};

void
Zezlup::renderInactiveCells()
{
  u8 index = 0;
  std::string selectedShader = isDisplayingPreview ? "Blur" : "Grid";

  for ( auto cell = grid->cells.begin(); cell != grid->cells.end();
        ++cell, ++index ) {
    renderer->setQuadPosition( "cellQuad", ( *cell )->x, ( *cell )->y );

    if ( index != selected || isDisplayingPreview ) {
      if ( index == empty )
        renderer->draw( "cellQuad", "Blank", "Grid" );
      else
        renderer->draw( "cellQuad", ( *cell )->id, selectedShader );
    }
  }
};

void
Zezlup::displayStats()
{
  print( std::string( "Moves: " ), 10, 18 );
  print( std::to_string( moves ), 80, 18 );

  print( std::string( "Time: " ), 150, 18 );
  print(
    std::to_string( static_cast<u32>( glfwGetTime() - startTime ) ), 205, 18 );
};

void
Zezlup::shiftCell()
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

  if ( inProgress )
    renderer->setQuadPosition(
      "cellQuad", startCoordinates.x, startCoordinates.y );
  else
    renderer->setQuadPosition( "cellQuad", endCoordinates.x, endCoordinates.y );

  renderer->draw(
    "cellQuad", grid->cells[selected]->id, "Grid", selectedColour );
};

void
Zezlup::print( std::string s, u32 x, u32 y )
{
  std::string st;
  for ( auto c = s.begin(); c != s.end(); ++c ) {
    if ( *c == 32 )
      x += 10;
    else {
      st = font->getStringChar( *c );
      renderer->setQuadPosition( st, x, y - font->getBitmapTop( *c ) );

      renderer->draw( st, st, "Text", WHITE );

      x += font->getAdvanceX( *c );
    }
  }
};

void
Zezlup::displayFPS( f32& start, f32& end )
{
  print( std::string( "FPS: " ), 650, 18 );
  print( std::to_string( static_cast<u8>( 1 / ( end - start ) ) ), 685, 18 );
};

void
Zezlup::run()
{
  f32 startSeconds = glfwGetTime();
  f32 endSeconds = 0.0f;

  do {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    key = renderer->getKey();
    renderer->getMouseState();

    renderer->setQuadPosition( "top_bar", 0, 0 );
    renderer->draw( "top_bar", BLUE );

    if ( menuMode ) {
      processMenuInput();
      menu();
    } else {
      displayStats();
      renderInactiveCells();
      if ( inProgress ) {
        shiftCell();
      } else if ( isDisplayingPreview ) {
        displayPreview();
        processGameInput();
      } else {
        renderActiveCell();
        processGameInput();
      }
    }

    endSeconds = glfwGetTime();

    displayFPS( startSeconds, endSeconds );
    startSeconds = glfwGetTime();

    renderer->swapBuffers();
    renderer->pollEvents();

  } while ( key != Renderer::Keys::esc && !renderer->windowShouldClose() );
};

Zezlup::~Zezlup()
{
  std::for_each(
    images.begin(), images.end(), []( Image* image ) { delete image; } );

  delete parser;
  delete preview;
  delete background;
  delete renderer;
  delete font;
  delete picker;
};
