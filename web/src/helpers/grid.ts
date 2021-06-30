export class Grid {
  private _size!: Vec2;
  private _tiles!: number[];

  constructor(gridSize: Vec2) {
    this.size = gridSize;
  }

  get size(): Readonly<Vec2> {
    return this._size;
  }

  set size([width, height]: Readonly<Vec2>) {
    this._size = [width, height];
    this.resetTiles();
  }

  get tiles(): Readonly<number[]> {
    return this._tiles;
  }

  get isInOrder() {
    return this._tiles.every((tile, index) => tile === index);
  }

  getLeftOfIndex(index: number) {
    return index % this._size[0] ? --index : null;
  }

  getRightOfIndex(index: number) {
    return ++index % this._size[0] ? index : null;
  }

  getTopOfIndex(index: number) {
    return (index += this._size[0]) < this._tiles.length ? index : null;
  }

  getBottomOfIndex(index: number) {
    return (index -= this._size[0]) >= 0 ? index : null;
  }

  getNeighborsOfIndex(index: number) {
    return [
      this.getLeftOfIndex(index),
      this.getRightOfIndex(index),
      this.getTopOfIndex(index),
      this.getBottomOfIndex(index),
    ].filter(index => index !== null) as number[];
  }

  areTilesNeighbors(index1: number, index2: number) {
    return this.getNeighborsOfIndex(index1).includes(index2);
  }

  swapTiles(index1: number, index2: number) {
    const tile1 = this._tiles[index1];
    this._tiles[index1] = this._tiles[index2];
    this._tiles[index2] = tile1;
  }

  resetTiles() {
    this._tiles = [...Array(this._size[0] * this._size[1]).keys()];
  }
}
