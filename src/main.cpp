#include "include/Png.h"
#include "include/Window.hpp"
#include <array>

int
main( int argc, char* argv[] )
{
  if ( argc < 2 ) {
    std::cout << "Missing argument" << std::endl;
    return 1;
  }

  GlWindow window;

  // Load Image
  PNG backgroundImage( argv[1] );

  Geometry background( backgroundImage.getImageBuffer(),
                          backgroundImage.getWidth(),
                          backgroundImage.getHeight(),
                          backgroundImage.getColourType(),
                          1280.0,
                          960.0 );

  window.GlRender( background );

  return 0;
};
