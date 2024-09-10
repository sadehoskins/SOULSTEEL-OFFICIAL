//
// Created by lrfri on 19.06.2024.
//

#ifndef RAYLIBSTARTER_GAMEOBJECTS_H
#define RAYLIBSTARTER_GAMEOBJECTS_H

#include "raylib.h"
#include "../scene.h"
#include <iostream>

class gameplay;

class gameobjects {
protected:


public:
    float size;
    virtual void update()=0;
    virtual void draw()=0;

    gameplay *_scene;
    gameobjects(){};
    gameobjects(gameplay*scene);
    Vector2 position;
};


#endif //RAYLIBSTARTER_GAMEOBJECTS_H
