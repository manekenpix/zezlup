#include "include/Grid.h"

Grid::Grid( u8 cellsRow,
            u8 cellsColumn,
            f32 windowW,
            f32 windowH,
            u8 empty,
            f32 xOffset,
            f32 yOffset )
  : cellsPerRow{ cellsRow }
  , cellsPerColumn{ cellsColumn }
  , xOffset( xOffset )
  , yOffset( yOffset )
{
  cellWidth = windowW / cellsPerRow;
  cellHeight = windowH / cellsPerColumn;

  for ( u8 rows = 0; rows < cellsPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < cellsPerRow; ++columns ) {

      cells.push_back( new Cell(
        0, 0, "cell" + std::to_string( rows * cellsPerRow + columns ) ) );
      coords.push_back( new Vec2( cellWidth * columns, cellHeight * rows ) );
    }
  }

  Cell* deletedCell = cells[empty];
  delete deletedCell;
  cells[empty] = nullptr;
};

void
Grid::setPositions()
{
  u8 position;

  for ( u8 rows = 0; rows < cellsPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < cellsPerRow; ++columns ) {
      position = ( rows * cellsPerRow ) + columns;
      coords[position]->x += xOffset;
      coords[position]->y += yOffset;
      if ( cells[position] ) {
        cells[position]->x = coords[position]->x;
        cells[position]->y = coords[position]->y;
      }
    }
  }
};

u8
Grid::shuffle( u8 empty, u8 moves )
{
  Shuffle<Cell*> s( cells, cellsPerRow, cellsPerColumn, empty, moves );
  cells = s.run();

  setPositions();
  return s.getEmpty();
};

void
Grid::swapCells( u8 c1, u8 c2 )
{
  cells[c1]->x = coords[c2]->x;
  cells[c1]->y = coords[c2]->y;

  std::swap( cells[c1], cells[c2] );
};

Grid::~Grid()
{
  std::for_each(
    cells.begin(), cells.end(), []( Cell* cell ) { delete cell; } );
};
