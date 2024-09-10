//
// Created by sadeh on 11.07.2024.
//

#ifndef RAYLIBSTARTER_UTILS_H
#define RAYLIBSTARTER_UTILS_H

#include "Wall.h"
#include "ENEMIES/Enemy.h"
#include "maincharacter.h"


    class maincharacter;
    class Enemy;
    class Wall;

namespace Collision{

    // Collision detection functions
    bool checkCollision(const maincharacter& maincharacter, const Enemy& enemy);
    bool checkCollision(const maincharacter& maincharacter, const Wall& wall);
    bool checkCollision(const Enemy& enemy, const Wall& wall);


};


#endif //RAYLIBSTARTER_UTILS_H
