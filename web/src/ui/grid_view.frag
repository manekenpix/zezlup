#version 300 es
precision highp float;

uniform sampler2D image;
uniform int blankTileIndex;
uniform int selectedTileIndex;

flat in int tileIndex;
in vec2 imagePosition;
out vec4 outColor;

void main() {
  if(tileIndex == selectedTileIndex) {
    outColor = vec4(0.6, 0.5, 1, 0.8);
    return;
  }

  if(tileIndex == blankTileIndex) {
    outColor = vec4(0, 0, 0, 1);
    return;
  }

  vec2 inverted = vec2(imagePosition[0], 1. - imagePosition[1]);
  outColor = texture(image, inverted);
}
