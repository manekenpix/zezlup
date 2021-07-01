import {Direction} from '../types/enum';
import {Grid} from './grid';
import {getRandomInt} from './math';

export class Game {
  readonly #grid = new Grid();
  #blankCell!: number;
  #currentCell!: number;

  onStateChange?: Function;

  constructor() {
    this.setupCells();
  }

  private setupCells() {
    this.#blankCell = this.#grid.width - 1; // bottom-right
    this.#currentCell = this.#blankCell;
  }

  get gridSize() {
    return this.#grid.size;
  }

  set gridSize(size: Readonly<Vec2>) {
    if (
      size[0] < Game.MIN_GRID_WIDTH ||
      size[0] > Game.MAX_GRID_WIDTH ||
      size[1] < Game.MIN_GRID_HEIGHT ||
      size[1] > Game.MAX_GRID_HEIGHT
    ) {
      return;
    }

    this.#grid.size = size;
    this.setupCells();
    this.shuffle();
    this.onStateChange?.();
  }

  get gridWidth() {
    return this.#grid.width;
  }

  set gridWidth(value) {
    this.gridSize = [value, this.gridHeight];
  }

  get gridHeight() {
    return this.#grid.height;
  }

  set gridHeight(value) {
    this.gridSize = [this.gridWidth, value];
  }

  get tilesByCell() {
    return this.#grid.tilesByCell;
  }

  get blankCell() {
    return this.#blankCell;
  }

  get currentCell() {
    return this.#currentCell;
  }

  get isCompleted() {
    return this.#grid.isInOrder;
  }

  shuffle(iterations = this.#grid.numTiles * 10) {
    let prevCell = this.#blankCell;

    for (let i = 0; i < iterations; ++i) {
      const neighbors = this.#grid.getNeighborsOfCell(this.#blankCell);
      const prevCellIndex = neighbors.findIndex(cell => cell === prevCell);
      if (prevCellIndex !== -1) neighbors.splice(prevCellIndex, 1);

      prevCell = this.#blankCell;
      this.#blankCell = neighbors[getRandomInt(neighbors.length)];
      this.#grid.swapTilesAtCells(prevCell, this.#blankCell);
    }

    this.#currentCell = this.#blankCell;
    this.onStateChange?.();
  }

  // return true if tile is moved; false otherwise
  moveTileAtCurrentCell() {
    if (!this.#grid.areCellsNeighbors(this.#currentCell, this.#blankCell))
      return false;

    this.#grid.swapTilesAtCells(this.#currentCell, this.#blankCell);
    this.#blankCell = this.#currentCell;
    this.onStateChange?.();
    return true;
  }

  // return true if cell is changed; false otherwise
  changeCurrentCell(direction: Direction) {
    const getNeighborMethod = `get${direction}OfCell` as const;
    const neighborCell = this.#grid[getNeighborMethod](this.#currentCell);
    if (neighborCell === null) return false;

    this.#currentCell = neighborCell;
    this.onStateChange?.();
    return true;
  }

  static MIN_GRID_WIDTH = 2;
  static MIN_GRID_HEIGHT = 2;
  static MAX_GRID_WIDTH = 8;
  static MAX_GRID_HEIGHT = 8;
}
