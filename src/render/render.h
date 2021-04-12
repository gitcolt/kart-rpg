#include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace Renderer {
  struct Attribute {
    const GLchar* name;
    GLenum type;
    GLint num_components;
  };

  GLuint load_texture(const std::string& filename);

  void setup_shader_attributes(GLuint shader_program, std::vector<Attribute> attributes);

  void set_vertex_array(GLuint* vao);

  void set_buffer(GLenum type, GLuint* buffer);

  template <typename T>
  void buffer_data(GLenum type, T* data, size_t size) {
    glBufferData(type, size, data, GL_STATIC_DRAW);
  }

  void set_index_buffer(GLuint* elements, size_t size);

  void render(Camera& camera, GLuint shader_program);

  GLuint compile_shader(const GLchar* shader_source, const std::string& shader_type);

  GLuint build_shader_program(GLuint vertex_shader, GLuint fragment_shader);
}

