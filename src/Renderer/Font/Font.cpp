#include "Font.h"

Font::Font( const std::string& fontFile )
{
  if ( FT_Init_FreeType( &ft ) ) {
    logger.error( "s", "ERROR::FREETYPE: Could not init FreeType Library\n" );
    throw;
  }

  if ( FT_New_Face( ft, fontFile.c_str(), 0, &face ) ) {
    logger.error( "s", "ERROR::FREETYPE: Failed to load font\n" );
    throw;
  }

  logger.info( "ss", "Font: ", fontFile.c_str() );
  FT_Set_Pixel_Sizes( face, 0, 13 );

  for ( u8 c = FIRST_CHAR; c < LAST_CHAR; ++c ) {
    if ( FT_Load_Char( face, c, FT_LOAD_RENDER ) ) {
      logger.error( "s", "ERROR::FREETYTPE: Failed to load Glyph\n" );
      throw;
    }

    charactersInfo.push_back(
      { c,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        static_cast<u32>( ( face->glyph->advance.x ) >> 6 ),
        static_cast<u32>( ( face->glyph->advance.y ) >> 6 ),
        static_cast<u32>( face->glyph->bitmap_top ) } );
  }
};

u8
Font::calculateOffset( const u8& c ) const
{
  return c - CHAR_OFFSET;
};

std::string
Font::getStringChar( const u8& c ) const
{
  return charactersInfo[calculateOffset( c )].character;
};

f32
Font::getWidth( const u8& c ) const
{
  return charactersInfo[calculateOffset( c )].width;
};

f32
Font::getHeight( const u8& c ) const
{
  return charactersInfo[calculateOffset( c )].height;
};

u32
Font::getAdvanceX( const u8& c ) const
{
  return charactersInfo[calculateOffset( c )].advanceX;
};

u32
Font::getAdvanceY( const u8& c ) const
{
  return charactersInfo[calculateOffset( c )].advanceY;
};

u32
Font::getBitmapTop( const u8& c ) const
{
  return charactersInfo[calculateOffset( c )].bitmapTop;
};

u8*
Font::getBuffer( const u8& c )
{
  if ( FT_Load_Char( face, c, FT_LOAD_RENDER ) ) {
    logger.error( "s", "ERROR::FREETYTPE: Failed to load Glyph\n" );
    throw;
  }

  return face->glyph->bitmap.buffer;
};

Font::~Font()
{
  FT_Done_Face( face );
  FT_Done_FreeType( ft );
};
