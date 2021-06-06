#include "Png.hpp"
#include "Quad.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include <algorithm>

class Game
{
  // Window
  const f32 screenWidth = 1000.0;
  const f32 screenHeight = 1000.0;
  Window* window = nullptr;

  // Images
  std::vector<Png*> images;
  u8* imageBuffer = nullptr;

  // Squares
  std::vector<Quad*> panel;
  std::vector<Quad*> sortedPanel;
  u8 squaresPerRow = 4;
  u8 squaresPerColumn = 4;

  u32 squareWidth = 0;
  u32 squareHeight = 0;

  u8 bytesPerPixel = 0;
  u32 squareResolution = 0;

  // Shaders
  std::vector<Shader*> shaders;

  // Gameplay
  u8 selected = 0;
  u8 empty = 0;

  void createPanel();

public:
  Game();
  ~Game();
  void run();
};
