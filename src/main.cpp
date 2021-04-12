#include "render/render.h"
#include "render/camera.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <chrono>
#include <bitset>

static Camera cam{};
class Input {
  private:
    std::bitset<16> actions{};
  public:
    enum Action {
      MOVE_FORWARD,
      MOVE_BACKWARD,
      TURN_LEFT,
      TURN_RIGHT
    };
    void set_action(Action a) {
      actions.set(a);
    }
    void unset_action(Action a) {
      actions.reset(a);
    }
    bool is_action_set(Action a) {
      return actions.test(a);
    }
} input;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_W) {
    if (action == GLFW_PRESS)
      input.set_action(Input::Action::MOVE_FORWARD);
    else if (action == GLFW_RELEASE)
      input.unset_action(Input::Action::MOVE_FORWARD);
  }
  if (key == GLFW_KEY_S) {
    if (action == GLFW_PRESS)
      input.set_action(Input::Action::MOVE_BACKWARD);
    else if (action == GLFW_RELEASE)
      input.unset_action(Input::Action::MOVE_BACKWARD);
  }
  if (key == GLFW_KEY_A) {
    if (action == GLFW_PRESS)
      input.set_action(Input::Action::TURN_LEFT);
    else if (action == GLFW_RELEASE)
      input.unset_action(Input::Action::TURN_LEFT);
  }
  if (key == GLFW_KEY_D) {
    if (action == GLFW_PRESS)
      input.set_action(Input::Action::TURN_RIGHT);
    else if (action == GLFW_RELEASE)
      input.unset_action(Input::Action::TURN_RIGHT);
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void error_callback(int error, const char* description) {
  std::cerr << "Error: " << description << "\n";
}

static void process_input(float ticks) {
  if (input.is_action_set(Input::Action::MOVE_FORWARD)) {
    cam.move_forward(ticks);
  }
  if (input.is_action_set(Input::Action::MOVE_BACKWARD)) {
    cam.move_backward(ticks);
  }
  if (input.is_action_set(Input::Action::TURN_LEFT)) {
    cam.turn_left(ticks);
  }
  if (input.is_action_set(Input::Action::TURN_RIGHT)) {
    cam.turn_right(ticks);
  }
}

int main() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const int screen_width = 1280;
  const int screen_height = 720;
  GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Kart RPG", NULL, NULL);
  if (!window)
    std::cerr << "Failed to create window\n";

  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  bool err = gladLoadGL() == 0;
  if (err)
    std::cerr << "Failed to initialize OpenGL context\n";

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 150 core";
  ImGui_ImplOpenGL3_Init(glsl_version);
  ImGui::StyleColorsDark();

  GLfloat vertices[] = {
  // x, y, z              u, v
    -0.5f,  0.5f, 0.0f,   0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f
  };

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };

  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  Renderer::set_vertex_array(&vao);
  Renderer::set_buffer(GL_ARRAY_BUFFER, &vbo);
  Renderer::set_buffer(GL_ELEMENT_ARRAY_BUFFER, &ebo);
  Renderer::buffer_data(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
  Renderer::buffer_data(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));

  const GLchar* vert_shader_source =
    #include "shaders/vert.glsl"
    ;
  const GLchar* frag_shader_source =
    #include "shaders/frag.glsl"
    ;

  GLuint vert_shader = Renderer::compile_shader(vert_shader_source, "VERTEX");
  GLuint frag_shader = Renderer::compile_shader(frag_shader_source, "FRAGMENT");
  GLuint shader_program = Renderer::build_shader_program(vert_shader, frag_shader);
  std::vector<Renderer::Attribute> attribs {
    {
      .name = "pos",
      .type = GL_FLOAT,
      .num_components = 3
    },
    {
      .name = "texcoord",
      .type = GL_FLOAT,
      .num_components = 2
    }
  };

  Renderer::setup_shader_attributes(shader_program, attribs);
  Renderer::load_texture("src/assets/course.png");

  auto t_prev = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    auto t_now = std::chrono::high_resolution_clock::now();
    float ticks = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_prev).count();
    t_prev = t_now;
    glfwPollEvents();
    process_input(ticks);
    cam.update();
    Renderer::render(cam, shader_program);
    glfwSwapBuffers(window);
  }

  glDeleteProgram(shader_program);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}

