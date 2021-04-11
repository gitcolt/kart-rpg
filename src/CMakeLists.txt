add_executable(main
main.cpp
)

target_link_libraries(main
  PRIVATE
    glfw
    GL
    dl
    glad
    imgui
    #pthread ?
    render
    )
  
add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(stb_image)
add_subdirectory(render)
add_subdirectory(glm)

add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${PROJECT_SOURCE_DIR}/src/assets"
  $<TARGET_FILE_DIR:main>/assets
  )

