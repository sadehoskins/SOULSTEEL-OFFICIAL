//
// Created by lrfri on 26.08.2024.
//

#include "movableblocks.h"
movableblocks::movableblocks(gameplay* scene, Vector2 position, bool isSwitch) : gameobjects(scene) {
    this->position = position;
    this->isSwitch = isSwitch;
    isActivated = true;
    animationTimer = 0.0f;
    currentFrame = 0;
    isAnimating = true;
}

void movableblocks::update() {
    if (isAnimating) {
        updateAnimation(GetFrameTime());
    }
}

void movableblocks::draw() {
    // This will be implemented in the child classes
}