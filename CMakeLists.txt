# Render Pipeline CMake

add_library(camera STATIC "${CMAKE_CURRENT_SOURCE_DIR}/Camera/Camera.c")
target_include_directories(camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Camera/include/")
target_link_libraries(camera roots_math)

add_library(render_obj STATIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderObj/render_object.c")
target_include_directories(render_obj PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderObj/include/")
target_link_libraries(render_obj roots_math data_types asset_io glad)

add_library(render_cache STATIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderCache/render_cache.c")
target_include_directories(render_cache PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderCache/include/")
target_link_libraries(render_cache render_obj)

add_executable(render_main "${CMAKE_CURRENT_SOURCE_DIR}/render_main.cpp")
target_link_libraries(render_main glad glfw "${CMAKE_SOURCE_DIR}/Tools/third_party/lib/glfw3.lib" ufbx camera render_cache)