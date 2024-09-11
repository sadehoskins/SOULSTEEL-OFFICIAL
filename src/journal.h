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
    int cursor = 1;
    int page=0;

    void update() override;

    scene *evaluateSceneChange() override;

    void draw() override;

    void drawDebug() override;

    void journalnavigation();

    void flippingpages();

    bool closejournal = false;

    void drawpage0();
    void drawpage1();
    void drawpage2();

    const char *day1text ="The experiments are going\nwell. The energy retention\nthrough the souls is\noutstanding, although\nI initially had my\nconcerns since we know so\nlittle about souls.";
    const char *day2text ="Today, the devices showed\nerrors even though there\nwere none. The equipment\nis supposed to be up\nto date, so this shouldn't\nbe happening. Is it a\nbad omen or just a\ncoincidence...?";

    //creates the boxes that will be used as buttons and loads the textures; marked boxes will work as an outline to show which button is selected
    Rectangle hitbox_close = {670, 400, 100, 60};
    Rectangle hitbox_close_marked = {660, 390, 120, 80};
    Texture2D closeButton = assestmanagergraphics::getTexture("userinterface/button_close");

    Rectangle arrowback = {16, 32*7+16, 32, 32};
    Rectangle arrowback_marked = {6, 32*7+6, 52, 52};

    Rectangle arrowforward = {32*24-16, 32*7+16, 32, 32};
    Rectangle arrowforward_marked = {32*24-26, 32*7+6, 52, 52};

    Texture2D journalbackground = assestmanagergraphics::getTexture("userinterface/journal_background_newsize");

    gameplay *gameplayInstance;
};


#endif //RAYLIBSTARTER_JOURNAL_H
