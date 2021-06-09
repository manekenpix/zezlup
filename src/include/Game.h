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
  u8 squaresPerRow;
  u8 squaresPerColumn;

  u32 squareWidth;
  u32 squareHeight;

  u8 bytesPerPixel;
  u32 squareResolution;

  // Shaders
  std::vector<Shader*> shaders;

  // Gameplay
  u8 selected;
  u8 empty;

  void createPanel();

public:
  Game();
  ~Game();
  void run();
};
