//
// Created by sadeh on 13.06.2024.
//

#include "Enemy1.h"
#include <iostream>
#include <regex>
#include <algorithm>
#include "../assestmanagergraphics.h"


Enemy1::Enemy1(gameplay *scene)
        : Enemy(scene, 1, 1, true, false, false, 12 * 32 + 16, 10 * 32 - 16, 11 * 32 - 16, 6 * 32 + 16),
          isThrowing(false), bombAnimationTimer(0.0f),
          detectionRadius(100.0f), bombCooldown(5.0f), timeSinceLastBomb(0.0f)
{
    enemyType = "teddy";
    animData.entityType = "teddy";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;
}

void Enemy1::draw() {
    Enemy::draw();  // draw main body

    drawHealthStatus();  // Draw the health status

    if (isThrowing){
        drawBombThrow();
    }
}

Texture2D Enemy1::getCurrentTexture()
{
    return assestmanagergraphics::getAnimationTexture(animData.entityType, animData.currentAnimationState, animData.facingDirection);
}


void Enemy1::throwBomb()
{
    isThrowing = true;
    animData.currentFrame = 0;
    bombAnimationTimer = 0.0f;

    maincharacter* player = _scene->themaincharacter;
    Vector2 direction = Vector2Normalize(Vector2Subtract(player->position, position));
    bombPosition = Vector2Add(position, Vector2Scale(direction, 32.0f));

    _scene->addBomb(bombPosition);
    timeSinceLastBomb = 0.0f;
}

void Enemy1::update() {
    Enemy::update();
    updateAnimation(GetFrameTime());
    timeSinceLastBomb += GetFrameTime();

    if (canThrowBomb()) {
        throwBomb();
    }

    animData.currentAnimationState = isThrowing ? AnimationState::ATTACK_NORMAL
                                                : AnimationState::WALK;

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

        }
    }
}

void Enemy1::drawBombThrow() {
    Texture2D bombTexture = assestmanagergraphics::getAnimationTexture(enemyType, AnimationState::BOMB_EFFECT, animData.facingDirection);

    Rectangle sourceRec = {
            static_cast<float>(animData.currentFrame * bombTexture.width / AnimationData::FRAME_COUNT), 0.0f,
            static_cast<float>(bombTexture.width / AnimationData::FRAME_COUNT),
            static_cast<float>(bombTexture.height)
    };
    Vector2 drawPos = {
            bombPosition.x - static_cast<float>(bombTexture.width) / (2.0f * AnimationData::FRAME_COUNT),
            bombPosition.y - static_cast<float>(bombTexture.height) / 2.0f
    };
    DrawTextureRec(bombTexture, sourceRec, drawPos, WHITE);
}

bool Enemy1::canThrowBomb()
{
    if (timeSinceLastBomb < bombCooldown)
        return false;

    maincharacter* player = _scene->themaincharacter;
    if (!player)
        return false;

    float distanceToPlayer = Vector2Distance(position, player->position);
    return distanceToPlayer <= detectionRadius;
}



Enemy1::~Enemy1() {
}
