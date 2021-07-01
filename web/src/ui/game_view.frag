#version 300 es
#define BORDER_SIZE vec2(0.05, 0.05)
precision highp float;

uniform sampler2D image;
uniform vec2 imageSize;
uniform vec2 gridSize;
uniform int blankCell;
uniform int currentCell;

flat in int cell;
in vec2 cellCoord;
in vec2 imageCoord;
out vec4 outColor;

void main() {
  if(cell == currentCell) {
    // equalize vertical and horizontal border size
    vec2 ratio = gridSize / imageSize;
    vec2 borderSize = BORDER_SIZE * ratio / max(ratio[0], ratio[1]);

    if(cellCoord[0] < borderSize[0] ||
      cellCoord[0] > 1. - borderSize[0] ||
      cellCoord[1] < borderSize[1] ||
      cellCoord[1] > 1. - borderSize[1]) {
      if(cell == blankCell) {
        outColor = vec4(.64, .64, .64, 1);
      } else {
        outColor = vec4(0, 0, 0, 1);
      }
      return;
    }
  }

  if(cell == blankCell) {
    outColor = vec4(0, 0, 0, 1);
    return;
  }

  vec2 inverted = imageCoord * vec2(1, -1);
  outColor = texture(image, inverted);
}
