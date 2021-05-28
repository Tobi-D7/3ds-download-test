uniform mat4 Projection;
attribute vec4 Position;
const vec4 RED = vec4(1.0, 0.0, 0.0, 1.0);

void main() {
  gl_Position = Projection * Position;
  gl_FrontColor = RED;
  asm("end");
}