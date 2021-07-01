import {Grid} from './grid';

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
    this.#grid.size = size;
    this.setupCells();
    this.onStateChange?.();
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
  changeCurrentCell(direction: 'Top' | 'Bottom' | 'Left' | 'Right') {
    const getNeighborMethod = `get${direction}OfCell` as const;
    const neighborCell = this.#grid[getNeighborMethod](this.#currentCell);
    if (neighborCell === null) return false;

    this.#currentCell = neighborCell;
    this.onStateChange?.();
    return true;
  }
}
