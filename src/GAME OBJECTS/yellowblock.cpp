//
// Created by lrfri on 26.08.2024.
//

#include "yellowblock.h"
const float yellowblock::animation_speed = 0.1f;

yellowblock::yellowblock(gameplay* scene, Vector2 position, bool isSwitch) : movableblocks(scene, position, isSwitch) {
    yellowblock_texture = assestmanagergraphics::getTexture("item/yellow_block");
    yellowblock_turning_on = assestmanagergraphics::getTexture("item/yellow_block_turning_on");
    yellowblock_turning_off = assestmanagergraphics::getTexture("item/yellow_block_turning_off");
    yellowswitch_on = assestmanagergraphics::getTexture("item/yellow_switch_on");
    yellowswitch_off = assestmanagergraphics::getTexture("item/yellow_switch_off");
}

void yellowblock::draw() {
    /*std::cout << "Drawing " << (isSwitch ? "switch" : "block") << " at (" << position.x << ", " << position.y
              << "), activated: " << isActivated << ", animating: " << isAnimating << std::endl;*/
    if (isSwitch) {
        if (isActivated) {
            // Draw animated switch
            int frameWidth = yellowswitch_on.width / frame_count;
            Rectangle sourceRec = {static_cast<float>(currentFrame * frameWidth), 0.0f, static_cast<float>(frameWidth), static_cast<float>(yellowswitch_on.height)};
            DrawTextureRec(yellowswitch_on, sourceRec, position, WHITE);
        } else {
            // Draw static off switch
            DrawTexture(yellowswitch_off, position.x, position.y, WHITE);
        }
    } else {
        // Draw block
        Texture2D currentTexture;
        if (isAnimating) {
            currentTexture = isActivated ? yellowblock_turning_on : yellowblock_turning_off;
        } else {
            currentTexture = isActivated ? yellowblock_turning_on : yellowblock_turning_off;
        }
        Rectangle sourceRec = {currentFrame * 32.0f, 0, 32, 32};
        Rectangle destRec = {position.x, position.y, 32, 32};
        DrawTexturePro(currentTexture, sourceRec, destRec, {0, 0}, 0, WHITE);
    }
}

void yellowblock::toggle() {
    /*std::cout << "Yellow " << (isSwitch ? "switch" : "block") << " toggled, now "
              << (isActivated ? "activated" : "deactivated") << std::endl;*/
    isActivated = !isActivated;
    isAnimating = true;
    currentFrame = 0;
    animationTimer = 0.0f;
}

void yellowblock::updateAnimation(float deltaTime) {
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

bool yellowblock::canWalkThrough() const {
    return !isSwitch && !isActivated;
}