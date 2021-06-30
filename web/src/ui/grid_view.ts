import {createProgram} from '../helpers/webgl';
import vertexShaderSrc from './grid_view.vert';
import fragmentShaderSrc from './grid_view.frag';
import {Game} from '../helpers/game';
import imageUrl from '../images/win_meme.png';

export class GridView {
  private readonly _game: Game;
  private readonly _canvas: HTMLCanvasElement;
  private readonly _gl: WebGL2RenderingContext;

  private readonly _program: WebGLProgram;
  private readonly _canvasSizeLoc: WebGLUniformLocation | null;
  private readonly _gridSizeLoc: WebGLUniformLocation | null;
  private readonly _imageSizeLoc: WebGLUniformLocation | null;
  private readonly _tilesLoc: WebGLUniformLocation | null;
  private readonly _blankTileIndexLoc: WebGLUniformLocation | null;
  private readonly _selectedTileIndexLoc: WebGLUniformLocation | null;
  private readonly _verticesLoc: WebGLUniformLocation | null;
  private readonly _imageLoc: WebGLUniformLocation | null;

  _imageSize: Vec2 = [1, 1];

  constructor(game: Game, canvas: HTMLCanvasElement) {
    this._game = game;

    const gl = canvas.getContext('webgl2');
    if (!gl) throw Error('failed to get webgl2 context');
    this._canvas = canvas;
    this._gl = gl;

    this._program = createProgram(gl, vertexShaderSrc, fragmentShaderSrc);
    this._canvasSizeLoc = this.getUniformLoc('canvasSize');
    this._gridSizeLoc = this.getUniformLoc('gridSize');
    this._imageSizeLoc = this.getUniformLoc('imageSize');
    this._tilesLoc = this.getUniformLoc('tiles');
    this._blankTileIndexLoc = this.getUniformLoc('blankTileIndex');
    this._selectedTileIndexLoc = this.getUniformLoc('selectedTileIndex');
    this._verticesLoc = this.getUniformLoc('vertices');
    this._imageLoc = this.getUniformLoc('image');

    this.setupAutoResize();

    const image = new Image();
    image.onload = () => {
      this._imageSize = [image.width, image.height];
      gl.activeTexture(gl.TEXTURE0);
      const tex = gl.createTexture();
      gl.bindTexture(gl.TEXTURE_2D, tex);
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);
      gl.generateMipmap(gl.TEXTURE_2D);
    };
    image.src = imageUrl;
  }

  private setupAutoResize() {
    const resizeObserver = new ResizeObserver(entries => {
      const {width, height} = entries[0].contentRect;
      this._canvas.width = width;
      this._canvas.height = height;
      this._gl.viewport(0, 0, width, height);
      this.draw();
    });
    resizeObserver.observe(this._canvas);
  }

  private getUniformLoc(uniformName: string) {
    return this._gl.getUniformLocation(this._program, uniformName);
  }

  draw() {
    this._gl.clearColor(0, 0, 0, 1);
    this._gl.clear(this._gl.COLOR_BUFFER_BIT);

    this._gl.useProgram(this._program);
    this._gl.uniform2f(
      this._canvasSizeLoc,
      this._canvas.width,
      this._canvas.height
    );
    this._gl.uniform2f(this._gridSizeLoc, ...this._game.gridSize);
    this._gl.uniform2f(this._imageSizeLoc, ...this._imageSize);
    this._gl.uniform1iv(this._tilesLoc, [...this._game.tiles]);
    this._gl.uniform1i(this._blankTileIndexLoc, this._game.blankTileIndex);
    this._gl.uniform1i(
      this._selectedTileIndexLoc,
      this._game.selectedTileIndex
    );
    this._gl.uniform2fv(this._verticesLoc, VERTICES);
    this._gl.uniform1i(this._imageLoc, 0);

    this._gl.drawArraysInstanced(
      this._gl.TRIANGLES,
      0,
      VERTICES.length,
      this._game.tiles.length
    );
  }
}

/*  4--3 2
 *  | / /|
 *  |/ / |
 *  5 0--1  */
const VERTICES = [0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0];
