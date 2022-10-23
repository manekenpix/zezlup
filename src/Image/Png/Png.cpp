#include "Png.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

Png::Png( const char* file )
{
  loadFile( file );
  fileIterator = fileBuffer.begin();

  if ( !isPNG( fileIterator ) )
    throw;

  u32 chunkLength = readU32( fileIterator );
  u32 chunkType = readU32( fileIterator );

  while ( chunkType != ChunkType::IEND ) {
    switch ( chunkType ) {
      case ChunkType::IHDR:
        readIHDR( fileIterator );
        break;

      case ChunkType::IDAT:
        readIDAT( fileIterator, chunkLength );
        break;

      case ChunkType::PLTE:
        readPLTE( fileIterator, chunkLength );
        break;

      case ChunkType::pHYs:
        logger.debug(
          "sd", "Pixels per unit, X axis: ", readU32( fileIterator ) );
        logger.debug(
          "sd", "Pixels per unit, Y axis: ", readU32( fileIterator ) );
        logger.debug( "sd", "Unit specifier: ", ( *fileIterator++ ) );

        readU32( fileIterator );
        break;

      default:
        fileIterator += chunkLength;
        readU32( fileIterator );
    }
    chunkLength = readU32( fileIterator );
    chunkType = readU32( fileIterator );
  }
  printImageInfo();

  zerr( inf() );

  if ( palette.size() ) {
    applyColoursFromPalette();
  }
  filter();
};

void
Png::loadFile( const char* file )
{
  std::ifstream image;

  image.open( file, std::ios::binary );

  if ( !image.is_open() ) {
    logger.error( "ss", "Error opening the file: ", file );
    throw;
  }

  logger.debug( "ss", "Image: ", file );

  u8 begin;
  begin = image.tellg();

  image.seekg( 0, std::ios::end );
  fileSize = image.tellg();
  image.seekg( 0, std::ios::beg );

  fileSize = fileSize - begin;
  logger.debug( "sd", "fileSize of file: ", fileSize );

  fileBuffer.resize( fileSize );
  logger.debug( "sd", "Size of vector: ", fileBuffer.size() );

  image.read( (char*)&fileBuffer[0], fileSize );
};

void
Png::readIDAT( u8vIt& it, u32 size )
{
  idatChunks.insert( idatChunks.end(), it, it + size );
  it += size;
  readU32( it );
};

void
Png::readIHDR( u8vIt& it )
{
  width = readU32( it );
  height = readU32( it );

  bitDepth = *it++;
  colourType = *it++;
  compressionMethod = *it++;
  filterMethod = *it++;
  interlaceMethod = *it++;
  readU32( it );

  switch ( colourType ) {
    case ColourType::Greyscale:
      bytesPerPixel = 1;
      break;

    case ColourType::Indexed:
    case ColourType::True:
      bytesPerPixel = 3;
      break;

    case ColourType::GreyscaleAlpha:
    case ColourType::TrueAlpha:
      bytesPerPixel = 4;
      break;
  }
};

void
Png::readPLTE( u8vIt& it, u32 size )
{
  paletteEntries = size / 3;
  logger.debug( "sd", "Number of entries in the palette: ", paletteEntries );

  if ( size % 3 != 0 )
    logger.error( "s", "Invalid palette" );

  u8vIt end = it + size;
  palette.resize( paletteEntries );
  PaletteIterator paletteIt = palette.begin();

  for ( ; it < end; it += bytesPerPixel, ++paletteIt ) {
    *paletteIt = { *it, *( it + 1 ), *( it + 2 ) };
  }

  readU32( it );
}

void
Png::applyColoursFromPalette()
{
  std::vector<u8> indexedImageBuffer( ( width * height * bytesPerPixel ) +
                                      height );

  u8vIt imageBufferIt = imageBuffer.begin(),
        indexedImageBufferIt = indexedImageBuffer.begin();
  u8vIt end = imageBuffer.end(), scanlineEnd;

  for ( ; imageBufferIt < end; ) {
    *indexedImageBufferIt++ = *imageBufferIt++;

    scanlineEnd = imageBufferIt + width;
    for ( ; imageBufferIt < scanlineEnd; ) {

      *indexedImageBufferIt++ = palette[*imageBufferIt].R;
      *indexedImageBufferIt++ = palette[*imageBufferIt].G;
      *indexedImageBufferIt++ = palette[*imageBufferIt].B;
      ++imageBufferIt;
    }
  }

  imageBuffer = indexedImageBuffer;
}

void
Png::filter()
{
  processedImage.resize( width * height * bytesPerPixel );
  u8vIt processedImageIt = processedImage.begin();

  u8vIt scanlineIt = imageBuffer.begin(), byteIt, endScanline, endFirstPixel,
        processedImageRowIt;

  u32 scanline = ( width * bytesPerPixel );
  u32 filterScanline = scanline + 1;

  for ( ; scanlineIt < imageBuffer.end(); scanlineIt += filterScanline ) {
    byteIt = scanlineIt + 1;
    switch ( *scanlineIt ) {
      case FilterType::None:
        std::copy( byteIt, byteIt + scanline, processedImageIt );
        break;

      case FilterType::Sub:
        // Process the first byte
        std::copy( byteIt, byteIt + bytesPerPixel, processedImageIt );

        // Process the rest of the scanline
        std::transform( byteIt + bytesPerPixel,
                        byteIt + scanline,
                        processedImageIt,
                        processedImageIt + bytesPerPixel,
                        std::plus<u8>() );
        break;

      case FilterType::Up:
        std::transform( byteIt,
                        byteIt + scanline,
                        processedImageIt - scanline,
                        processedImageIt,
                        std::plus<u8>() );
        break;

      case FilterType::Average:
        endScanline = byteIt + scanline;
        processedImageRowIt = processedImageIt;

        // Process the first pixel
        endFirstPixel = byteIt + bytesPerPixel;
        for ( ; byteIt < endFirstPixel; ++byteIt, ++processedImageRowIt ) {
          *processedImageRowIt =
            *byteIt + ( ( *( processedImageRowIt - scanline ) ) >> 1 );
        }

        for ( ; byteIt < endScanline; ++byteIt, ++processedImageRowIt ) {
          *processedImageRowIt =
            *byteIt + ( ( ( *( processedImageRowIt - bytesPerPixel ) +
                            *( processedImageRowIt - scanline ) ) ) >>
                        1 );
        }
        break;

      case FilterType::Paeth:
        endScanline = byteIt + scanline;
        processedImageRowIt = processedImageIt;

        // Process the first pixel
        endFirstPixel = byteIt + bytesPerPixel;
        for ( ; byteIt < endFirstPixel; ++byteIt, ++processedImageRowIt ) {
          *processedImageRowIt =
            *byteIt +
            paethPredictor( 0, *( processedImageRowIt - scanline ), 0 );
        }

        // Process the rest of the scanline
        for ( ; byteIt < endScanline; ++byteIt, ++processedImageRowIt ) {
          *processedImageRowIt =
            *byteIt + paethPredictor(
                        *( processedImageRowIt - bytesPerPixel ),
                        *( processedImageRowIt - scanline ),
                        *( processedImageRowIt - scanline - bytesPerPixel ) );
        }
        break;
      default:
        logger.error( "s", "Wrong filter" );
    }
    processedImageIt += scanline;
  }
};

u32
Png::inf()
{
  u32 ret;
  unsigned have;
  z_stream strm;

  /* allocate inflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;
  ret = inflateInit( &strm );
  if ( ret != Z_OK )
    return ret;

  /* decompress until deflate stream ends or end of file */
  std::vector<u8> t( CHUNK );
  strm.next_in = idatChunks.data();
  strm.avail_in = idatChunks.size();
  u8 counter = 0;

  /* run inflate() on input until output buffer not full */
  do {
    strm.avail_out = CHUNK;
    strm.next_out = t.data();

    ret = inflate( &strm, Z_NO_FLUSH );
    switch ( ret ) {
      case Z_NEED_DICT:
        ret = Z_DATA_ERROR;
    }

    have = CHUNK - strm.avail_out;

    imageBuffer.insert( imageBuffer.end(),
                        t.begin(),
                        have == CHUNK ? t.end() : t.begin() + have );
    counter++;
  } while ( strm.avail_out == 0 );
  /* done when inflate() says it's done */
  logger.debug( "sds", "Size of the image buffer: ", imageBuffer.size(), "\n" );

  /* clean up and return */
  (void)inflateEnd( &strm );
  return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/* report a zlib or i/o error */
void
Png::zerr( int ret )
{
  if ( ret != 0 ) {
    fputs( "zpipe: ", stderr );
    switch ( ret ) {
      case Z_ERRNO:
        if ( ferror( stdin ) )
          fputs( "error reading stdin\n", stderr );
        if ( ferror( stdout ) )
          fputs( "error writing stdout\n", stderr );
        break;
      case Z_STREAM_ERROR:
        fputs( "invalid compression level\n", stderr );
        break;
      case Z_DATA_ERROR:
        fputs( "invalid or incomplete deflate data\n", stderr );
        break;
      case Z_MEM_ERROR:
        fputs( "out of memory\n", stderr );
        break;
      case Z_VERSION_ERROR:
        fputs( "zlib version mismatch!\n", stderr );
    }
  }
}

void
Png::printImageInfo()
{
  logger.debug( "sd", "Width: ", width );
  logger.debug( "sd", "Height: ", height );
  logger.debug( "sc", "Bit Depth: ", bitDepth );
  logger.debug( "sc", "Colour Type: ", colourType );
  logger.debug( "sc", "Bytes per Pixel: ", bytesPerPixel );
  logger.debug( "sc", "Compression Method: ", +compressionMethod );
  logger.debug( "sc", "Filter Method: ", +filterMethod );
  logger.debug( "sc", "Interlace Method: ", +interlaceMethod );
};

u32
Png::readU32( u8vIt& it )
{
  u32 value = 0;
  for ( u8 i = 0; i < 3; ++i, ++it ) {
    value = ( value | *it ) << 8;
  }
  value = value | *it++;

  return value;
};

bool
Png::isPNG( u8vIt& it )
{
  for ( u8 n : PNGSignature )
    if ( n != *it++ )
      return false;

  return true;
};

u8*
Png::getImageBuffer()
{
  return processedImage.data();
};

u32
Png::getWidth()
{
  return width;
};

u32
Png::getHeight()
{
  return height;
};

u8
Png::getColourType()
{
  return ( colourType <= 3 ? 0 : 1 );
};

void
Png::printChunkType( u32 chunk )
{
  std::cout << chunk << ' ';
  for ( s8 i = 3; i > -1; --i ) {
    std::cout << u8( chunk >> ( 8 * i ) );
  }
  std::cout << "\n";
}

u8
paethPredictor( u8 a, u8 b, u8 c )
{
  s32 p = a + b - c;
  u32 pa = abs( p - a );
  u32 pb = abs( p - b );
  u32 pc = abs( p - c );

  if ( pa <= pb && pa <= pc )
    return a;
  else if ( pb <= pc )
    return b;
  else
    return c;
};
