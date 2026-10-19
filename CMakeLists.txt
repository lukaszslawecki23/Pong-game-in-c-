cmake_minimum_required(VERSION 3.16)
project(MojaGra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/SDL)

add_executable(${PROJECT_NAME} src/main.cpp
    src/gracz.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)