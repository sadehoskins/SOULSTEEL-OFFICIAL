//
// Created by sadeh on 13.06.2024.
//

#ifndef RAYLIBSTARTER_ENEMY1_H
#define RAYLIBSTARTER_ENEMY1_H
#include "Enemy.h"


class Enemy1: public Enemy{
public:
    Enemy1(gameplay *scene);
    void update() override;
    void draw() override;
    void throwBomb();
    Texture2D getCurrentTexture() override;
    ~Enemy1() override;


private:
    bool isThrowing;
    Vector2 bombPosition;
    float bombAnimationTimer;
    void updateBombThrow();
    void drawBombThrow();

    //bomb throwing
    float detectionRadius;
    float bombCooldown;
    float timeSinceLastBomb;
    bool canThrowBomb();





};


#endif //RAYLIBSTARTER_ENEMY1_H
