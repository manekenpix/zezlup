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

  bool isTopLeft() { return position == 0; };
  bool isTopRight() { return position == width - 1; };
  bool isBottomLeft() { return position == size - width; };
  bool isBottomRight() { return position == size - 1; };

  bool isACorner()
  {
    return isTopLeft() || isTopRight() || isBottomLeft() || isBottomRight();
  };

  bool isInTopSide() { return position < width && position != 0; };
  bool isInBottomSide() { return position > size - width - 1; };
  bool isInLeftSide( u8 _y ) { return ( _y * width ) == position; };
  bool isInRightSide( u8 _y )
  {
    return ( ( ( _y + 1 ) * width ) - 1 ) == position;
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
    u8 selected;

    std::vector<u8> topRow, bottomRow, leftColumn, rightColumn;
    u8 stride;
    for ( u8 y = 0; y < height; ++y ) {
      stride = y * width;
      for ( u8 x = 0; x < width; ++x ) {
        position = stride + x;

        // Corners
        if ( !isACorner() ) {

          // Top Row
          if ( isInTopSide() ) {
            topRow.push_back( position );
          }

          // Bottom Row
          else if ( isInBottomSide() ) {
            bottomRow.push_back( position );
          }

          // Left Column
          else if ( isInLeftSide( y ) ) {
            leftColumn.push_back( position );
          }

          // Right Column
          else if ( isInRightSide( y ) ) {
            rightColumn.push_back( position );
          }
        }
      }
    }

    position = start;
    for ( u8 index = 0; index < moves; ++index ) {
      std::vector<u8> options;

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

      // Top Row
      else if ( std::find( topRow.begin(), topRow.end(), position ) !=
                topRow.end() ) {
        options.push_back( position + 1 );
        options.push_back( position - 1 );
        options.push_back( position + width );
      }

      // Bottom Row
      else if ( std::find( bottomRow.begin(), bottomRow.end(), position ) !=
                bottomRow.end() ) {
        options.push_back( position + 1 );
        options.push_back( position - 1 );
        options.push_back( position - width );
      }

      // Left Column
      else if ( std::find( leftColumn.begin(), leftColumn.end(), position ) !=
                leftColumn.end() ) {
        options.push_back( position - width );
        options.push_back( position + 1 );
        options.push_back( position + width );
      }

      // Right Column
      else if ( std::find( rightColumn.begin(), rightColumn.end(), position ) !=
                rightColumn.end() ) {
        options.push_back( position - width );
        options.push_back( position - 1 );
        options.push_back( position + width );
      }

      else {
        options.push_back( position - width );
        options.push_back( position - 1 );
        options.push_back( position + width );
        options.push_back( position + 1 );
      }

      selected = options[std::rand() % options.size()];
      std::swap( array[position], array[selected] );
      position = selected;
    }

    return array;
  };

  ~Shuffle<T>(){};
};

#endif
