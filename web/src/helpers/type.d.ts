declare type Vec2 = [number, number];

declare type Vec2String = `${number},${number}`;

declare module '*.vert' {
  const value: string;
  export default value;
}

declare module '*.frag' {
  const value: string;
  export default value;
}
