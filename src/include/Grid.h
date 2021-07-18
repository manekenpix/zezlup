#ifndef GRID_H
#define GRID_H

#include "Png.h"
#include "Quad.h"
#include <algorithm>

class Grid
{
  u8 cellsPerRow;
  u8 cellsPerColumn;
  u8 bpp;
  u8 colourType;

  // Coordinates
  std::vector<Vec2*> coords;

  enum BytesPerPixel : u8
  {
    RGB = 3,
    RGBA = 4
  };

public:
  std::vector<Quad*> cells;

  Grid( u8 cellsRow, u8 cellsColumn, f32 windowW, f32 windowH, u8 empty );
  ~Grid();

  void swapCells( u8 c1, u8 c2 );
  void setPositions();
};

#endif
