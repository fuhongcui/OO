cmake_minimum_required(VERSION 3.0)

project(Main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(include external/glad/include external/glfw/include)

aux_source_directory(src MAIN_SRC_FILE)

aux_source_directory(external/glad/src MAIN_SRC_FILE)

link_directories(external/glfw/lib)

add_executable(Main ${MAIN_SRC_FILE})

target_link_libraries(Main glfw3)
