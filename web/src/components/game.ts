import {Grid} from './grid';

export class Game {
  readonly #canvas: HTMLCanvasElement;
  readonly #gl: WebGL2RenderingContext;

  readonly #grid: Grid;

  constructor() {
    // set up canvas
    this.#canvas = document.createElement('canvas');
    this.#canvas.tabIndex = 0;
    this.#canvas.style.display = 'block';
    this.#canvas.style.width = '100%';
    this.#canvas.style.height = '100%';
    this.#canvas.style.outline = 'none';

    // set up webgl context
    const gl = this.#canvas.getContext('webgl2');
    if (!gl) throw Error('failed to get webgl2 context');
    this.#gl = gl;

    // set up auto resize
    const resizeObserver = new ResizeObserver(entries => {
      const {width, height} = entries[0].contentRect;
      this.#canvas.width = width;
      this.#canvas.height = height;
      this.#gl.viewport(0, 0, width, height);
      gl.clearColor(0, 0, 0, 1);
      gl.clear(gl.COLOR_BUFFER_BIT);
    });
    resizeObserver.observe(this.#canvas);

    // set up components
    this.#grid = new Grid(gl, [4, 3]);

    // set up draw loop
    const drawLoop = () => {
      this.draw();
      window.requestAnimationFrame(drawLoop);
    };
    drawLoop();
  }

  appendTo(htmlElement: HTMLElement) {
    htmlElement.append(this.#canvas);
  }

  handleKeyPress(event: KeyboardEvent) {
    event.stopPropagation();
    console.log(event.key);
  }

  draw() {
    this.#gl.clearColor(0, 0, 0, 1);
    this.#gl.clear(this.#gl.COLOR_BUFFER_BIT);
    this.#grid.draw();
  }
}
