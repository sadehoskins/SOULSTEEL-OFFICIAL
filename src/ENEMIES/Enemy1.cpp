//
// Created by sadeh on 13.06.2024.
//

#include "Enemy1.h"
#include <iostream>
#include <regex>
#include <algorithm>
#include "../assestmanagergraphics.h"


Enemy1::Enemy1(gameplay *scene)
        : Enemy(scene, 1, 1, true, false, false, 12 * 32 + 16, 10 * 32 - 16, 11 * 32 - 16, 6 * 32 + 16)
{
    enemyType = "enemy1";
    animData.entityType = "teddy";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;

    isThrowing = false;
    bombAnimationTimer = 0.0f;
    bombPosition = {0, 0};
}

void Enemy1::draw() {
    Enemy::draw();  // This will draw the main body

    //*NEW CODE*
    drawHealthStatus();  // Draw the health status

    if (isThrowing)
    {
        // Draw bomb throw animation
        Texture2D bombTexture = assestmanagergraphics::getAnimationTexture(enemyType, AnimationState::BOMB_EFFECT, animData.facingDirection);

        Rectangle sourceRec = {
                static_cast<float>(animData.currentFrame * bombTexture.width / AnimationData::FRAME_COUNT), 0.0f,
                static_cast<float>(bombTexture.width / AnimationData::FRAME_COUNT),
                static_cast<float>(bombTexture.height)
        };
        Vector2 drawPos = {
                position.x - static_cast<float>(bombTexture.width) / (2.0f * AnimationData::FRAME_COUNT),
                position.y - static_cast<float>(bombTexture.height) / 2.0f
        };
        DrawTextureRec(bombTexture, sourceRec, drawPos, WHITE);
    }
}

Texture2D Enemy1::getCurrentTexture()
{
    return assestmanagergraphics::getAnimationTexture(animData.entityType, animData.currentAnimationState, animData.facingDirection);
}


void Enemy1::throwBomb() {
    isThrowing = true;
    animData.currentFrame = 0;
    bombAnimationTimer = 0.0f;
    bombPosition = {position.x + (animData.facingDirection == Direction::Right ? 20.0f : -20.0f), position.y};
}

void Enemy1::update() {
    Enemy::update();
    updateAnimation(GetFrameTime());

    if (isThrowing) {
        animData.currentAnimationState = AnimationState::ATTACK_NORMAL;
    } else if (!isThrowing) {
        animData.currentAnimationState = AnimationState::WALK;
    } else {
        animData.currentAnimationState = AnimationState::IDLE;
    }

    updateBombThrow();
}

void Enemy1::updateBombThrow()
{
    if (!isThrowing) return;

    bombAnimationTimer += GetFrameTime();
    if (bombAnimationTimer >= FRAME_DURATION)
    {
        bombAnimationTimer -= FRAME_DURATION;
        animData.currentFrame++;
        if (animData.currentFrame >= AnimationData::FRAME_COUNT)
        {
            isThrowing = false;
            animData.currentFrame = 0;
            // Here you would typically create an actual bomb object
        }
    }
}

Enemy1::~Enemy1() {
}
