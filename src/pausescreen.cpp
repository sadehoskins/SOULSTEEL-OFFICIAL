//
// Created by lrfri on 14.05.2024.
//
#include "pausescreen.h"
#include <iostream>
#include "raylib.h"
#include "gameplay.h"
#include "mainmenu.h"

void pausescreen::update() {

}

scene *pausescreen::evaluateSceneChange() {

    if (IsKeyPressed(KEY_P)) {
        return gameplayInstance;  // Return the stored gameplay instance
    }else if (IsKeyPressed(KEY_M)) {
        return new mainmenu();}
    else{
        return this;
    }
}

void pausescreen::draw() {
//draws the background and textures
    DrawTexture(background,0,0,WHITE);
    DrawTexture(logo_soulsteel, 120, 20, WHITE);
    DrawTexture(logo_team, 320, 390, WHITE);

    DrawTexture(pause, 360,235,WHITE);


    DrawText("The game is paused. Press P to continue or M for menu.",
             95, 345, 20, WHITE);
}

void pausescreen::drawDebug() {

}
