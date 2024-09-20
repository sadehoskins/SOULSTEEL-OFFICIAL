//
// Created by lrfri on 14.05.2024.
//

#ifndef RAYLIBSTARTER_PAUSESCREEN_H
#define RAYLIBSTARTER_PAUSESCREEN_H

#include "raylib.h"
#include <iostream>
#include "assestmanagergraphics.h"
#include "scene.h"
class gameplay;
class pausescreen:public scene {
private:
    Texture2D logo_soulsteel = assestmanagergraphics::getTexture("userinterface/logo_soulsteel");
    Texture2D logo_team = assestmanagergraphics::getTexture("userinterface/logo_team");
    Texture2D pause = assestmanagergraphics::getTexture("userinterface/pause");

public:
    pausescreen(gameplay* gameplayInstance) : scene(gameplayInstance), gameplayInstance(gameplayInstance) {
        if (gameplayInstance == nullptr) {
            std::cout << "Warning: gameplayInstance is null in pausescreen constructor\n";
        }
    }
    void update() override;

    scene *evaluateSceneChange() override;

    void draw() override;

    void drawDebug() override;

    //loads the necessary textures
    Texture2D pausebutton = assestmanagergraphics::getTexture("userinterface/button_pause");
    Texture2D title = assestmanagergraphics::getTexture("userinterface/title_3");

    //creates the box that will be used as a button
    Rectangle boxforpausebutton = {339,340,122,122};
    gameplay *gameplayInstance;
};
#endif //RAYLIBSTARTER_PAUSESCREEN_H
