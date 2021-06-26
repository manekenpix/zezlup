export class GameScreen {
  readonly #canvas: HTMLCanvasElement;
  readonly #gl: WebGL2RenderingContext;

  constructor() {
    // set up canvas
    this.#canvas = document.createElement('canvas');
    this.#canvas.style.display = 'block';
    this.#canvas.style.width = '100%';
    this.#canvas.style.height = '100%';

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
    });
    resizeObserver.observe(this.#canvas);
  }

  appendTo(htmlElement: HTMLElement) {
    htmlElement.append(this.#canvas);
  }
}
