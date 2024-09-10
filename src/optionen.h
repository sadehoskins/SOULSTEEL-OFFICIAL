//
// Created by lrfri on 14.05.2024.
//

#ifndef RAYLIBSTARTER_OPTIONEN_H
#define RAYLIBSTARTER_OPTIONEN_H

#include "raylib.h"
#include <iostream>
#include "assestmanagergraphics.h"
#include "scene.h"

class optionen: public scene {
public:
    optionen() : scene() {}

    int cursor=0;

    void update() override;

    scene *evaluateSceneChange() override;

    void draw() override;

    void drawDebug() override;

    //creates the boxes that will be used as buttons
    //the marked version is used to outline the button that is currently selected
    Rectangle hitbox_flagDE = {50, 270, 150, 70};
    Rectangle hitbox_flagDE_marked = {40, 260, 170, 90};

    Rectangle hitbox_flagENG = {450, 270, 150, 70};
    Rectangle hitbox_flagENG_marked = {440, 260, 170, 90};

    Rectangle hitbox_close = {640, 360, 150, 70};
    Rectangle hitbox_close_marked = {630, 350, 170, 90};

    Rectangle hitbox_tastatur ={50, 90,150,70};
    Rectangle hitbox_tastatur_marked={40, 80, 170, 90};

    Rectangle hitbox_controller ={450,90,150,70};
    Rectangle hitbox_controller_marked={440,80,170,90};

    Rectangle hitbox_soundless ={50, 180,150,70};
    Rectangle hitbox_soundless_marked ={40, 170,170,90};

    Rectangle hitbox_soundmore ={450,180, 150,70};
    Rectangle hitbox_soundmore_marked ={440, 170,170,90};

    Rectangle hitbox_guided ={50,360,150,70};
    Rectangle hitbox_guided_marked ={40,350,170,90};

    Rectangle hitbox_exploration ={450,360,150,70};
    Rectangle hitbox_exploration_marked ={440,350,170,90};



    //loads all the necessary textures

    Texture2D flagDE = assestmanagergraphics::getTexture("userinterface/flag_DE_5");
    Texture2D flagENG = assestmanagergraphics::getTexture("userinterface/flag_ENG_2");
    Texture2D closeButton = assestmanagergraphics::getTexture("userinterface/button_close");
    Texture2D tastaturbutton = assestmanagergraphics::getTexture("userinterface/tastatur");
    Texture2D controllerbutton = assestmanagergraphics::getTexture("userinterface/controller");
    Texture2D soundlessbutton = assestmanagergraphics::getTexture("userinterface/soundless");
    Texture2D soundmorebutton = assestmanagergraphics::getTexture("userinterface/soundmore");

};


#endif //RAYLIBSTARTER_OPTIONEN_H
