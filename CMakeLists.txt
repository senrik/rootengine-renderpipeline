# Render Pipeline CMake

# Camera Requires GLM
add_library(camera STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp")
target_include_directories(camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/" "${CMAKE_SOURCE_DIR}/Tools/third_party/include/")


# RenderObj Requires GLM, UFBX, STBI, GLFW, GLAD
add_library(render_obj STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/render_object.cpp")
target_include_directories(render_obj PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(render_obj glad ufbx stbi "${CMAKE_SOURCE_DIR}/Tools/third_party/lib/glfw3.lib")