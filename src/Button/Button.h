#ifndef BUTTON_H
#define BUTTON_H

#include "../Renderer/Renderer.h"
#include "../Renderer/types.h"

class Button
{
  std::string name;
  Renderer* renderer = nullptr;
  u32 width, height;
  Vec2 position;
  Colour colour;
  bool isClicked;

  std::vector<u8> buffer;

public:
  Button() = delete;
  Button( Renderer* _renderer,
          std::string name,
          u32 _width,
          u32 _height,
          Vec2 position,
          Colour _colour );
  ~Button(){};

  Vec2 getPosition();
  bool isButtonClicked( Vec2 _position );
  void show();
};

#endif
