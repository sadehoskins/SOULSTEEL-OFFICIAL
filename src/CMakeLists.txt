cmake_minimum_required(VERSION 3.11)

include(../cmake/raylib.cmake)

project(game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp
        pausescreen.cpp
        pausescreen.h
        optionen.cpp
        optionen.h
        mainmenu.cpp
        mainmenu.h
        journal.cpp
        journal.h
        gameplay.cpp
        gameplay.h
        maincharacter.cpp
        maincharacter.h
        maincharactermodus.h
        ingameoptions.cpp
        ingameoptions.h
        assestmanagergraphics.cpp
        assestmanagergraphics.h
        scene.cpp
        scene.h
        ENEMIES/Enemy.cpp
        ENEMIES/Enemy.h
        enemymodus.h
        ENEMIES/Enemy1.cpp
        ENEMIES/Enemy1.h
        ENEMIES/Enemy2.cpp
        ENEMIES/Enemy2.h
        "GAME OBJECTS/gameobjects.h"
        "GAME OBJECTS/gameobjects.cpp"
        "GAME OBJECTS/bombs.h"
        "GAME OBJECTS/bombs.cpp"
        "GAME OBJECTS/robot.cpp"
        "GAME OBJECTS/robot.h"
        ENEMIES/Enemy3.cpp
        ENEMIES/Enemy3.h
        Enums.h
        Utils.cpp
        Utils.h
        Wall.cpp
        Wall.h
        gamechoicescreen.cpp
        gamechoicescreen.h
        "GAME OBJECTS/stone.cpp"
        "GAME OBJECTS/stone.h"
        "GAME OBJECTS/movableblocks.cpp"
        "GAME OBJECTS/movableblocks.h"
        "GAME OBJECTS/yellowblock.cpp"
        "GAME OBJECTS/yellowblock.h"
        "GAME OBJECTS/blueblock.cpp"
        "GAME OBJECTS/blueblock.h"
        pausescreen.cpp
        pausescreen.cpp
        pausescreen.cpp
        "GAME OBJECTS/blueblock.cpp"
        AudioPlayer.h
        AudioPlayer.cpp
        HealthManager.cpp
        HealthManager.h
)

# Add config.h.in file
configure_file(config.h.in config.h)
target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
        )

#  Link to Raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets to assets path
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../cmake/copy-assets.cmake"
        )
add_dependencies(${PROJECT_NAME} copy_assets)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()

# CPack instructions
# call e.g. cpack -G ZIP -C Debug to create a binary distribution
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_NAME}_Bundle")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets" DESTINATION "${PROJECT_NAME}_Bundle")