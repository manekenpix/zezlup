import {Grid} from './grid';

test('grid size', () => {
  const grid = new Grid([3, 4]);
  expect(grid.size).toEqual([3, 4]);
  expect(grid.tiles.length).toBe(3 * 4);

  grid.size = [5, 6];
  expect(grid.size).toEqual([5, 6]);
  expect(grid.tiles.length).toBe(5 * 6);
});

test('neighbor indexes', () => {
  const grid = new Grid([3, 4]);

  expect(grid.getLeftOfIndex(0)).toBe(null);
  expect(grid.getLeftOfIndex(1)).toBe(0);

  expect(grid.getRightOfIndex(2)).toBe(null);
  expect(grid.getRightOfIndex(1)).toBe(2);

  expect(grid.getTopOfIndex(9)).toBe(null);
  expect(grid.getTopOfIndex(8)).toBe(11);

  expect(grid.getBottomOfIndex(2)).toBe(null);
  expect(grid.getBottomOfIndex(3)).toBe(0);

  const neighborsOfTileIndex0 = grid.getNeighborsOfIndex(0);
  expect(neighborsOfTileIndex0.length).toBe(2);
  expect(neighborsOfTileIndex0).toEqual(expect.arrayContaining([1, 3]));

  const neighborsOfTileIndex11 = grid.getNeighborsOfIndex(11);
  expect(neighborsOfTileIndex11.length).toBe(2);
  expect(neighborsOfTileIndex11).toEqual(expect.arrayContaining([10, 8]));

  const neighborsOfTileIndex7 = grid.getNeighborsOfIndex(7);
  expect(neighborsOfTileIndex7.length).toBe(4);
  expect(neighborsOfTileIndex7).toEqual(expect.arrayContaining([6, 8, 4, 10]));

  expect(grid.areTilesNeighbors(1, 2)).toBeTruthy();
  expect(grid.areTilesNeighbors(2, 3)).toBeFalsy();
  expect(grid.areTilesNeighbors(2, 4)).toBeFalsy();
  expect(grid.areTilesNeighbors(2, 5)).toBeTruthy();
});

test('grid order', () => {
  const grid = new Grid([3, 4]);
  expect(grid.isInOrder).toBeTruthy();

  grid.swapTiles(1, 2);
  expect(grid.isInOrder).toBeFalsy();

  grid.swapTiles(1, 2);
  expect(grid.isInOrder).toBeTruthy();

  grid.swapTiles(2, 3);
  grid.swapTiles(4, 5);
  expect(grid.isInOrder).toBeFalsy();

  grid.resetTiles();
  expect(grid.isInOrder).toBeTruthy();
});
