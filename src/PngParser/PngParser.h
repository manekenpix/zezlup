#ifndef PNG_H
#define PNG_H

#include "../Logger/Logger.h"
#include "../types.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <zlib.h>

struct PaletteColour
{
  u8 R;
  u8 G;
  u8 B;
};
typedef std::vector<PaletteColour>::iterator PaletteIterator;
typedef std::vector<uint8_t>::iterator u8vIt;

class Png
{
  Logger logger;
  const u32 CHUNK = 16384;
  const u8 PNGSignature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

  enum ColourType : u8
  {
    Greyscale = 0,      // Greyscale: grey.
    True = 2,           // Truecolour: red, green, blue.
    Indexed = 3,        // Indexed-colour: palette index.
    GreyscaleAlpha = 4, // Greyscale with alpha: grey, alpha.
    TrueAlpha = 6       // Truecolour with alpha: red, green, blue, alpha.
  };

  enum FilterType : u8
  {
    None = 0,
    Sub = 1,
    Up = 2,
    Average = 3,
    Paeth = 4
  };

  enum ChunkType : u32
  {
    IHDR = 1229472850,
    IDAT = 1229209940,
    IEND = 1229278788,
    PLTE = 1347179589,
    pHYs = 1883789683
  };

  // File
  std::vector<u8> fileBuffer;
  u8vIt fileIterator;
  u64 fileSize;

  // IDAT Chunks
  std::vector<u8> idatChunks;

  // IHDR
  u32 width, height;
  u8 bitDepth, colourType, compressionMethod, filterMethod, interlaceMethod,
    bytesPerPixel;

  // PLTE
  std::vector<PaletteColour> palette;
  u32 paletteEntries = 0;

  // Image buffer
  std::vector<u8> imageBuffer;
  std::vector<u8> processedImage;

  bool isPNG( u8vIt& );
  void loadFile( const char* );
  void readIHDR( u8vIt& );
  void readPLTE( u8vIt&, u32 );
  void readIDAT( u8vIt&, u32 );
  void applyColoursFromPalette();

  u32 inf();
  void zerr( int );

  void filter();

  u32 readU32( u8vIt& );
  void printChunkType( u32 );
  void printImageInfo();

public:
  Png( const char* );
  u32 getWidth();
  u32 getHeight();
  u8* getImageBuffer();
  u8 getColourType();
};

u8 paethPredictor( u8, u8, u8 );

#endif
