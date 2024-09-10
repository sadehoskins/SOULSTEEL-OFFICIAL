//
// Created by lrfri on 26.08.2024.
//

#include "blueblock.h"
const float blueblock::animation_speed = 0.1f;

blueblock::blueblock(gameplay* scene, Vector2 position, bool isSwitch) : movableblocks(scene, position, isSwitch) {
    blueblock_texture = assestmanagergraphics::getTexture("item/blue_block");
    blueblock_turning_on = assestmanagergraphics::getTexture("item/blue_block_turning_on");
    blueblock_turning_off = assestmanagergraphics::getTexture("item/blue_block_turning_off");
    blueswitch_on = assestmanagergraphics::getTexture("item/blue_switch_on");
    blueswitch_off = assestmanagergraphics::getTexture("item/blue_switch_off");
}

void blueblock::draw() {
    /*std::cout << "Drawing " << (isSwitch ? "switch" : "block") << " at (" << position.x << ", " << position.y
              << "), activated: " << isActivated << ", animating: " << isAnimating << std::endl;*/
    if (isSwitch) {
        if (isActivated) {
            // Draw animated switch
            int frameWidth = blueswitch_on.width / frame_count;
            Rectangle sourceRec = {static_cast<float>(currentFrame * frameWidth), 0.0f, static_cast<float>(frameWidth), static_cast<float>(blueswitch_on.height)};
            DrawTextureRec(blueswitch_on, sourceRec, position, WHITE);
        } else {
            // Draw static off switch
            DrawTexture(blueswitch_off, position.x, position.y, WHITE);
        }
    } else {
        // Draw block
        Texture2D currentTexture;
        if (isAnimating) {
            currentTexture = isActivated ? blueblock_turning_on : blueblock_turning_off;
        } else {
            currentTexture = isActivated ? blueblock_turning_on : blueblock_turning_off;
        }
        Rectangle sourceRec = {currentFrame * 32.0f, 0, 32, 32};
        Rectangle destRec = {position.x, position.y, 32, 32};
        DrawTexturePro(currentTexture, sourceRec, destRec, {0, 0}, 0, WHITE);
    }
}

void blueblock::toggle() {
    /*std::cout << "Blue " << (isSwitch ? "switch" : "block") << " toggled, now "
              << (isActivated ? "activated" : "deactivated") << std::endl;*/
    isActivated = !isActivated;
    isAnimating = true;
    currentFrame = 0;
    animationTimer = 0.0f;
}

void blueblock::updateAnimation(float deltaTime) {
    if (isSwitch && isActivated) {
        // Continuous animation for activated switch
        animationTimer += deltaTime;
        if (animationTimer >= animation_speed) {
            animationTimer -= animation_speed;
            currentFrame = (currentFrame + 1) % frame_count;
        }
    } else if (!isSwitch && isAnimating) {
        // One-time animation for block
        animationTimer += deltaTime;
        if (animationTimer >= animation_speed) {
            animationTimer -= animation_speed;
            currentFrame++;
            if (currentFrame >= frame_count) {
                currentFrame = frame_count - 1;
                isAnimating = false;
            }
        }
    }
}

bool blueblock::canWalkThrough() const {
    return !isSwitch && !isActivated;
}