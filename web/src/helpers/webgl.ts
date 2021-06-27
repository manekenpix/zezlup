export const createShader = (
  gl: WebGL2RenderingContext,
  type: GLenum,
  source: string
) => {
  const shader = gl.createShader(type);
  if (!shader) throw Error('failed to create shader');

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    console.error(gl.getShaderInfoLog(shader));
    throw Error('failed to compile shader');
  }
  return shader;
};

export const createProgram = (
  gl: WebGL2RenderingContext,
  vertexShaderSrc: string,
  fragmentShaderSrc: string
) => {
  const vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexShaderSrc);
  const framentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentShaderSrc);

  const program = gl.createProgram();
  if (!program) throw Error('failed to create program');

  gl.attachShader(program, vertexShader);
  gl.attachShader(program, framentShader);
  gl.linkProgram(program);

  if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    console.error(gl.getShaderInfoLog(program));
    throw Error('failed to link program');
  }

  gl.deleteShader(vertexShader);
  gl.deleteShader(framentShader);
  return program;
};
