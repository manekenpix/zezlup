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
  f32 xOffset;
  f32 yOffset;

  std::vector<Vec2*> coords;

  void setPositions();

public:
  struct Cell
  {
    f32 x;
    f32 y;
    std::string id;

    Cell()
      : x{ 0 }
      , y{ 0 }
      , id{ "0" } {};

    Cell( f32 _x, f32 _y, std::string _id )
      : x{ _x }
      , y{ _y }
      , id{ _id } {};

    bool operator==( Cell rhs ) { return x == rhs.x && y == rhs.y; };
  };

  std::vector<Cell*> cells;
  f32 cellWidth;
  f32 cellHeight;

  Grid( u8 cellsRow,
        u8 cellsColumn,
        f32 windowW,
        f32 windowH,
        u8 empty,
        f32 xOffset,
        f32 yOffset );
  ~Grid();

  void swapCells( u8 c1, u8 c2 );
  u8 shuffle( u8 empty, u8 moves );
};

#endif
