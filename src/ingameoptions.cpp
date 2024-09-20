//
// Created by lrfri on 28.05.2024.
//

#include "ingameoptions.h"
#include "raylib.h"
#include "gameplay.h"

void ingameoptions::update() {
}

scene *ingameoptions::evaluateSceneChange() {
    if (IsKeyPressed(KEY_O)) {
        return gameplayInstance;
    } else {
        return this;
    }
}

void ingameoptions::draw() {
    DrawText("Controls", 300, 40, 50, WHITE);
    DrawText("Movement:", 50, 120, 25, WHITE);
    DrawText("W: Move up\nA: Move left\nS: Move down\nD: Move right", 200, 120, 20, WHITE);
    DrawText("Abilities:", 50, 280, 25, WHITE);
    DrawText(

            "I: Soul dash (move through bars and over abyss)\nL: Soul Dust (light up fire bowls)\nK: Robot Strength (push stone)\nJ: Robot Weapon (throw bomb)",
            200, 280, 20, WHITE);
    DrawText("Press M to go back to main menu", 50, 440, 20, WHITE);
}

void ingameoptions::drawDebug() {

}

