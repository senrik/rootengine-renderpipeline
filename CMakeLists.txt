# Render Pipeline CMake

add_library(camera STATIC "${CMAKE_CURRENT_SOURCE_DIR}/Camera/Camera.c")
target_include_directories(camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Camera/include/")
target_link_libraries(camera roots_math)

add_library(render_obj STATIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderObj/render_object.c")
target_include_directories(render_obj PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderObj/include/")
target_link_libraries(render_obj roots_math data_struct glad)

add_library(render_cache STATIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderCache/render_cache.c")
target_include_directories(render_cache PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/RenderCache/include/")
target_link_libraries(render_cache render_obj)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/assets/")
	message("[Render Pipeline] Assets exists, re-importing...")
	file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/assets/")
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets/")

add_executable(render_main "${CMAKE_CURRENT_SOURCE_DIR}/render_main.cpp")
target_link_libraries(render_main glad glfw MY_GLFW_LIB asset_io ufbx camera render_cache)
