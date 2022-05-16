#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "Logger.h"
#include "Png.h"
#include "Quad.h"
#include "Renderer.h"
#include "Shader.h"
#include "Shuffle.h"
#include "Texture.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>
#include <charconv>
#include <ft2build.h>
#include <string>
#include FT_FREETYPE_H

class Game
{
  enum class Directions
  {
    left,
    right,
    up,
    down
  };

  Logger logger;
  Renderer* renderer;

  Grid* grid;
  u8 gridWidth;
  u8 gridHeight;

  // Window
  const f32 windowWidth = 800.0f;
  const f32 windowHeight = 825.0f;
  f32 framerate;

  // Preview
  Quad* preview;
  bool isDisplayingPreview;
  const f32 previewWidth = 600.0f;
  const f32 previewHeight = 600.0f;
  const f32 previewX = 100.0f;
  const f32 previewY = 125.0f;
  const u8 initialEmpty = 0;

  // Fonts
  FT_Library ft;
  FT_Face face;
  typedef struct Font
  {
    u32 advanceX;
    u32 advanceY;
    u32 bitmapTop;
    std::string character;

    Font( u8 _character, u32 _advanceX, u32 _advanceY, u32 _bitmapTop )
      : advanceX( _advanceX )
      , advanceY( _advanceY )
      , character( std::string( 1, _character ) )
      , bitmapTop( _bitmapTop ){};
  } Font;

  std::vector<Font> font;

  // Menu
  Quad* background;
  const f32 backgroundWidth = 800.0f;
  const f32 backgroundHeight = 800.0f;

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
  u32 moves = 0;
  f32 startTime = 0;
  u8 empty;
  u8 optionSelected;
  bool menuMode;
  bool isKeyPressed;
  Renderer::Keys key;
  Vec2* emptyCell = nullptr;

  bool isDisplayingHelp;

  // Animation
  u8 shiftOffset = 60;
  Directions direction;
  Grid::Cell startCoordinates;
  Grid::Cell endCoordinates;
  bool inProgress = false;
  void processMenuInput();
  void processGameInput();
  void loadTextures();
  void loadShaders();
  void loadAssets();
  void loadCoordinates();
  void loadFont();
  void loadGridTextures();
  void removeGridTextures();
  void shuffle();
  void createGrid();
  void removeGrid();

  void shiftCell();
  void print( std::string s, u32 x, u32 y );

  void menu();
  void play();

  void displayHelp();
  void displayPreview();

public:
  Game();
  ~Game();
  void run();
};

#endif
