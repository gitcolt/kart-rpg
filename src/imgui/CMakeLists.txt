add_library(imgui
  imgui.cpp
  imgui.h
  imgui_widgets.cpp
  imgui_draw.cpp
  imgui_tables.cpp
  imgui_internal.h
  backends/imgui_impl_opengl3.cpp
  backends/imgui_impl_opengl3.h
  backends/imgui_impl_glfw.cpp
  backends/imgui_impl_glfw.h
  )

target_link_libraries(imgui
  glad
  )

target_include_directories(imgui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

