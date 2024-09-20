//
// Created by lrfri on 14.05.2024.
//

#ifndef RAYLIBSTARTER_JOURNAL_H
#define RAYLIBSTARTER_JOURNAL_H

#include <iostream>
#include "raylib.h"
#include "assestmanagergraphics.h"
#include "scene.h"
class gameplay;
class journal :public scene{
public:
    journal(gameplay* gameplayInstance) : scene(gameplayInstance), gameplayInstance(gameplayInstance) {
        if (gameplayInstance == nullptr) {
            std::cout << "Warning: gameplayInstance is null in ingameoptions constructor\n";
        }
    }
    int cursor = 0;

    void update() override;

    scene *evaluateSceneChange() override;

    void draw() override;

    void drawDebug() override;

    //creates the boxes that will be used as buttons and loads the textures; marked boxes will work as an outline to show which button is selected
    Rectangle hitbox_close = {670, 400, 100, 60};
    Rectangle hitbox_close_marked = {660, 390, 120, 80};
    Texture2D closeButton = assestmanagergraphics::getTexture("userinterface/button_close");

    Rectangle journalpage1 = {25, 70, 280, 350};
    Rectangle journalpage1_marked = {15, 60, 300, 370};

    Rectangle journalpage2 = {375, 70, 280, 350};
    Rectangle journalpage2_marked = {365, 60, 300, 370};

    Texture2D journalbackground = assestmanagergraphics::getTexture("userinterface/journal_background_newsize");

    gameplay *gameplayInstance;
};


#endif //RAYLIBSTARTER_JOURNAL_H
