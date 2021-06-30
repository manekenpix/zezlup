import {Grid} from './grid';

export class Game {
  private _grid = new Grid([5, 3]);
  private _blankTile!: number;
  private _blankTileIndex!: number;
  private _selectedTileIndex!: number;

  constructor() {
    this.setupTiles();
  }

  private setupTiles() {
    this._blankTile = this.gridSize[0] - 1; // bottom-right
    this._blankTileIndex = this._blankTile;
    this._selectedTileIndex = 0;
  }

  get gridSize() {
    return this._grid.size;
  }

  set gridSize(size: Readonly<Vec2>) {
    this._grid.size = size;
    this.setupTiles();
  }

  // get imageSize(): Readonly<Vec2> {
  //   return this._imageSize;
  // }

  get tiles() {
    return this._grid.tiles;
  }

  get blankTileIndex() {
    return this._blankTileIndex;
  }

  get selectedTileIndex() {
    return this._selectedTileIndex;
  }

  get isCompleted() {
    return this._grid.isInOrder;
  }

  // return true if tile is moved; false otherwise
  moveSelectedTile() {
    const canMoveTile = this._grid.areTilesNeighbors(
      this._selectedTileIndex,
      this._blankTileIndex
    );
    if (!canMoveTile) return false;

    this._grid.swapTiles(this._selectedTileIndex, this._blankTileIndex);
    this._blankTileIndex = this._selectedTileIndex;
    return true;
  }

  // return true if tile is changed; false otherwise
  changeSelectedTile(direction: 'Top' | 'Bottom' | 'Left' | 'Right') {
    const getNeighborMethod = `get${direction}OfIndex` as const;
    const neighbor = this._grid[getNeighborMethod](this._selectedTileIndex);
    if (neighbor === null) return false;

    this._selectedTileIndex = neighbor;
    return true;
  }
}
