#ifndef GRID_H
#define GRID_H

#include "../Image/Image.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/types.h"
#include "../Shuffle.h"
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
  const std::string Empty = "-1";
  const std::string CELL = "cell";

  struct Cell
  {
    f32 x;
    f32 y;
    std::string id;
    Vec2 coords;

    Cell()
      : x{ 0 }
      , y{ 0 }
      , id{ "0" }
      , coords{ Vec2( 0, 0 ) } {};

    Cell( f32 _x, f32 _y, std::string _id, Vec2 _coords )
      : x{ _x }
      , y{ _y }
      , id{ _id }
      , coords{ _coords } {};

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

  Vec2* getCoords( u8 cell );
  std::string getId( u8 position );
  void swapCells( u8 c1, u8 c2 );
  u8 shuffle( u8 empty, u8 moves );
};

#endif
