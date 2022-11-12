#include "ColourPicker.h"

ColourPicker::ColourPicker( u32 _width, u32 _height )
  : width{ _width }
  , height{ _height }
{
  const u8 RED = 0;
  const u8 GREEN = 1;
  const u8 BLUE = 2;

  u8 colours[3] = { 255, 0, 0 };
  buffer.reserve( height * width * 3 );

  auto fillColour =
    [&]( u8 index, s8 increment, u32 sectionHeight, u32 sectionWidth ) {
      for ( u8 i = 0; i < sectionHeight; ++i ) {
        for ( u8 j = 0; j < sectionWidth; ++j ) {
          buffer.push_back( colours[RED] );
          buffer.push_back( colours[GREEN] );
          buffer.push_back( colours[BLUE] );
        }
        colours[index] += increment;
      }
    };

  // Red to Yellow
  fillColour( GREEN, 2, height / 5, width );
  // Yellow to Green
  fillColour( RED, -2, height / 5, width );
  // Green to Turquoise
  fillColour( BLUE, 2, height / 5, width );
  //  Turquoise to Blue
  fillColour( GREEN, -2, height / 5, width );
  // Blue to Purple
  fillColour( RED, 2, height / 5, width );
};

u8*
ColourPicker::getBuffer()
{
  return buffer.data();
};

std::array<f32, 3>
ColourPicker::getColour( u32 x, u32 y )
{
  f32 positionInBuffer = ( y * width * 3 ) + ( x * 3 );

  f32 red = buffer[positionInBuffer] / 256.0;
  f32 green = buffer[positionInBuffer + 1] / 256.0;
  f32 blue = buffer[positionInBuffer + 2] / 256.0;

  return std::array<f32, 3>( { red, green, blue } );
};
