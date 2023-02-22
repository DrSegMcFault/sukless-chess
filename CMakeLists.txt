cmake_minimum_required(VERSION 3.25.0)

project(chess LANGUAGES C CXX)

#set(CMAKE_CXX_COMPILIER "clang++")
set(CMAKE_CXX_STANDARD 20)

add_executable(chess)
target_sources(chess PRIVATE main.cpp Piece.cpp Game.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(chess PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(chess PRIVATE SDL2::SDL2_image)

file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})
