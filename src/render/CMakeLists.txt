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
    )

 target_include_directories(render
   PRIVATE
     ${CMAKE_SOURCE_DIR}/src
   )

