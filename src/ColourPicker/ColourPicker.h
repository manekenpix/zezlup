#ifndef COLOUR_PICKER_H
#define COLOUR_PICKER_H

#include "../types.h"
#include <vector>

class ColourPicker
{
  u32 width, height;
  std::vector<u8> buffer;

public:
  ColourPicker() = delete;
  ColourPicker( u32 _width, u32 _height );
  ~ColourPicker(){};

  u8* getBuffer();
  std::array<f32, 3> getColour( u32 x, u32 y );
};

#endif
