cmake_minimum_required(VERSION 3.25.0)
set(CMAKE_CXX_STANDARD 20)

project(chess LANGUAGES C CXX)

#set(CMAKE_CXX_COMPILIER "clang++")

add_executable(chess)
target_sources(chess PRIVATE main.cpp App.cpp Piece.cpp Game.cpp ChessAI.cpp)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

IF (WIN32)

  find_package(SDL2_image CONFIG REQUIRED)
  target_link_libraries(chess
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
  target_link_libraries(chess PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

ELSE()

  find_package(sdl2-image CONFIG REQUIRED)

  target_link_libraries(chess PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static SDL2::SDL2_image)

ENDIF()

target_link_libraries(chess PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})
