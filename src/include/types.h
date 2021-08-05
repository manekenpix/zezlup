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
typedef std::array<f32, 20> Vertices;
typedef std::array<f32, 24> TextVertices;

typedef struct Vec2
{
  u32 x;
  u32 y;

  Vec2( u32 _x, u32 _y )
    : x{ _x }
    , y{ _y } {};

} Vec2;

#endif
