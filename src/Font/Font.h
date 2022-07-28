#ifndef FONT_H
#define FONT_H

#include "../Logger/Logger.h"
#include "../types.h"
#include <ft2build.h>
#include <string>
#include <vector>

#include FT_FREETYPE_H

class Font
{
  const u8 FIRST_CHAR = '!';
  const u8 LAST_CHAR = '}';
  const u8 CHAR_OFFSET = 33;

  Logger logger;

  FT_Library ft;
  FT_Face face;

  typedef struct CharInfo
  {
    std::string character;
    f32 width;
    f32 height;
    u32 advanceX;
    u32 advanceY;
    u32 bitmapTop;

    CharInfo( u8 _character,
              u32 _width,
              u32 _height,
              u32 _advanceX,
              u32 _advanceY,
              u32 _bitmapTop )
      : character( std::string( 1, _character ) )
      , width( _width )
      , height( _height )
      , advanceX( _advanceX )
      , advanceY( _advanceY )
      , bitmapTop( _bitmapTop ){};
  } CharInfo;

  std::vector<CharInfo> charactersInfo;

  u8 calculateOffset( const u8& c ) const;

public:
  Font() = delete;
  Font( const std::string& fontFile );
  ~Font();

  std::string getStringChar( const u8& c ) const;
  f32 getWidth( const u8& c ) const;
  f32 getHeight( const u8& c ) const;
  u32 getAdvanceX( const u8& c ) const;
  u32 getAdvanceY( const u8& c ) const;
  u32 getBitmapTop( const u8& c ) const;
  u8* getBuffer( const u8& c );
};

#endif
