cmake_minimum_required(VERSION 3.20)
project(FruitShark)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ZLIB REQUIRED)

find_library(Enet REQUIRED
        NAMES enet
        PATHS ${ENET_PATH}
        )

find_path(Enet_INCLUDE_DIRS REQUIRED
        NAMES enet/enet.h
        PATHS ${ENET_PATH}/include
        )

get_target_property(SDL2_INCLUDE_DIRS SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_image_INCLUDE_DIRS SDL2_image::SDL2_image INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_ttf_INCLUDE_DIRS SDL2_ttf::SDL2_ttf INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_mixer_INCLUDE_DIRS SDL2_mixer::SDL2_mixer INTERFACE_INCLUDE_DIRECTORIES)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_image_INCLUDE_DIRS})
include_directories(${SDL2_mixer_INCLUDE_DIRS})
include_directories(${SDL2_ttf_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${Enet_INCLUDE_DIRS})

set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/src/engine)

add_library(
        Engine OBJECT
        ${ENGINE_DIR}/game.cpp
        ${ENGINE_DIR}/engine.cpp
        ${ENGINE_DIR}/input.cpp
        ${ENGINE_DIR}/texture.cpp
        ${ENGINE_DIR}/ui.cpp
)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp
        src/sharkGame.cpp
        src/ship.cpp src/shark.cpp
        src/entity.cpp src/fruit.cpp
        src/textureHandler.cpp
        src/mainMenu.cpp
        src/sound.cpp
        src/bite.cpp
        src/pickup.cpp
        src/server.cpp)
target_link_libraries(main Engine)
target_link_libraries(main ${SDL2_LIBRARIES})
target_link_libraries(main SDL2_image::SDL2_image)
target_link_libraries(main SDL2_mixer::SDL2_mixer)
target_link_libraries(main SDL2_ttf::SDL2_ttf)
target_link_libraries(main ZLIB::ZLIB)
target_link_libraries(main ${Enet})
if (WIN32)
    target_link_libraries(main ws2_32.lib)
    target_link_libraries(main winmm.lib)
endif()

add_custom_command (TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:main>
        )
add_custom_command (TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_image::SDL2_image> $<TARGET_FILE_DIR:main>
        )
add_custom_command (TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_mixer::SDL2_mixer> $<TARGET_FILE_DIR:main>
        )
add_custom_command (TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_ttf::SDL2_ttf> $<TARGET_FILE_DIR:main>
        )