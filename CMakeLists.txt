cmake_minimum_required(VERSION 3.16)
project(game)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")

add_executable(game main.cpp Renderer.cpp Renderer.h VertexArray.cpp VertexArray.h Shader.cpp Shader.h Face.tpp Face.h Vertex.h Vertex.tpp Matrix.tpp Matrix.h)
#Add Libraries
find_package(PkgConfig REQUIRED)

find_package(sdl2)
if(sdl2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(game ${SDL2_LIBRARIES})
    target_include_directories(game PUBLIC ${SDL2_INCLUDE_DIRS})
    target_compile_options(game PUBLIC ${SDL2_CFLAGS_OTHER})
else()
    pkg_check_modules(sdl2 REQUIRED sdl2)
    target_link_libraries(game ${SDL2_LIBRARIES})
    target_include_directories(game PUBLIC ${SDL2_INCLUDE_DIRS})
    target_compile_options(game PUBLIC ${SDL2_CFLAGS_OTHER})
endif(sdl2_FOUND)

find_package(sdl2_image)
if(SDL2_IMAGE_FOUND)
    target_link_libraries(game sdl2_image)
    target_include_directories(game PUBLIC ${sdl2_image_INCLUDE_DIRS})
    target_compile_options(game PUBLIC ${sdl2_image_CFLAGS_OTHER})
else()
    pkg_check_modules(sdl2_image REQUIRED SDL2_image)
    target_link_libraries(game ${sdl2_image_LIBRARIES})
    target_include_directories(game PUBLIC ${sdl2_image_INCLUDE_DIRS})
    target_compile_options(game PUBLIC ${sdl2_image_CFLAGS_OTHER})
endif()


find_package(glew)
add_compile_definitions(GLEW_NO_GLU GLFW_INCLUDE_NONE)
if(GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(game GLEW::glew)
    target_include_directories(game PUBLIC ${glew_INCLUDE_DIRS})
    target_compile_options(game PUBLIC ${glew_CFLAGS_OTHER})
else()
    pkg_check_modules(glew REQUIRED glew)
    target_link_libraries(game ${glew_LIBRARIES})
    target_include_directories(game PUBLIC ${glew_INCLUDE_DIRS})
    target_compile_options(game PUBLIC ${glew_CFLAGS_OTHER})
endif()


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
#copy libraries during build
#TODO: symlink would be better, but is not supported on windows
set (source "${CMAKE_SOURCE_DIR}/libs/")
set (destination "${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "copy libraries from ${source} => ${destination}"
)
