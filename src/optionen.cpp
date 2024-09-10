//
// Created by lrfri on 14.05.2024.
//
#include "optionen.h"
#include "raylib.h"
#include "mainmenu.h"

void optionen::update() {
}

scene *optionen::evaluateSceneChange() {
    if (IsKeyPressed(KEY_M)) {
                return new mainmenu();

    } else {
        return this;
    }
}

void optionen::draw() {
DrawText("Controls", 300,40,50,WHITE);
DrawText("Movement:", 50,120,25,WHITE);
DrawText("W: Move up\nA: Move left\nS: Move down\nD: Move right", 200,120, 20,WHITE);
DrawText("Abilities:", 50, 280, 25, WHITE);
DrawText("I: Soul dash (move through bars and over abyss)\nL: Soul Dust (light up fire bowls)\nK: Robot Strength (push stone)\nJ: Robot Weapon (throw bomb)", 200, 280, 20,WHITE);
DrawText("Press M to go back to main menu", 50, 440, 20,WHITE);
}

void optionen::drawDebug() {

}


