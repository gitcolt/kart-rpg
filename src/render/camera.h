#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

class Camera {
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;
  float yaw;
  float pitch;
  float movement_speed;
  public:
    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
           glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
           float yaw = -90.f,
           float pitch = 0.f);
    glm::mat4 get_view_matrix() const;
    void move_forward(float ticks);
    void move_backward(float ticks);
    void turn_left(float ticks);
    void turn_right(float ticks);
    void update();
};
