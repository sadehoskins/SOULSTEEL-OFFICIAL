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
    DrawText("Controls", 300,20,35,WHITE);
    DrawText("Movement:", 40,90,23,WHITE);
    DrawText("W: Move up\nA: Move left\nS: Move down\nD: Move right", 190,90, 20,WHITE);
    DrawText("Abilities:", 40, 230, 23, WHITE);
    DrawText("I: Soul dash (move through bars and over abyss)\nL: Soul Dust (light up fire bowls)\nK: Robot Strength (push stone)\nJ: Robot Weapon (throw bomb)", 190, 230, 20,WHITE);
    DrawText("Spacebar: Switch between characters (soul needs to be close to robot)\nU: Punch/activate switch (robot)\n", 40, 370,20, WHITE);
    DrawText("Press M to go back to main menu", 40, 450, 20,WHITE);
}

void optionen::drawDebug() {

}


