#include "Image.h"
#include "Png/Png.h"

Image*
loadImage( const char* file )
{
  return new Png( file );
};
