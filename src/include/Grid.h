#ifndef GRID_H
#define GRID_H

#include "Png.h"
#include "Quad.h"
#include "Shuffle.h"
#include "types.h"
#include <algorithm>

class Grid
{
  u8 cellsPerRow;
  u8 cellsPerColumn;

  std::vector<Vec2*> coords;

  void setPositions();

public:
  typedef struct Cell
  {
    f32 x;
    f32 y;
    std::string id;

    Cell( f32 _x, f32 _y, std::string _id )
      : x{ _x }
      , y{ _y }
      , id{ _id } {};

  } Cell;

  std::vector<Cell*> cells;
  f32 cellWidth;
  f32 cellHeight;

  Grid( u8 cellsRow, u8 cellsColumn, f32 windowW, f32 windowH, u8 empty );
  ~Grid();

  void swapCells( u8 c1, u8 c2 );
  u8 shuffle( u8 empty, u8 moves );
};

#endif
