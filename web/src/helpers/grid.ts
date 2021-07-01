/**
 * Example of grid 3x2:
 *    Cells  |  Tiles
 *    3 4 5  |  3 2 4
 *    0 1 2  |  0 1 5
 * - Cells increase from Left to Right/Bottom to Top
 *   depending on grid size, and they are fixed.
 * - Tiles can move around.
 * - In the example above, tile 5 is at cell 2, tile 2
 *   is at cell 4, and tile 4 is at cell 5.
 */
export class Grid {
  #size!: Vec2;
  #tilesByCell!: number[];
  #prevTilesByCell?: number[];

  constructor(width = Grid.DEFAULT_WIDTH, height = Grid.DEFAULT_HEIGHT) {
    this.size = [width, height];
  }

  get size(): Readonly<Vec2> {
    return this.#size;
  }

  set size([width, height]) {
    this.#size = [width, height];
    this.reset();
    this.#prevTilesByCell = undefined;
  }

  get width() {
    return this.size[0];
  }

  get height() {
    return this.size[1];
  }

  get isInOrder() {
    return this.tilesByCell.every((tile, cell) => tile === cell);
  }

  get tilesByCell(): Readonly<number[]> {
    return this.#tilesByCell;
  }

  get numTiles() {
    return this.#tilesByCell.length;
  }

  // return true if previous state is saved; false otherwise
  undoReset() {
    if (!this.#prevTilesByCell) return false;
    this.#tilesByCell = this.#prevTilesByCell;
    return true;
  }

  reset() {
    this.#prevTilesByCell = this.#tilesByCell;
    this.#tilesByCell = [...Array(this.width * this.height).keys()];
  }

  getLeftOfCell(cell: number) {
    return cell % this.width ? --cell : null;
  }

  getRightOfCell(cell: number) {
    return ++cell % this.width ? cell : null;
  }

  getTopOfCell(cell: number) {
    return (cell += this.width) < this.numTiles ? cell : null;
  }

  getBottomOfCell(cell: number) {
    return (cell -= this.width) >= 0 ? cell : null;
  }

  getNeighborsOfCell(cell: number) {
    return [
      this.getLeftOfCell(cell),
      this.getRightOfCell(cell),
      this.getTopOfCell(cell),
      this.getBottomOfCell(cell),
    ].filter(cell => cell !== null) as number[];
  }

  areCellsNeighbors(cell1: number, cell2: number) {
    return this.getNeighborsOfCell(cell1).includes(cell2);
  }

  swapTilesAtCells(cell1: number, cell2: number) {
    const tile1 = this.#tilesByCell[cell1];
    this.#tilesByCell[cell1] = this.#tilesByCell[cell2];
    this.#tilesByCell[cell2] = tile1;
  }

  static DEFAULT_WIDTH = 8;
  static DEFAULT_HEIGHT = 4;
}
