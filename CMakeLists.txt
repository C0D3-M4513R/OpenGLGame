cmake_minimum_required(VERSION 3.10)
project(game)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g -Wall -Wextra -D_GLIBCXX_DEBUG")

add_executable(game main.cpp World/Renderer.cpp OpenGL/VertexArray.cpp OpenGL/Shader.cpp Object/Face.cpp Object/Camera.cpp STLParser/parse_stl.cpp Callback/Keyboard.cpp Object/Player.cpp Callback/Window.cpp Callback/Debug.cpp World/Scene.cpp)
#Add Libraries
find_package(PkgConfig REQUIRED)

add_compile_definitions(GLEW_NO_GLU GLFW_INCLUDE_NONE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(game Threads::Threads)

pkg_check_modules(GL REQUIRED IMPORTED_TARGET glew)
include_directories(${GL_INCLUDE_DIRS})
target_link_libraries(game ${GL_STATIC_LIBRARIES})

pkg_check_modules(GL REQUIRED IMPORTED_TARGET glfw3)
include_directories(${GL_INCLUDE_DIRS})
target_link_libraries(game ${GL_STATIC_LIBRARIES})

ADD_SUBDIRECTORY(glfreetype)
include_directories(glfreetype/include)
target_link_libraries(game glfreetype)


#copy resources during build
#TODO: symlink would be better, but is not supported on windows
set (source "${CMAKE_SOURCE_DIR}/resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "copy resources folder from ${source} => ${destination}"
)
