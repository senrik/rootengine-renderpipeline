# Render Pipeline CMake

add_library(camera STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp")
target_include_directories(camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(render_obj STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/render_object.cpp")
target_include_directories(render_obj PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")