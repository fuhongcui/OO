cmake_minimum_required(VERSION 3.0)

project(Main)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

include_directories(include external/glad/include external/glfw/include external/stb_image/include external/glm/include)

aux_source_directory(src MAIN_SRC_FILE)
aux_source_directory(external/glad/src MAIN_SRC_FILE)
aux_source_directory(external/stb_image/src MAIN_SRC_FILE)

if (MSVC)
link_directories(external/glfw/lib/win32)
elseif(MINGW)
link_directories(external/glfw/lib/mingw)
else()
link_directories(external/glfw/lib/linux)
endif()

add_executable(Main ${MAIN_SRC_FILE})

if(MSVC)
target_link_libraries(Main glfw3)
elseif(MINGW)
target_link_libraries(Main glfw3)
else()
target_link_libraries(Main glfw3 GL X11 pthread Xrandr Xxf86vm Xinerama Xcursor Xi dl)
endif()
