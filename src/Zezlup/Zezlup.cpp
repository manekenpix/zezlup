#include "Zezlup.h"

Zezlup::Zezlup()
  : leftMouseAlreadyClicked{ false }
  , rightMouseAlreadyClicked{ false }
  , gridWidth{ 4 }
  , gridHeight{ 4 }
  , framerate{ 0 }
  , preview{ nullptr }
  , isDisplayingPreview{ false }
  , background{ nullptr }
  , isDisplayingPicker{ false }
  , selected{ 0 }
  , optionSelected{ 0 }
  , menuMode{ true }
  , isKeyPressed{ false }
  , key{ Keys::blank }
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

  vector<string> palette = parser->getSectionData( PALETTE );
  std::vector<std::string> rgb;
  rgb = splitString( palette[0], ',' );
  GAME_BLUE =
    Colour( std::stof( rgb[0] ), std::stof( rgb[1] ), std::stof( rgb[2] ) );

  rgb = splitString( palette[1], ',' );
  YELLOW =
    Colour( std::stof( rgb[0] ), std::stof( rgb[1] ), std::stof( rgb[2] ) );

  selectedColour = YELLOW;

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
  renderer->setQuadPosition( "blank", Vec2( previewX, previewY ) );

  renderer->createQuad(
    "gameBlank", backgroundWidth / gridWidth, backgroundHeight / gridHeight );

  // Selected Colour
  renderer->createQuad(
    "selectedColour", selectedColourWidth, selectedColourHeight );
  renderer->setQuadPosition( "selectedColour",
                             Vec2( selectedColourX, selectedColourY ) );
  controls = new Button( renderer, "Controls", 65, 18, Vec2( 500, 4 ), YELLOW );
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
  renderer->setQuadPosition( "background", Vec2( 0, 25 ) );

  // Preview
  renderer->createQuad( "preview", previewWidth, previewHeight );
  renderer->setQuadPosition( "preview", Vec2( previewX, previewY ) );
  logger.info( "s", "Loading menu assets: done" );
};

void
Zezlup::removeGrid()
{
  for ( const auto& cell : grid->cells ) {
    if ( cell->id != Grid::Empty )
      renderer->deleteQuad( cell->id );
  }

  delete grid;
};

void
Zezlup::loadFont()
{
  logger.info( "s", "Loading fonts..." );
  std::vector<string> fontPaths = parser->getSectionData( FONTS );

  std::vector<std::string> font;
  for ( std::string& fontPath : fontPaths ) {
    font = splitString( fontPath, ',' );
    renderer->loadFont( font[0], font[1] );
    fontIds.push_back( font[0] );
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
  picker = new ColourPicker( pickerWidth, pickerHeight );

  renderer->createQuad( "colourPicker", pickerWidth, pickerHeight );
  renderer->setQuadPosition( "colourPicker", Vec2( pickerX, pickerY ) );

  renderer->loadTexture(
    "colourPicker", picker->getBuffer(), pickerWidth, pickerHeight, 0 );
};

bool
Zezlup::getOptionSelectedWithMouse()
{
  s8 xPos = -1, yPos = -1;
  u8 index = 0;
  for ( auto it = optionsCoords.cbegin(); index < optionColumns; ++index, ++it )
    if ( mouse->position.x >= it->x && mouse->position.x < it->x + optionWidth )
      xPos = index;

  index = 0;
  for ( auto it = optionsCoords.cbegin(); index < optionRows * optionColumns;
        index += optionColumns, it += optionColumns )
    if ( mouse->position.y >= it->y &&
         mouse->position.y < it->y + optionHeight )
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
    isFadeOut = true;

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
  return mouse->position.x >= selectedColourX &&
         mouse->position.x <= ( selectedColourX + selectedColourWidth ) &&
         mouse->position.y >= selectedColourY &&
         mouse->position.y <= ( selectedColourY + selectedColourHeight );
};

bool
Zezlup::isColourPickerClicked()
{
  return mouse->position.x >= pickerX &&
         mouse->position.x <= ( pickerX + pickerWidth ) &&
         mouse->position.y >= pickerY &&
         mouse->position.y <= ( pickerY + pickerHeight );
};

void
Zezlup::processMenuInput()
{
  // TODO(Josue): Use a map here
  switch ( key ) {
    case Keys::right:
      if ( optionSelected < assets.size() - 1 )
        ++optionSelected;
      break;

    case Keys::left:
      if ( optionSelected > 0 )
        --optionSelected;
      break;

    case Keys::down:
      if ( optionSelected < 3 )
        optionSelected += 3;
      break;

    case Keys::up:
      if ( optionSelected > 2 )
        optionSelected -= 3;
      break;

    case Keys::enter:
      isFadeOut = true;

      break;
  }

  if ( mouse->isCoordsChanged )
    getOptionSelectedWithMouse();

  if ( mouse->isLeftPressed && !leftMouseAlreadyClicked ) {
    leftMouseAlreadyClicked = true;
    if ( !isDisplayingPicker && isSelectedColourClicked() ) {
      isDisplayingPicker = true;
    } else if ( isDisplayingPicker && isColourPickerClicked() ) {
      isDisplayingPicker = false;
      Colour newColour = picker->getColour(
        Vec2( mouse->position.x - pickerX, mouse->position.y - pickerY ) );
      selectedColour = newColour;
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
    if ( mouse->position.x >= ( *it )->x &&
         mouse->position.x < ( *it )->x + cellWidth )
      xPos = index;

  index = 0;
  for ( auto it = cellsCoords.cbegin(); index < gridWidth * gridHeight;
        index += gridWidth, it += gridWidth )
    if ( mouse->position.y >= ( *it )->y &&
         mouse->position.y < ( *it )->y + cellHeight )
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
    case Keys::right:
      if ( selected < gridWidth * gridHeight - 1 )
        ++selected;
      break;

    case Keys::left:
      if ( selected > 0 )
        --selected;
      break;

    case Keys::down:
      if ( selected < gridWidth * ( gridHeight - 1 ) )
        selected += gridWidth;
      break;

    case Keys::up:
      if ( selected > gridWidth - 1 )
        selected -= gridWidth;
      break;

    case Keys::z:
      isDisplayingPreview = !isDisplayingPreview;
      break;

    case Keys::backspace:
      removeGridTextures();
      removeGrid();
      isDisplayingPreview = false;
      menuMode = true;
      break;

    case Keys::x:
      if ( !isDisplayingPreview )
        shiftSelectedCell();
      break;
  }

  if ( mouse->isCoordsChanged )
    getCellSelectedWithMouse();

  if ( mouse->isLeftPressed && !isDisplayingPreview &&
       !leftMouseAlreadyClicked ) {
    leftMouseAlreadyClicked = true;
    selectCellWithMouseClick();
  }
};

bool
Zezlup::isPuzzleCompleted()
{
  for ( u8 i = 1; i < gridWidth * gridHeight; ++i ) {
    std::string s = grid->getId( i );
    if ( s == Grid::Empty || s.substr( 4 ) != std::to_string( i ) )
      return false;
  }

  return true;
}

void
Zezlup::displayPreview()
{
  renderer->draw( "preview", assets[optionSelected], ROUNDED );
  renderer->draw( "blank", "Blank", GRID );
};

void
Zezlup::menu()
{
  renderer->draw( "background", assets[optionSelected], BLUR );
  u8 index = 0;

  for ( auto asset = assets.begin(); asset != assets.end(); ++asset, ++index ) {
    renderer->setQuadPosition(
      "optionQuad", Vec2( optionsCoords[index].x, optionsCoords[index].y ) );

    if ( index == optionSelected )
      renderer->draw( "optionQuad", *asset, ROUNDED, selectedColour );
    else
      renderer->draw( "optionQuad", *asset, ROUNDED );
  }

  if ( isDisplayingPicker )
    displayColourPicker();
};

void
Zezlup::fadeOut()
{
  renderer->draw( "background", assets[optionSelected], BLUR );
  u8 index = 0;

  if ( fadeAlpha > 0.0 )
    fadeAlpha -= 0.018;
  if ( fadeAlpha <= 0.0 ) {
    isFadeOut = false;
    initializeGameplay();
    fadeAlpha = 1.0f;
  }

  for ( auto asset = assets.begin(); asset != assets.end(); ++asset, ++index ) {
    renderer->setQuadPosition(
      "optionQuad", Vec2( optionsCoords[index].x, optionsCoords[index].y ) );

    if ( index == optionSelected )
      renderer->draw(
        "optionQuad", *asset, ROUNDED, selectedColour, fadeAlpha );
    else
      renderer->draw( "optionQuad", *asset, ROUNDED, fadeAlpha );
  }
};

void
Zezlup::renderActiveCell()
{
  std::string selectedShader = isDisplayingPreview ? BLUR : GRID;
  renderer->setQuadPosition(
    "cellQuad", Vec2( grid->cells[selected]->x, grid->cells[selected]->y ) );

  if ( grid->cells[selected]->id != Grid::Empty )
    renderer->draw(
      "cellQuad", grid->cells[selected]->id, selectedShader, selectedColour );
  else
    renderer->draw( "cellQuad", "Blank", GRID, selectedColour );
};

void
Zezlup::renderInactiveCells()
{
  u8 index = 0;
  std::string selectedShader = isDisplayingPreview ? BLUR : GRID;

  for ( auto cell = grid->cells.begin(); cell != grid->cells.end();
        ++cell, ++index ) {
    renderer->setQuadPosition( "cellQuad", Vec2( ( *cell )->x, ( *cell )->y ) );

    if ( index != selected || isDisplayingPreview ) {
      if ( index == empty )
        renderer->draw( "cellQuad", "Blank", GRID );
      else
        renderer->draw( "cellQuad", ( *cell )->id, selectedShader );
    }
  }
};

void
Zezlup::displayColourPicker()
{
  renderer->draw( "colourPicker", "colourPicker", GRID );
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
    renderer->setQuadPosition( "cellQuad",
                               Vec2( startCoordinates.x, startCoordinates.y ) );
  else
    renderer->setQuadPosition( "cellQuad",
                               Vec2( endCoordinates.x, endCoordinates.y ) );

  renderer->draw( "cellQuad", grid->cells[selected]->id, GRID, selectedColour );
};

void
Zezlup::displayFPS( f32& start, f32& end )
{
  renderer->print( std::string( "FPS: " ), Vec2( 650, 18 ), fontIds[0] );
  renderer->print( std::to_string( static_cast<u8>( 1 / ( end - start ) ) ),
                   Vec2( 685, 18 ),
                   fontIds[0] );
};

void
Zezlup::displayStaticInfo()
{
  if ( menuMode )
    renderer->print( std::string( "Zezlup!" ), Vec2( 10, 18 ), fontIds[0] );
  else {
    renderer->print( std::string( "Moves" ), Vec2( 10, 18 ), fontIds[0] );
    renderer->print( std::to_string( moves ), Vec2( 60, 18 ), fontIds[0] );

    u32 time = static_cast<u32>( renderer->getTime() - startTime );
    u8 seconds = time % 60;
    u8 minutes = time / 60;

    renderer->print( std::string( "| Time" ), Vec2( 100, 18 ), fontIds[0] );
    if ( minutes < 10 ) {
      renderer->print( std::string( "0" ), Vec2( 153, 18 ), fontIds[0] );
      renderer->print( std::to_string( minutes ), Vec2( 162, 18 ), fontIds[0] );
    } else {
      renderer->print( std::to_string( minutes ), Vec2( 188, 18 ), fontIds[0] );
    }

    renderer->print( ":", Vec2( 173, 18 ), fontIds[0] );
    if ( seconds < 10 ) {
      renderer->print( std::string( "0" ), Vec2( 180, 18 ), fontIds[0] );
      renderer->print( std::to_string( seconds ), Vec2( 189, 18 ), fontIds[0] );
    } else {
      renderer->print( std::to_string( seconds ), Vec2( 180, 18 ), fontIds[0] );
    }
    renderer->print(
      std::string( "Controls | Back to Menu" ), Vec2( 275, 18 ), fontIds[0] );
  }

  renderer->draw( "selectedColour", selectedColour );
  renderer->print( std::string( "v0.0.1" ), Vec2( 750, 18 ), fontIds[0] );
  controls->show();
};

void
Zezlup::run()
{
  f32 startSeconds = renderer->getTime();
  f32 endSeconds = 0.0f;

  do {
    renderer->clearColor( GAME_BLUE );
    renderer->clear();

    key = renderer->getKey();
    renderer->getMouseState();
    if ( !mouse->isLeftPressed )
      leftMouseAlreadyClicked = false;
    if ( !mouse->isRightPressed )
      rightMouseAlreadyClicked = false;

    displayStaticInfo();

    if ( menuMode ) {
      processMenuInput();
      if ( isFadeOut )
        fadeOut();
      else
        menu();
    } else {
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

    endSeconds = renderer->getTime();

    startSeconds = renderer->getTime();

    renderer->swapBuffers();
    renderer->pollEvents();

  } while ( key != Keys::esc && !renderer->windowShouldClose() );
};

Zezlup::~Zezlup()
{
  std::for_each(
    images.begin(), images.end(), []( Image* image ) { delete image; } );

  delete parser;
  delete preview;
  delete background;
  delete renderer;
  delete picker;
};
