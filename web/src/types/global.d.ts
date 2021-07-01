/// <reference types="vite/client" />

declare module '*.vert' {
  const value: string;
  export default value;
}

declare module '*.frag' {
  const value: string;
  export default value;
}

declare type Vec2 = [number, number];

declare type Tile = [
  originalX: number,
  originalY: number,
  currentX: number,
  currentY: number
];

declare enum Direction {
  Top = 'Top',
  Bottom = 'Bottom',
  Left = 'Left',
  Right = 'Right',
}
