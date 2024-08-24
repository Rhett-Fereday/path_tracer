target_sources(${PROJECT_NAME}
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(utils)
add_subdirectory(image_buffers)