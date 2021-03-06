R"glsl(
#version 150 core
in vec3 pos;
in vec2 texcoord;
out vec2 texcoord_out;
uniform mat4 mvp;

void main() {
  gl_Position = mvp * vec4(pos, 1.0);
  texcoord_out = vec2(texcoord.x, texcoord.y);
}
)glsl"
