//
// Created by sadeh on 24.06.2024.
//

#ifndef RAYLIBSTARTER_ENEMY3_H
#define RAYLIBSTARTER_ENEMY3_H
#include "Enemy.h"


class Enemy3: public Enemy {
public:
    Enemy3(gameplay *scene);
    void update() override;
    void draw() override;
    Texture2D getCurrentTexture() override;
    void performTackleAttack();
    void performToothAttack();
    ~Enemy3();

private:
    bool isAttacking;
    void updateTackleAttack();
    bool isNearMainCharacter() const;
    void drawTackleAttack();
    void updateToothAttack();
    void drawToothAttack();
    Vector2 toothPosition;
    float attackAnimationTimer;
    bool isToothFlying;
};



#endif //RAYLIBSTARTER_ENEMY3_H
