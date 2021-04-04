add_library(imgui
  imgui.cpp
  imgui.h
  imgui_widgets.cpp
  imgui_draw.cpp
  imgui_tables.cpp
  imgui_internal.h
  )

target_include_directories(imgui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  )

