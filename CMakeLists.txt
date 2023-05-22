cmake_minimum_required(VERSION 3.20)
project(FruitShark)

set(CMAKE_CXX_STANDARD 17)

if (LOCAL_LIBS)
    if (MSVC)
        set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libs-msvc)
    else()
        set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/libs-minGW)
    endif()
    set(ENET_PATH ${CMAKE_PREFIX_PATH}/enet)
    set(ZLIB_ROOT ${CMAKE_PREFIX_PATH}/zlib)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ZLIB REQUIRED)

if (MSVC)
    find_library(Enet REQUIRED NAMES enet PATHS ${ENET_PATH})
else()
    find_library(Enet REQUIRED NAMES libenet PATHS ${ENET_PATH})
endif()

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

add_executable(main src/main.cpp
        src/sharkGame.cpp
        src/entities/ship.cpp src/entities/shark.cpp
        src/entities/entity.cpp src/entities/fruit.cpp
        src/asset_handlers/textureHandler.cpp
        src/mainMenu.cpp
        src/asset_handlers/sound.cpp
        src/entities/bite.cpp
        src/entities/pickup.cpp
        src/gameState.cpp
        src/net/client.cpp)

add_executable(server src/net/serverMain.cpp
        src/entities/ship.cpp src/entities/shark.cpp
        src/entities/entity.cpp src/entities/fruit.cpp
        src/asset_handlers/textureHandler.cpp
        src/entities/pickup.cpp
        src/gameState.cpp
        src/net/server.cpp)

target_link_libraries(main Engine)
target_link_libraries(main ${SDL2_LIBRARIES})
target_link_libraries(main SDL2_image::SDL2_image)
target_link_libraries(main SDL2_mixer::SDL2_mixer)
target_link_libraries(main SDL2_ttf::SDL2_ttf)
target_link_libraries(main ZLIB::ZLIB)
target_link_libraries(main ${Enet})

target_link_libraries(server Engine)
target_link_libraries(server ${SDL2_LIBRARIES})
target_link_libraries(server SDL2_image::SDL2_image)
target_link_libraries(server SDL2_ttf::SDL2_ttf)
target_link_libraries(server ZLIB::ZLIB)
target_link_libraries(server ${Enet})
if (WIN32)
    target_link_libraries(main ws2_32.lib)
    target_link_libraries(main winmm.lib)
    target_link_libraries(server ws2_32.lib)
    target_link_libraries(server winmm.lib)

    cmake_path(GET ZLIB_LIBRARIES PARENT_PATH ZLIB_DIR)
    cmake_path(GET ZLIB_DIR PARENT_PATH ZLIB_DIR)
    set(ZLIB_DIR ${ZLIB_DIR}/bin)

    if(MSVC)
        set(ZLIB_DLL ${ZLIB_DIR}/zlib.dll)
    else()
        set(ZLIB_DLL ${ZLIB_DIR}/libzlib.dll)
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
add_custom_command (TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ZLIB_DLL} $<TARGET_FILE_DIR:main>
        )

endif()

install (
	TARGETS main server
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)

if (WIN32)
install (
	FILES
	$<TARGET_FILE:SDL2::SDL2>
	$<TARGET_FILE:SDL2_ttf::SDL2_ttf>
	$<TARGET_FILE:SDL2_image::SDL2_image>
	$<TARGET_FILE:SDL2_mixer::SDL2_mixer>
	${ZLIB_DLL}
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)
endif()

install (
	DIRECTORY
	assets
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)