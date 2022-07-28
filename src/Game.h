#ifndef GAME_H
#define GAME_H

#include "Font/Font.h"
#include "Grid/Grid.h"
#include "Logger/Logger.h"
#include "Parser/Parser.h"
#include "PngParser/PngParser.h"
#include "Renderer/Quad/Quad.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture/Texture.h"
#include "Shuffle.h"
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
  Parser* parser;
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
  const string FONTS = "Fonts";
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
  const string COORDS = "Coords";
  const f32 optionWidth = 200.0f;
  const f32 optionHeight = 200.0f;
  const u8 optionRows = 2;
  const u8 optionColumns = 3;
  std::vector<Vec2> optionsCoords;

  // Images
  const string ASSETS = "Assets";
  const string SHADERS = "Shaders";
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

  friend inline std::vector<std::string> splitString( const std::string& str,
                                                      s8 token );
};

inline std::vector<std::string>
splitString( const std::string& str, s8 token )
{
  std::vector<std::string> result;

  std::size_t start = 0;
  std::size_t end = str.find( token );

  while ( end != std::string::npos ) {
    result.push_back( str.substr( start, end - start ) );
    start = end + 1;
    end = str.find( token, start );
  }
  result.push_back( str.substr( start ) );

  return result;
};

#endif
