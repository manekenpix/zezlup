#version 300 es
#define MAX_TILES 20 * 20 // max grid size = [20, 20]
#define NUM_VERTICES 6 // 2 triangles
#define CLIP_SPACE_SIZE 2.0 // webgl clip space = [-1, 1]

uniform vec2 canvasSize;
uniform vec2 gridSize;
uniform vec2 imageSize;
uniform int tilesByCell[MAX_TILES];
uniform vec2 vertices[NUM_VERTICES];

flat out int cell;
out vec2 cellPosition;
out vec2 imagePosition;

void main() {
  cell = gl_InstanceID;
  int vertexId = gl_VertexID;

  int cellX = cell % int(gridSize[0]);
  int cellY = cell / int(gridSize[0]);
  int tileX = tilesByCell[cell] % int(gridSize[0]);
  int tileY = tilesByCell[cell] / int(gridSize[0]);

  vec2 position = vertices[vertexId];
  cellPosition = position;
  imagePosition = (position + vec2(tileX, tileY)) / gridSize;

  position += vec2(cellX, cellY); // move to cell position
  position /= gridSize; // normalize
  position -= 0.5; // centralize
  position *= imageSize; // to image size

  float widthRatio = canvasSize[0] / imageSize[0];
  float heightRatio = canvasSize[1] / imageSize[1];
  position *= min(widthRatio, heightRatio); // fit to canvas
  position /= canvasSize; // normalize

  position *= CLIP_SPACE_SIZE; // to clip space size

  gl_Position = vec4(position, 0, 1);
}
