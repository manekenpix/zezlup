#version 300 es
#define MAX_TILES 20 * 20 // numColumns * numRows

precision highp float;

uniform vec4 tileColors[MAX_TILES];

flat in int tileId;
out vec4 outColor;

void main() {
  outColor = tileColors[tileId];
}
