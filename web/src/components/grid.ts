import {Vec2} from '../helpers/type';
import {createProgram} from '../helpers/webgl';
import fragmentShaderSrc from './grid.frag';
import vertexShaderSrc from './grid.vert';

export class Grid {
  #size!: Vec2; // [numRows, numColumns]
  #tileSize!: Vec2; // [1 / numRows, 1 / numColumns]
  #tiles!: number[];
  #tileColors!: number[];

  readonly #gl: WebGL2RenderingContext;
  readonly #program: WebGLProgram;
  readonly #gridSizeLoc: WebGLUniformLocation | null;
  readonly #tileSizeLoc: WebGLUniformLocation | null;
  readonly #tilesLoc: WebGLUniformLocation | null;
  readonly #verticesLoc: WebGLUniformLocation | null;
  readonly #tileColorsLoc: WebGLUniformLocation | null;

  constructor(gl: WebGL2RenderingContext, size: Vec2) {
    this.#gl = gl;
    this.#program = createProgram(gl, vertexShaderSrc, fragmentShaderSrc);
    this.#gridSizeLoc = gl.getUniformLocation(this.#program, 'gridSize');
    this.#tileSizeLoc = gl.getUniformLocation(this.#program, 'tileSize');
    this.#tilesLoc = gl.getUniformLocation(this.#program, 'tiles');
    this.#verticesLoc = gl.getUniformLocation(this.#program, 'vertices');
    this.#tileColorsLoc = gl.getUniformLocation(this.#program, 'tileColors');

    this.size = size;
  }

  set size([width, height]: Vec2) {
    this.#size = [width, height];
    this.#tileSize = [1 / width, 1 / height];
    this.#tiles = [...Array(width * height).keys()];
    this.#tileColors = this.#tiles.flatMap(() => [
      Math.random(),
      Math.random(),
      Math.random(),
      1,
    ]);
  }

  draw() {
    this.#gl.useProgram(this.#program);

    this.#gl.uniform2fv(this.#gridSizeLoc, this.#size);
    this.#gl.uniform2fv(this.#tileSizeLoc, this.#tileSize);
    this.#gl.uniform1iv(this.#tilesLoc, this.#tiles);
    this.#gl.uniform2fv(this.#verticesLoc, VERTICES);
    this.#gl.uniform4fv(this.#tileColorsLoc, this.#tileColors);

    this.#gl.drawArraysInstanced(
      this.#gl.TRIANGLES,
      0,
      VERTICES.length / 2,
      this.#tiles.length
    );
  }
}

//    4--3 2
//    | / /|
//    |/ / |
//    5 0--1
const VERTICES = [0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0];
