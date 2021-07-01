import {Grid} from './grid';

test('grid size', () => {
  const grid = new Grid(1, 2);
  expect(grid.size).toEqual([1, 2]);
  expect(grid.width).toBe(1);
  expect(grid.height).toBe(2);
  expect(grid.numTiles).toBe(1 * 2);
  expect(grid.tilesByCell).toEqual([0, 1]);

  grid.size = [2, 3];
  expect(grid.size).toEqual([2, 3]);
  expect(grid.width).toBe(2);
  expect(grid.height).toBe(3);
  expect(grid.numTiles).toBe(2 * 3);
  expect(grid.tilesByCell).toEqual([0, 1, 2, 3, 4, 5]);
});

test('neighbor cells', () => {
  const grid = new Grid(3, 3);

  expect(grid.getLeftOfCell(0)).toBe(null);
  expect(grid.getLeftOfCell(1)).toBe(0);

  expect(grid.getRightOfCell(2)).toBe(null);
  expect(grid.getRightOfCell(1)).toBe(2);

  expect(grid.getTopOfCell(8)).toBe(null);
  expect(grid.getTopOfCell(5)).toBe(8);

  expect(grid.getBottomOfCell(2)).toBe(null);
  expect(grid.getBottomOfCell(5)).toBe(2);

  const neighborsOfCell0 = grid.getNeighborsOfCell(0);
  expect(neighborsOfCell0.length).toBe(2);
  expect(neighborsOfCell0).toEqual(expect.arrayContaining([1, 3]));

  const neighborsOfCell1 = grid.getNeighborsOfCell(1);
  expect(neighborsOfCell1.length).toBe(3);
  expect(neighborsOfCell1).toEqual(expect.arrayContaining([0, 2, 4]));

  const neighborsOfCell4 = grid.getNeighborsOfCell(4);
  expect(neighborsOfCell4.length).toBe(4);
  expect(neighborsOfCell4).toEqual(expect.arrayContaining([1, 3, 5, 7]));

  expect(grid.areCellsNeighbors(0, 1)).toBeTruthy();
  expect(grid.areCellsNeighbors(0, 2)).toBeFalsy();
  expect(grid.areCellsNeighbors(0, 3)).toBeTruthy();
  expect(grid.areCellsNeighbors(0, 4)).toBeFalsy();
});

test('grid order', () => {
  const grid = new Grid(3, 3);
  expect(grid.isInOrder).toBeTruthy();

  grid.swapTilesAtCells(0, 1);
  expect(grid.isInOrder).toBeFalsy();

  grid.swapTilesAtCells(1, 2);
  grid.swapTilesAtCells(2, 0);
  grid.swapTilesAtCells(0, 1);
  grid.swapTilesAtCells(1, 2);
  grid.swapTilesAtCells(2, 0);
  expect(grid.isInOrder).toBeTruthy();
});
