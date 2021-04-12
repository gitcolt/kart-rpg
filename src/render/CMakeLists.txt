add_library(camera
  camera.cpp
  camera.h
  )

target_link_libraries(camera
  PRIVATE
    glm
    )

 target_include_directories(camera
   PRIVATE
     ${CMAKE_SOURCE_DIR}/src
   )

add_library(render
  render.cpp
  render.h
  )

target_link_libraries(render
  PRIVATE
    glfw
    GL
    dl
    glad
    imgui
    stb_image
    glm
  PUBLIC
    camera
    )

 target_include_directories(render
   PRIVATE
     ${CMAKE_SOURCE_DIR}/src
   )

