#version 300 es
#define MAX_TILES 20 * 20 // numColumns * numRows
#define NUM_VERTICES 6 // 2 triangles

uniform vec2 gridSize; // [numColumns, numRows]
uniform vec2 tileSize; // [1 / numColumns, 1 / numRows]
uniform int tiles[MAX_TILES];
uniform vec2 vertices[NUM_VERTICES];

flat out int tileId;

void main() {
  tileId = gl_InstanceID;

  int tile = tiles[tileId];
  int tileX = tile % int(gridSize[0]);
  int tileY = tile / int(gridSize[0]);

  vec2 translate = vec2(tileX, tileY) - gridSize / 2.0;
  vec2 position = vertices[gl_VertexID];

  gl_Position = vec4((position + translate) * tileSize, 0, 1);
}
