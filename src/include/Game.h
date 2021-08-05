#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "Png.h"
#include "Quad.h"
#include "Renderer.h"
#include "Shader.h"
#include "Shuffle.h"
#include "Text.h"
#include "Texture.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

class Game
{
  // Renderer
  Renderer* renderer;

  // Grid
  Grid* grid;
  u8 gridWidth;
  u8 gridHeight;

  // Window
  const f32 windowWidth = 1024.0f;
  const f32 windowHeight = 1024.0f;
  f32 framerate;

  // Preview
  Quad* preview;
  bool displayPreview;
  const f32 previewWidth = 800.0f;
  const f32 previewHeight = 800.0f;

  // Fonts
  FT_Library ft;
  FT_Face face;
  std::vector<Text*> font;

  // Menu
  Quad* background;
  const f32 backgroundWidth = 1024.0f;
  const f32 backgroundHeight = 1024.0f;

  // Options
  std::vector<Quad*> options;
  const f32 optionWidth = 200.0f;
  const f32 optionHeight = 200.0f;
  std::vector<Vec2> optionsCoords;

  // Images
  std::vector<std::string> assets;
  std::vector<Png*> images;

  // Gameplay
  u8 selected;
  // .u8 selectedImage;
  u8 empty;
  u8 optionSelected;
  bool menuMode;
  bool isKeyPressed;
  std::string key;

  void getRefreshRate();
  void processMenuInput();
  void processGameInput();
  void loadTextures();
  void loadShaders();
  void loadAssets();
  void loadCoordinates();
  void loadFont();
  void loadGridTextures();
  void shuffle();

  void menu();
  void play();

public:
  Game();
  ~Game();
  void run();
};

#endif
