cmake_minimum_required(VERSION 3.0)

project(Main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(include external/glad/include external/glfw/include external/stb_image/include)

aux_source_directory(src MAIN_SRC_FILE)
aux_source_directory(external/glad/src MAIN_SRC_FILE)
aux_source_directory(external/stb_image/src MAIN_SRC_FILE)

if (WIN32)
link_directories(external/glfw/lib/win32)
else()
link_directories(external/glfw/lib/linux)
endif()

add_executable(Main ${MAIN_SRC_FILE})

if (WIN32)
target_link_libraries(Main glfw3)
else()
target_link_libraries(Main glfw3 GL X11 pthread Xrandr Xxf86vm Xinerama Xcursor Xi dl)
endif()
