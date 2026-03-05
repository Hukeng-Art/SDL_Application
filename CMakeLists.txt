cmake_minimum_required(VERSION 3.5)

project( SDL_Application_test )

add_executable( SDL_Application_test main.cpp )

target_link_libraries(SDL_Application_test -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer)
