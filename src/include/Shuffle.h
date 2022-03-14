#ifndef SHUFFLE_H
#define SHUFFLE_H

#include "types.h"
#include <algorithm>
#include <ctime>
#include <vector>

template<typename T>
class Shuffle
{
  std::vector<T> array;
  u8 size;
  u8 position;
  u8 start;
  u8 width;
  u8 height;
  u8 moves;

  // Corners
  bool isTopLeft() { return position == 0; };
  bool isTopRight() { return position == width - 1; };
  bool isBottomLeft() { return position == size - width; };
  bool isBottomRight() { return position == size - 1; };

  // Sides
  bool isInTopSide() { return position < width; };
  bool isInBottomSide() { return position > size - width - 1; };
  bool isInLeftSide()
  {
    s8 n = position;
    if ( n < width )
      return false;
    while ( n >= width ) {
      if ( n % width != 0 )
        return false;
      n -= width;
    }
    return true;
  };

  bool isInRightSide()
  {
    s8 n = position;
    if ( n < width - 1 )
      return false;
    while ( ( n + 1 ) >= width ) {
      if ( ( n + 1 ) % width != 0 )
        return false;
      n -= width;
    }
    return true;
  };

public:
  Shuffle( std::vector<T> v, u8 _width, u8 _height, u8 _start, u8 _moves )
    : array{ v }
    , width{ _width }
    , height{ _height }
    , start{ _start }
    , moves{ _moves }
  {
    std::srand( std::time( nullptr ) );
    size = v.size();
  };

  u8 getEmpty() { return position; };

  std::vector<T> run()
  {
    u8 selected, previous;
    position = previous = start;
    std::vector<u8> options;
    bool isEqual;

    for ( u8 index = 0; index < moves; ++index ) {

      // Top Left
      if ( isTopLeft() ) {
        options.push_back( 1 );
        options.push_back( width );
      }

      // Top Right
      else if ( isTopRight() ) {
        options.push_back( width - 2 );
        options.push_back( ( 2 * width ) - 1 );
      }

      // Botton Left
      else if ( isBottomLeft() ) {
        options.push_back( size - width + 1 );
        options.push_back( size - ( 2 * width ) );
      }

      // Bottom Right
      else if ( isBottomRight() ) {
        options.push_back( size - 2 );
        options.push_back( size - width - 1 );
      }

      // Top side of the grid
      else if ( isInTopSide() ) {
        options.push_back( position + 1 );
        options.push_back( position - 1 );
        options.push_back( position + width );
      }

      // Bottom side of the grid
      else if ( isInBottomSide() ) {
        options.push_back( position + 1 );
        options.push_back( position - 1 );
        options.push_back( position - width );
      }

      // Left side of the grid
      else if ( isInLeftSide() ) {
        options.push_back( position - width );
        options.push_back( position + 1 );
        options.push_back( position + width );
      }

      // Right side of the grid
      else if ( isInRightSide() ) {
        options.push_back( position - width );
        options.push_back( position - 1 );
        options.push_back( position + width );
      }

      // All the cells that are not in any of the sides
      else {
        options.push_back( position - width );
        options.push_back( position - 1 );
        options.push_back( position + width );
        options.push_back( position + 1 );
      }

      do {
        selected = options[std::rand() % options.size()];
        isEqual = previous == selected;
      } while ( isEqual );

      std::swap( array[position], array[selected] );
      previous = position;
      position = selected;

      options.clear();
    }

    return array;
  };

  ~Shuffle<T>(){};
};

#endif
