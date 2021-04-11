R"glsl(
#version 150 core
in vec2 texcoord_out;
out vec4 out_color;
uniform sampler2D tex;
void main() {
  out_color = texture(tex, texcoord_out);
}
)glsl"
