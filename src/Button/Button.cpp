#include "Button.h"

Button::Button( Renderer* _renderer,
                std::string _name,
                u32 _width,
                u32 _height,
                Vec2 _position,
                Colour _colour )
  : width{ _width }
  , height{ _height }
  , renderer{ _renderer }
  , position{ _position }
  , colour{ _colour }
  , name{ _name }
{
  std::cout << "this is the constructor" << std::endl;

  renderer->createQuad( name, width, height );
  renderer->setQuadPosition( name, position );
};

Vec2
Button::getPosition()
{
  return position;
};

bool
Button::isButtonClicked( Vec2 _position )
{
  return _position.x >= position.x && _position.y >= position.y &&
         _position.x <= position.x + width &&
         _position.y <= position.y + height;
};

void
Button::show()
{
  renderer->draw( name, colour );
  renderer->print(
    name, Vec2( position.x + 3, position.y + height - 3 ), "vg5000" );
};
