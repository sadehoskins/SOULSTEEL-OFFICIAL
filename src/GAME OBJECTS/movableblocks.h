//
// Created by lrfri on 26.08.2024.
//

#ifndef RAYLIBSTARTER_MOVABLEBLOCKS_H
#define RAYLIBSTARTER_MOVABLEBLOCKS_H

#include "gameobjects.h"

class movableblocks : public gameobjects{
public:
    movableblocks(gameplay* scene, Vector2 position, bool isSwitch = false);
    virtual void update() override;
    virtual void draw() override;
    virtual void toggle() = 0;
    virtual bool canWalkThrough() const = 0;

protected:
    bool isActivated;
    float animationTimer;
    int currentFrame;
    bool isAnimating;
    bool isSwitch;

    virtual void updateAnimation(float deltaTime) = 0;
};

#endif //RAYLIBSTARTER_MOVABLEBLOCKS_H
