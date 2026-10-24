cmake_minimum_required(VERSION 3.16)
project(MojaGra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)

add_executable(${PROJECT_NAME} src/main.cpp
    src/gracz.cpp
    src/manager.cpp
    src/sound.h
    src/sound.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/texture
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/texture
)