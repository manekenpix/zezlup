#ifndef COLOUR_PICKER_H
#define COLOUR_PICKER_H

#include "../Renderer/types.h"
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
  Colour getColour( Vec2 position );
};

#endif
