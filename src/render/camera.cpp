#include "camera.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtx/string_cast.hpp"

#include <iostream>

Camera::Camera(glm::vec3 position,
               glm::vec3 up,
               float yaw,
               float pitch) :
               position{position},
               world_up{up},
               yaw{yaw},
               pitch{pitch},
               front{glm::vec3{0.f, 0.f, -1.f}},
               movement_speed{2.5f} {
  update();
}

void Camera::move_forward(float ticks) {
  position += front * (float)(ticks * 0.3);
}

void Camera::move_backward(float ticks) {
  position -= front * (float)(ticks * 0.3);
}

void Camera::turn_left(float ticks) {
  yaw -= ticks * 80.f;
}

void Camera::turn_right(float ticks) {
  yaw += ticks * 80.f;
}


glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(position, position + front, up);
}

void Camera::update() {
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));

  /*
  std::cout << "position: " << glm::to_string(this->position) << "\n";
  std::cout << "world_up: " << glm::to_string(this->world_up) << "\n";
  std::cout << "yaw: " << this->yaw << "\n";
  std::cout << "pitch: " << this->pitch << "\n";
  std::cout << "front: " << glm::to_string(this->front) << "\n";
  std::cout << "movement_speed: " << this->movement_speed << "\n";
  */
}
