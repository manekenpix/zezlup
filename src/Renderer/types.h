#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <iostream>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef float f32;
enum class Keys
{
  left,
  right,
  up,
  down,
  enter,
  esc,
  backspace,
  help,
  x,
  z,
  blank
};

typedef struct Vec2
{
  f32 x, y;

  Vec2()
    : x{ 0.0 }
    , y{ 0.0 } {};

  Vec2( f32 _x, f32 _y )
    : x{ _x }
    , y{ _y } {};
} Vec2;

typedef struct Mouse
{
  Vec2 position;
  bool isLeftPressed;
  bool isRightPressed;
  bool isCoordsChanged;
} Mouse;

typedef struct Colour
{
  f32 r, g, b;

  Colour()
  {
    r = 0.0;
    g = 0.0;
    b = 0.0;
  };

  Colour( f32 _r, f32 _g, f32 _b )
    : r{ _r }
    , g{ _g }
    , b{ _b } {};
} Colour;

#endif
