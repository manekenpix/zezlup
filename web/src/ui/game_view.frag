#version 300 es
precision highp float;

uniform sampler2D image;
uniform int blankCell;
uniform int currentCell;

flat in int cell;
in vec2 cellCoord;
in vec2 imageCoord;
out vec4 outColor;

void main() {
  if(cell == blankCell) {
    outColor = vec4(0, 0, 0, 1);
  } else {
    vec2 inverted = imageCoord * vec2(1, -1);
    outColor = texture(image, inverted);
  }

  if(cell == currentCell) {
    if(cellCoord[0] < .02 ||
      cellCoord[0] > .98 ||
      cellCoord[1] < .02 ||
      cellCoord[1] > .98) {

      if(cell == blankCell) {
        outColor = vec4(.25, .25, .25, 1);
      } else {
        outColor = vec4(0, 0, 0, 1);
      }
    }
  }
}
