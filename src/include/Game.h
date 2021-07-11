#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "Png.h"
#include "Quad.h"
#include "Renderer.h"
#include "Shader.h"
#include "Shuffle.h"
#include "Texture.h"
#include "Window.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>
#include <string>

class Game
{
  // Renderer
  Renderer* renderer;

  Grid* grid;

  // Window
  GameWindow* window = nullptr;
  const f32 screenWidth = 1000.0f;
  const f32 screenHeight = 1000.0f;
  f32 framerate;

  // Preview
  Quad* preview;
  const f32 previewWidth = 800.0f;
  const f32 previewHeight = 800.0f;

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
