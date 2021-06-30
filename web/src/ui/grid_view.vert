#version 300 es
#define MAX_TILES 20 * 20 // numColumns * numRows
#define NUM_VERTICES 6 // 2 triangles
#define CLIP_SPACE_SIZE 2.0 // webgl clip space [-1, 1]

uniform vec2 canvasSize;
uniform vec2 gridSize;
uniform vec2 imageSize;
uniform int tiles[MAX_TILES];
uniform vec2 vertices[NUM_VERTICES];

flat out int tileIndex;
out vec2 imagePosition;

void main() {
  tileIndex = gl_InstanceID;
  int vertexIndex = gl_VertexID;

  int tileIndexX = tileIndex % int(gridSize[0]);
  int tileIndexY = tileIndex / int(gridSize[0]);
  int tileX = tiles[tileIndex] % int(gridSize[0]);
  int tileY = tiles[tileIndex] / int(gridSize[0]);

  vec2 vertexPosition = vertices[vertexIndex];

  imagePosition = (vertexPosition + vec2(tileX, tileY)) / gridSize;

  vertexPosition += vec2(tileIndexX, tileIndexY); // move to tile position
  vertexPosition /= gridSize; // normalize

  vertexPosition -= 0.5; // centralize
  vertexPosition *= imageSize; // to image size

  float widthRatio = canvasSize[0] / imageSize[0];
  float heightRatio = canvasSize[1] / imageSize[1];
  vertexPosition *= min(widthRatio, heightRatio); // fit to canvas
  vertexPosition /= canvasSize; // normalize

  vertexPosition *= CLIP_SPACE_SIZE; // to clip space size

  gl_Position = vec4(vertexPosition, 0, 1);
}
