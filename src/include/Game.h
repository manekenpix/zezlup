#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "Png.h"
#include "Quad.h"
#include "Renderer.h"
#include "Shader.h"
#include "Shuffle.h"
#include "Window.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>

class Game
{
  // Renderer
  Renderer* renderer;

  Grid* grid;

  // Window
  const f32 screenWidth = 1000.0f;
  const f32 screenHeight = 1000.0f;
  GameWindow* window = nullptr;
  f32 framerate;

  // Images
  std::vector<Png*> images;

  // Gameplay
  u8 gridWidth;
  u8 gridHeight;
  u8 selected;
  u8 empty;
  bool isKeyPressed;
  int key;

  void getRefreshRate();
  void processKeyboardInput();
  void shuffle();

public:
  Game();
  ~Game();
  void run();
};

#endif
