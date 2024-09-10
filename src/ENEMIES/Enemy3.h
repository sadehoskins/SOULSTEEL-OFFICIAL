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
    void updateAnimation(float deltaTime) override;
    ~Enemy3();

private:


    bool isAttacking;

    //void updateAnimation(float deltaTime) override;
    //void drawAnimation() override;
    bool isNearMainCharacter() const;

    void loadAnimations() override;
};


#endif //RAYLIBSTARTER_ENEMY3_H
