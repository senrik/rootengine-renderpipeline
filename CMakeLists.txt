# Render Pipeline CMake

# RenderObj Requires UFBX, STBI, GLFW, GLAD
add_library(roots_math STATIC "src/roots_math/roots_math.c")
target_include_directories(roots_math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/roots_math/")
target_link_libraries(roots_math data_types)


# Camera
add_library(camera STATIC "src/Camera.c")
target_include_directories(camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(camera roots_math)


add_library(render_obj STATIC "src/render_object.c")
target_include_directories(render_obj PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(render_obj glad roots_math asset_io "${CMAKE_SOURCE_DIR}/Tools/third_party/lib/glfw3.lib")

add_library(render_cache STATIC "src/render_cache.c")
target_include_directories(render_cache PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(render_cache rander_obj)