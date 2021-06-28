import {Grid} from './grid';

test('set grid size', () => {
  const grid = new Grid([3, 4]);
  expect(grid.size).toEqual([3, 4]);
  expect(grid.numTiles).toBe(3 * 4);
  expect(grid.blankTile).toBe(3 - 1);

  grid.size = [5, 6];
  expect(grid.size).toEqual([5, 6]);
  expect(grid.numTiles).toBe(5 * 6);
  expect(grid.blankTile).toBe(5 - 1);
});

test('neighbor indexes', () => {
  const grid = new Grid([3, 4]);

  expect(grid.getLeftNeighborTileIndex(0)).toBe(null);
  expect(grid.getLeftNeighborTileIndex(1)).toBe(0);

  expect(grid.getRightNeighborTileIndex(2)).toBe(null);
  expect(grid.getRightNeighborTileIndex(1)).toBe(2);

  expect(grid.getTopNeighborTileIndex(9)).toBe(null);
  expect(grid.getTopNeighborTileIndex(8)).toBe(11);

  expect(grid.getBottomNeighborTileIndex(2)).toBe(null);
  expect(grid.getBottomNeighborTileIndex(3)).toBe(0);

  const neighborsOfTileIndex0 = grid.getNeighborTileIndexes(0);
  expect(neighborsOfTileIndex0.length).toBe(2);
  expect(neighborsOfTileIndex0).toEqual(expect.arrayContaining([1, 3]));

  const neighborsOfTileIndex11 = grid.getNeighborTileIndexes(11);
  expect(neighborsOfTileIndex11.length).toBe(2);
  expect(neighborsOfTileIndex11).toEqual(expect.arrayContaining([10, 8]));

  const neighborsOfTileIndex7 = grid.getNeighborTileIndexes(7);
  expect(neighborsOfTileIndex7.length).toBe(4);
  expect(neighborsOfTileIndex7).toEqual(expect.arrayContaining([6, 8, 4, 10]));
});

test('move tile', () => {
  const grid = new Grid([3, 4]);

  expect(grid.moveTile(4)).toBeFalsy(); // move top left of blank
  expect(grid.moveTile(5)).toBeTruthy(); // move blank tile up
  expect(grid.moveTile(0)).toBeFalsy(); // move far away of blank
  expect(grid.moveTile(4)).toBeTruthy(); // move blank tile to left
  expect(grid.moveTile(4)).toBeFalsy(); // move blank tile itself
  expect(grid.moveTile(1)).toBeTruthy(); // move blank tile down

  expect(grid.blankTileIndex).toBe(1);
});

test('grid order', () => {
  const grid = new Grid([3, 4]);
  expect(grid.isInOrder).toBeTruthy();

  grid.moveTile(1);
  expect(grid.isInOrder).toBeFalsy();

  grid.moveTile(2);
  expect(grid.isInOrder).toBeTruthy();

  grid.shuffleTiles();
  expect(grid.isInOrder).toBeFalsy();

  grid.resetTiles();
  expect(grid.isInOrder).toBeTruthy();
});
