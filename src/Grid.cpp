#include "include/Grid.h"

Grid::Grid( Png* src,
            u8 cellsRow,
            u8 cellsColumn,
            f32 windowW,
            f32 windowH,
            u8 empty )
  : cellsPerRow{ cellsRow }
  , cellsPerColumn{ cellsColumn }
{

  u8* imageBuffer = src->getImageBuffer();
  u32 imageWidth = src->getWidth();
  u32 imageHeight = src->getHeight();
  colourType = src->getColourType();

  cellWidth = imageWidth / cellsPerRow;
  cellHeight = imageHeight / cellsPerColumn;
  bpp = colourType ? BytesPerPixel::RGBA : BytesPerPixel::RGB;

  u32 cellSize = cellWidth * cellHeight * bpp;
  f32 cellPositionX, cellPositionY;

  for ( u8 rows = 0; rows < cellsPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < cellsPerRow; ++columns ) {

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
      cells.push_back( new Quad(
        windowW / cellsPerRow, windowH / cellsPerColumn, windowW, windowH ) );
      cells.back()->tex =
        new Texture( cellBuffer, cellWidth, cellHeight, colourType );

      // Store cell positions
      cellPositionX = ( windowW / cellsPerRow ) * columns;
      cellPositionY = ( windowH / cellsPerColumn ) * rows;
      coords.push_back( new Vec2( cellPositionX, cellPositionY ) );
    }
  }

  Quad* last = cells[empty];
  delete last;
  cells[empty] = nullptr;

  setPositions();
};

void
Grid::swapCells( u8 c1, u8 c2 )
{
  std::swap( cells[c1], cells[c2] );

  u8 selected = cells[c1] ? c1 : c2;

  cells[selected]->setPosition( coords[selected]->x, coords[selected]->y );
};

void
Grid::setPositions()
{
  u8 position;

  for ( u8 rows = 0; rows < cellsPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < cellsPerRow; ++columns ) {
      position = ( rows * cellsPerRow ) + columns;
      if ( cells[position] )
        cells[position]->setPosition( coords[position]->x,
                                      coords[position]->y );
    }
  }
};

Grid::~Grid()
{
  std::for_each(
    cells.begin(), cells.end(), []( Quad* cell ) { delete cell; } );
  std::for_each(
    coords.begin(), coords.end(), []( Vec2* coord ) { delete coord; } );
};
