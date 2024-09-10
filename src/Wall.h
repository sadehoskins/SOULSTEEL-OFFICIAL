//
// Created by sadeh on 11.07.2024.
//

#ifndef RAYLIBSTARTER_WALL_H
#define RAYLIBSTARTER_WALL_H


#include "raylib.h"

class Wall {

public:
    Wall(float x, float y, float width, float height);
    virtual ~Wall();

    Rectangle getCollisionRectangle() const;
private:
    float x;
    float y;
    float width;
    float height;
};


#endif //RAYLIBSTARTER_WALL_H
