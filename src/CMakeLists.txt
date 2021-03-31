add_executable(main
  main.cpp
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_opengl3.h
  imgui/backends/imgui_impl_glfw.cpp
  imgui/backends/imgui_impl_glfw.h
  )

target_link_libraries(main
  PUBLIC
    glfw
    GL
    dl
    glad
    imgui
    #pthread ?
    )
  
target_include_directories(main
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/linmath
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image
    )

add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(linmath)
add_subdirectory(stb_image)

add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/src/assets"
  $<TARGET_FILE_DIR:main>/assets
  )

