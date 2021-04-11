#include "render.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <string>
#include <numeric>
#include <iostream>


static float y_translate = -0.1f;

void check_compile_errors(GLuint shader, std::string type) {
  GLint success;
  GLchar info_log[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, info_log);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n";
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, info_log);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n";
    }
  }
}

GLuint Renderer::compile_shader(const GLchar* shader_source, const std::string& shader_type) {
  GLuint shader;
  if (shader_type == "VERTEX")
    shader = glCreateShader(GL_VERTEX_SHADER);
  else if (shader_type == "FRAGMENT")
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  else {
    std::cout << "Unknown shader type \"" << shader_type <<  "\"\n";
    return 0;
  }
  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);
  check_compile_errors(shader, shader_type);
  return shader;
}

GLuint Renderer::build_shader_program(GLuint vertex_shader, GLuint fragment_shader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  check_compile_errors(program, "PROGRAM");

  return program;
}

void Renderer::setup_shader_attributes(GLuint shader_program, std::vector<Attribute> attributes) {
  glUseProgram(shader_program);

  int total = std::accumulate(attributes.begin(), attributes.end(), 0,
      [](int accumulator, const Attribute& attrib) {
        return accumulator + attrib.num_components;
      });

  int components_offset = 0;
  for (auto& attrib : attributes)  {
    GLint loc = glGetAttribLocation(shader_program, attrib.name);
    glEnableVertexAttribArray(loc);

    GLsizei stride = total*sizeof(attrib.type);
    const void* offset = (void*)(components_offset*sizeof(attrib.type));
    components_offset += attrib.num_components;
    glVertexAttribPointer(loc, attrib.num_components, attrib.type, GL_FALSE, stride, offset);
  }
}

GLuint Renderer::load_texture(const char* filename) {
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  int width;
  int height;
  int num_color_channels;
  unsigned char* data = stbi_load(filename, &width, &height, &num_color_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  } else {
    std::cout << "Failed to load image.\n";
    std::cout << stbi_failure_reason();
  }
  stbi_image_free(data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  return tex;
}
  
void Renderer::set_vertex_array(GLuint *vao) {
  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);
}

void Renderer::set_buffer(GLenum type, GLuint* buffer) {
  glGenBuffers(1, buffer);
  glBindBuffer(type, *buffer);
}

void Renderer::render(GLuint shader_program) {
  static auto t_start = std::chrono::high_resolution_clock::now();

  glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  auto t_now = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

  glm::mat4 mat_model = glm::mat4(1.0f);
  mat_model = glm::translate(mat_model, glm::vec3(0.0f, y_translate, -0.3f));
  mat_model = glm::rotate(mat_model, glm::radians(time * 10.f), glm::vec3(0.0f, 1.0f, 0.0f));
  mat_model = glm::rotate(mat_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  static float aspect_ratio = (1280.f/720.f);
  glm::mat4 mat_projection = glm::perspective(glm::radians(45.0f), (float)(aspect_ratio), 0.1f, 100.0f);
  glm::mat4 mvp = mat_projection * mat_model;

  GLint mvp_loc = glGetUniformLocation(shader_program, "mvp");
  glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  ImGui::Text(":)");
  ImGui::Text("%.2f FPS", ImGui::GetIO().Framerate);
  ImGui::SliderFloat("y_translate", &y_translate, -0.3f, 0.3f);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
