#include "Png.h"
#include "Quad.h"
#include "Shader.h"
#include "Window.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>

class Game
{
  struct Vec2
  {
    u32 x;
    u32 y;

    Vec2( u32 _x, u32 _y )
    {
      x = _x;
      y = _y;
    };
  };

  std::vector<Vec2*> coords;

  // Window
  const f32 screenWidth = 1000.0;
  const f32 screenHeight = 1000.0;
  GameWindow* window = nullptr;
  f32 framerate;

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
  bool isKeyPressed;
  int key;
  enum BytesPerPixel : u8
  {
    RGB = 3,
    RGBA = 4
  };

  void createPanel();
  void getRefreshRate();
  void processKeyboardInput();

public:
  Game();
  ~Game();
  void run();
};
