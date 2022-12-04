#ifndef ZEZLUP_H
#define ZEZLUP_H

#include "../Button/Button.h"
#include "../ColourPicker/ColourPicker.h"
#include "../Grid/Grid.h"
#include "../Image/Image.h"
#include "../Logger/Logger.h"
#include "../Parser/Parser.h"
#include "../Renderer/Renderer.h"
#include "../Shuffle.h"
#include <ft2build.h>
#include <string>
#include FT_FREETYPE_H

class Zezlup
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
  Mouse* mouse;
  bool leftMouseAlreadyClicked;
  bool rightMouseAlreadyClicked;

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
  std::vector<std::string> fontIds;

  // Menu
  Quad* background;
  const f32 backgroundWidth = 800.0f;
  const f32 backgroundHeight = 800.0f;

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
  std::vector<Image*> images;

  // Shaders
  const std::string GRID = "grid";
  const std::string ROUNDED = "rounded";
  const std::string SOLID = "solid";
  const std::string TEXT = "text";
  const std::string BLUR = "blur";

  // Top bar
  Button* backToMenu;
  Button* controls;

  // Colour picker
  const std::string PALETTE = "Palette";
  const u32 selectedColourHeight = 15;
  const u32 selectedColourWidth = 25;
  const u32 selectedColourX = 710;
  const u32 selectedColourY = 6;
  const u32 pickerHeight = 127 * 5;
  const u32 pickerWidth = 25;
  const u32 pickerX = 710;
  const u32 pickerY = 21;
  // potential palette
  // https://coolors.co/palette/f72585-7209b7-3a0ca3-4361ee-4cc9f0
  // const Colour GAME_BLUE = Colour( 0.26f, 0.38f, 0.92f );
  Colour GAME_BLUE;
  // const Colour YELLOW = Colour( 0.22f, 0.04f, 0.63f );
  Colour YELLOW;

  bool isDisplayingPicker;
  ColourPicker* picker;
  Colour selectedColour;

  // Gameplay
  u8 selected;
  u32 moves = 0;
  f32 startTime = 0;
  u8 empty;
  u8 optionSelected;
  bool menuMode;
  bool isKeyPressed;
  Keys key;
  Vec2* emptyCell = nullptr;
  const u8 initialMoves = 100;

  // Animation
  u8 shiftOffset = 60;
  Directions direction;
  Grid::Cell startCoordinates;
  Grid::Cell endCoordinates;
  bool inProgress = false;
  f32 fadeAlpha = 1.0f;
  bool isFadeOut = false;

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
  void fadeOut();
  bool getOptionSelectedWithMouse();
  void selectOptionWithMouseClick();
  bool getCellSelectedWithMouse();
  void selectCellWithMouseClick();
  bool isSelectedColourClicked();
  bool isColourPickerClicked();

  void menu();
  void renderActiveCell();
  void renderInactiveCells();
  bool isPuzzleCompleted();
  void initializeGameplay();
  void shiftSelectedCell();

  void displayPreview();
  void displayFPS( f32& start, f32& end );
  void displayStaticInfo();
  void createColourPicker();
  void displayColourPicker();

public:
  Zezlup();
  ~Zezlup();
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
