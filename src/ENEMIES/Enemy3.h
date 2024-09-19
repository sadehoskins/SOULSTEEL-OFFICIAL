//
// Created by sadeh on 24.06.2024.
//

#ifndef RAYLIBSTARTER_ENEMY3_H
#define RAYLIBSTARTER_ENEMY3_H
#include "Enemy.h"


class Enemy3: public Enemy {
public:
    //Enemy3(gameplay *scene);
    Enemy3(gameplay *scene, const Vector2& initialPosition);
    void update() override;
    void draw() override;
    Texture2D getCurrentTexture() override;

    //*NEW CODE*
    int spiderToothFrame;
    float spiderToothFrameTimer;

    static const int SPIDERTOOTH_FRAME_COUNT = 8;
    static const float SPIDERTOOTH_FRAME_DURATION;


    ~Enemy3();

private:
    bool isShooting;
    Vector2 spiderToothPosition;
    Vector2 spiderToothDirection;
    float shootingRange;
    float spiderToothSpeed;
    float shootCooldown;
    float shootTimer;

    void shootSpiderTooth();
    void updateSpiderToothAttack();
    void drawSpiderToothAttack();



};



#endif //RAYLIBSTARTER_ENEMY3_H
