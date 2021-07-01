import {createProgram} from '../helpers/webgl';
import vertexShaderSrc from './game_view.vert';
import fragmentShaderSrc from './game_view.frag';

export class GameView {
  readonly #canvas: HTMLCanvasElement;
  readonly #gl: WebGL2RenderingContext;
  readonly #program: WebGLProgram;

  readonly #canvasSizeLoc: WebGLUniformLocation | null;
  readonly #gridSizeLoc: WebGLUniformLocation | null;
  readonly #tilesByCellLoc: WebGLUniformLocation | null;
  readonly #blankCellLoc: WebGLUniformLocation | null;
  readonly #currentCellLoc: WebGLUniformLocation | null;
  readonly #verticesLoc: WebGLUniformLocation | null;

  readonly #image: WebGLTexture | null;
  readonly #imageLoc: WebGLUniformLocation | null;
  readonly #imageSizeLoc: WebGLUniformLocation | null;

  onStateChange?: Function;

  constructor() {
    const canvas = document.createElement('canvas');
    const gl = canvas.getContext('webgl2');
    if (!gl) throw Error('failed to get webgl2 context');

    this.#canvas = canvas;
    this.#gl = gl;
    this.#program = createProgram(gl, vertexShaderSrc, fragmentShaderSrc);

    this.#canvasSizeLoc = this.getUniformLoc('canvasSize');
    this.#gridSizeLoc = this.getUniformLoc('gridSize');
    this.#tilesByCellLoc = this.getUniformLoc('tilesByCell');
    this.#blankCellLoc = this.getUniformLoc('blankCell');
    this.#currentCellLoc = this.getUniformLoc('currentCell');
    this.#verticesLoc = this.getUniformLoc('vertices');

    this.#image = gl.createTexture();
    this.#imageLoc = this.getUniformLoc('image');
    this.#imageSizeLoc = this.getUniformLoc('imageSize');

    this.setupAutoResize();
  }

  private setupAutoResize() {
    this.#canvas.style.width = '100%';
    this.#canvas.style.height = '100%';

    const resizeObserver = new ResizeObserver(entries => {
      const {width, height} = entries[0].contentRect;
      this.#canvas.width = width;
      this.#canvas.height = height;
      this.#gl.viewport(0, 0, width, height);
      this.onStateChange?.();
    });
    resizeObserver.observe(this.#canvas);
  }

  private getUniformLoc(uniformName: string) {
    return this.#gl.getUniformLocation(this.#program, uniformName);
  }

  set image(value: HTMLImageElement) {
    const gl = this.#gl;
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, this.#image);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, value);
    gl.generateMipmap(gl.TEXTURE_2D);

    this.onStateChange?.();
  }

  appendTo(htmlElement: HTMLElement) {
    htmlElement.append(this.#canvas);
  }

  draw(
    gridSize: Readonly<Vec2>,
    tilesByCell: Readonly<number[]>,
    blankCell: number,
    currentCell: number,
    imageSize: Vec2
  ) {
    const gl = this.#gl;

    gl.clearColor(0, 0, 0, 1);
    gl.clear(this.#gl.COLOR_BUFFER_BIT);
    gl.useProgram(this.#program);

    gl.uniform2f(this.#canvasSizeLoc, this.#canvas.width, this.#canvas.height);
    gl.uniform2fv(this.#gridSizeLoc, gridSize as Vec2);
    gl.uniform1iv(this.#tilesByCellLoc, tilesByCell as number[]);
    gl.uniform1i(this.#blankCellLoc, blankCell);
    gl.uniform1i(this.#currentCellLoc, currentCell);
    gl.uniform2fv(this.#verticesLoc, VERTICES);

    gl.uniform1i(this.#imageLoc, 0);
    gl.uniform2fv(this.#imageSizeLoc, imageSize);

    gl.drawArraysInstanced(
      gl.TRIANGLES,
      0,
      VERTICES.length,
      tilesByCell.length
    );
  }
}

/*  4--3 2
 *  | / /|
 *  |/ / |
 *  5 0--1  */
const VERTICES = [0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0];
