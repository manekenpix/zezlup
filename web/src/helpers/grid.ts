import {getRandomInt} from './math';

// Example:
// original | current (swapped tiles at index 2 & 5)
//   6 7 8     6 7 8
//   3 4 5     3 4 2
//   0 1 2     0 1 5
// - gridSize = [3, 3]
// - blankTile = 2 (bottom right of original)
// - blankTile is currently at index 5
// - tile 5 is currently at index 2
export class Grid {
  #size!: Vec2;
  #tiles!: number[];
  #blankTileIndex!: number;

  constructor(gridSize: Vec2) {
    this.size = gridSize;
  }

  get size(): Readonly<Vec2> {
    return this.#size;
  }

  set size([gridWidth, gridHeight]: Readonly<Vec2>) {
    this.#size = [gridWidth, gridHeight];
    this.#blankTileIndex = this.blankTile;
    this.resetTiles();
  }

  get numTiles() {
    return this.#tiles.length;
  }

  get blankTile() {
    return this.#size[0] - 1; // bottom right
  }

  get blankTileIndex() {
    return this.#blankTileIndex;
  }

  get isInOrder() {
    return this.#tiles.every((tile, tileIndex) => tile === tileIndex);
  }

  #moveNeighborTile(neighborTileIndex: number) {
    const neighborTile = this.#tiles[neighborTileIndex];
    this.#tiles[neighborTileIndex] = this.blankTile;
    this.#tiles[this.#blankTileIndex] = neighborTile;
    this.#blankTileIndex = neighborTileIndex;
  }

  getLeftNeighborTileIndex(tileIndex: number) {
    return tileIndex % this.#size[0] ? --tileIndex : null;
  }

  getRightNeighborTileIndex(tileIndex: number) {
    return ++tileIndex % this.#size[0] ? tileIndex : null;
  }

  getTopNeighborTileIndex(tileIndex: number) {
    return (tileIndex += this.#size[0]) < this.numTiles ? tileIndex : null;
  }

  getBottomNeighborTileIndex(tileIndex: number) {
    return (tileIndex -= this.#size[0]) >= 0 ? tileIndex : null;
  }

  getNeighborTileIndexes(tileIndex: number) {
    return [
      this.getLeftNeighborTileIndex(tileIndex),
      this.getRightNeighborTileIndex(tileIndex),
      this.getTopNeighborTileIndex(tileIndex),
      this.getBottomNeighborTileIndex(tileIndex),
    ].filter(tileIndex => tileIndex !== null) as number[];
  }

  // return true if tile is moved; false otherwise
  moveTile(tileIndex: number) {
    const neighborTileIndexes = this.getNeighborTileIndexes(tileIndex);
    if (!neighborTileIndexes.includes(this.#blankTileIndex)) return false;

    this.#moveNeighborTile(tileIndex);
    return true;
  }

  shuffleTiles() {
    const shuffleIterations = this.numTiles * 4;
    for (let i = 0; i < shuffleIterations; ++i) {
      const tileIndexes = this.getNeighborTileIndexes(this.#blankTileIndex);
      const tileIndex = tileIndexes[getRandomInt(tileIndexes.length - 1)];
      this.#moveNeighborTile(tileIndex);
    }
  }

  resetTiles() {
    this.#tiles = [...Array(this.#size[0] * this.#size[1]).keys()];
  }
}
