#ifndef GAME_H
#define GAME_H

#include "Font.h"
#include "Grid.h"
#include "Logger.h"
#include "Png.h"
#include "Quad.h"
#include "Renderer.h"
#include "Shader.h"
#include "Shuffle.h"
#include "Texture.h"
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
  Renderer::Mouse* mouse;

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
  Font* font;

  // Menu
  Quad* background;
  const f32 backgroundWidth = 800.0f;
  const f32 backgroundHeight = 800.0f;

  // Help
  const u32 headerOffsetX = 375;
  const u32 bodyOffsetX = 225;
  const u32 footerOffsetX = 310;

  // Options
  const f32 optionWidth = 200.0f;
  const f32 optionHeight = 200.0f;
  const u8 optionRows = 2;
  const u8 optionColumns = 3;
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
  const u8 initialMoves = 20;

  bool isDisplayingHelp;
  std::array<f32, 3> PURPLE = { 0.55f, 0.0f, 0.55f };
  std::array<f32, 3> WHITE = { 1.0f, 1.0f, 1.0f };
  std::array<f32, 3> BLUE = { 0.07f, 0.12f, 0.23f };

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
  void loadMenuAssets();
  void loadFont();
  void loadGridTextures();
  void removeGridTextures();
  void shuffle();
  void createGrid();
  void removeGrid();

  void shiftCell();
  void print( std::string s, u32 x, u32 y );

  bool getOptionSelectedWithMouse();
  void selectOptionWithMouseClick();
  bool getCellSelectedWithMouse();
  void selectCellWithMouseClick();

  void menu();
  void renderActiveCell();
  void renderInactiveCells();
  bool isPuzzleCompleted();
  void initializeGameplay();
  void shiftSelectedCell();

  void displayHelp();
  void displayPreview();
  void displayFPS( f32& start, f32& end );
  void displayStats();

public:
  Game();
  ~Game();
  void run();
};

#endif
