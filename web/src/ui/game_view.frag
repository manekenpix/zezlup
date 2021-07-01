#version 300 es
precision highp float;

uniform sampler2D image;
uniform int blankCell;
uniform int currentCell;

flat in int cell;
in vec2 cellPosition;
in vec2 imagePosition;
out vec4 outColor;

void main() {
  if(cell == currentCell) {
    if(cellPosition[0] < .02 ||
      cellPosition[0] > .98 ||
      cellPosition[1] < .02 ||
      cellPosition[1] > .98) {
      outColor = vec4(0.63, 0.17, 0.63, 1);
      return;
    }
  }

  if(cell == blankCell) {
    outColor = vec4(0, 0, 0, 1);
    return;
  }

  vec2 inverted = vec2(imagePosition[0], 1. - imagePosition[1]);
  outColor = texture(image, inverted);
}
