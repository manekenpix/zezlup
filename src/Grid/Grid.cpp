#include "Grid.h"

Grid::Grid( u8 cellsRow,
            u8 cellsColumn,
            f32 windowW,
            f32 windowH,
            u8 empty,
            f32 xOffset,
            f32 yOffset )
  : cellsPerRow{ cellsRow }
  , cellsPerColumn{ cellsColumn }
  , xOffset{ xOffset }
  , yOffset{ yOffset }
{
  cellWidth = windowW / cellsPerRow;
  cellHeight = windowH / cellsPerColumn;

  u32 xPos, yPos;
  u8 position;
  for ( u8 rows = 0; rows < cellsPerColumn; ++rows ) {
    for ( u8 columns = 0; columns < cellsPerRow; ++columns ) {
      xPos = cellWidth * columns;
      yPos = cellHeight * rows;
      position = ( rows * cellsPerRow ) + columns;

      cells.push_back( new Cell( xPos + xOffset,
                                 yPos + yOffset,
                                 CELL + std::to_string( position ),
                                 Vec2( xPos + xOffset, yPos + yOffset ) ) );
      coords.push_back( new Vec2( xPos + xOffset, yPos + yOffset ) );
    }
  }
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

Vec2*
Grid::getCoords( u8 cell )
{
  return &( cells[cell]->coords );
};

std::string
Grid::getId( u8 position )
{
  return cells[position]->id;
};

u8
Grid::shuffle( u8 empty, u8 moves )
{
  std::vector<std::string> ids( cellsPerRow * cellsPerColumn, "0" );
  u8 index = 0;
  for ( auto i = ids.begin(); i != ids.end(); ++i, ++index )
    *i = std::to_string( index );

  Shuffle<std::string> s( ids, cellsPerRow, cellsPerColumn, empty, moves );
  ids = s.run();

  auto cell = cells.begin();
  for ( auto i = ids.begin(); i != ids.end(); ++i, ++cell ) {
    ( *cell )->id = CELL + *i;
  }

  u8 emptyAfterShuffling = s.getEmpty();
  cells[emptyAfterShuffling]->id = Empty;

  return emptyAfterShuffling;
};

void
Grid::swapCells( u8 c1, u8 c2 )
{
  std::swap( cells[c1]->id, cells[c2]->id );
};

Grid::~Grid()
{
  std::for_each(
    cells.begin(), cells.end(), []( Cell* cell ) { delete cell; } );
};
