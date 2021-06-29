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
  private _size!: Vec2;
  private _tiles!: number[];
  private _blankTileIndex!: number;

  constructor(gridSize: Vec2 = [2, 2]) {
    this.size = gridSize;
  }

  get size(): Readonly<Vec2> {
    return this._size;
  }

  set size([gridWidth, gridHeight]: Readonly<Vec2>) {
    this._size = [gridWidth, gridHeight];
    this._blankTileIndex = this.blankTile;
    this.resetTiles();
  }

  get tiles(): Readonly<number[]> {
    return this._tiles;
  }

  get blankTile() {
    return this._size[0] - 1; // bottom right
  }

  get blankTileIndex() {
    return this._blankTileIndex;
  }

  get isInOrder() {
    return this._tiles.every((tile, tileIndex) => tile === tileIndex);
  }

  #moveNeighborTile(neighborTileIndex: number) {
    const neighborTile = this._tiles[neighborTileIndex];
    this._tiles[neighborTileIndex] = this.blankTile;
    this._tiles[this._blankTileIndex] = neighborTile;
    this._blankTileIndex = neighborTileIndex;
  }

  getLeftNeighborTileIndex(tileIndex: number) {
    return tileIndex % this._size[0] ? --tileIndex : null;
  }

  getRightNeighborTileIndex(tileIndex: number) {
    return ++tileIndex % this._size[0] ? tileIndex : null;
  }

  getTopNeighborTileIndex(tileIndex: number) {
    return (tileIndex += this._size[0]) < this._tiles.length ? tileIndex : null;
  }

  getBottomNeighborTileIndex(tileIndex: number) {
    return (tileIndex -= this._size[0]) >= 0 ? tileIndex : null;
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
    if (!neighborTileIndexes.includes(this._blankTileIndex)) return false;

    this.#moveNeighborTile(tileIndex);
    return true;
  }

  shuffleTiles() {
    const shuffleIterations = this._tiles.length * 10;
    for (let i = 0; i < shuffleIterations; ++i) {
      const tileIndexes = this.getNeighborTileIndexes(this._blankTileIndex);
      const tileIndex = tileIndexes[getRandomInt(tileIndexes.length - 1)];
      this.#moveNeighborTile(tileIndex);
    }
  }

  resetTiles() {
    this._tiles = [...Array(this._size[0] * this._size[1]).keys()];
  }
}
