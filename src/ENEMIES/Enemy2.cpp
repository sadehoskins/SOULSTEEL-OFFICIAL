//
// Created by sadeh on 13.06.2024.
//

#include <regex>
#include "Enemy2.h"
#include "../assestmanagergraphics.h"

Enemy2::Enemy2(gameplay *scene)
        : Enemy(scene, 2, 2, false, true, false,
                6.0f * 32.0f - 16.0f, 8 * 32 + 16, 8 * 32 - 16, 6 * 32 + 16),
          isAttacking(false)
{
    enemyType = "spider";
    animData.entityType = "spider";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;

    isAttacking = false;
    attackAnimationTimer = 0.0f;
    attackPosition = {0, 0};
}

void Enemy2::draw()
{
    Enemy::draw();  // Draw the base enemy
    //*NEW CODE*
    //drawHealthStatus();  // Draw the health status

    if (isAttacking)
    {
        drawRangedAttack();
    }
}

void Enemy2::drawRangedAttack() {
    std::string effectKey = "ranged_effect_" + std::string(animData.facingDirection == Direction::Left ? "left" :
                                                           animData.facingDirection == Direction::Right ? "right" :
                                                           animData.facingDirection == Direction::Up ? "back" : "front");
    Texture2D effectTexture = assestmanagergraphics::getAnimationTexture(enemyType, AnimationState::ATTACK_RANGED, animData.facingDirection);

    Rectangle sourceRec = {
            static_cast<float>(animData.currentFrame * effectTexture.width / AnimationData::FRAME_COUNT), 0.0f,
            static_cast<float>(effectTexture.width / AnimationData::FRAME_COUNT),
            static_cast<float>(effectTexture.height)
    };
    Vector2 drawPos = {
            attackPosition.x - static_cast<float>(effectTexture.width) / (2.0f * AnimationData::FRAME_COUNT),
            attackPosition.y - static_cast<float>(effectTexture.height) / 2.0f
    };
    DrawTextureRec(effectTexture, sourceRec, drawPos, WHITE);
}

Texture2D Enemy2::getCurrentTexture()
{
    return assestmanagergraphics::getAnimationTexture(animData.entityType, animData.currentAnimationState, animData.facingDirection);
}

void Enemy2::update() {
    Enemy::update();

    if (isAttacking) {
        animData.currentAnimationState = AnimationState::ATTACK_RANGED;
    } else if (stopdown || stopleft || stopright || stopup) {
        animData.currentAnimationState = AnimationState::WALK;
    } else {
        animData.currentAnimationState = AnimationState::IDLE;
    }

    std::cout << "Enemy2 update: state = " << static_cast<int>(animData.currentAnimationState)
              << ", direction = " << static_cast<int>(animData.facingDirection) << std::endl;

    updateRangedAttack();
}

void Enemy2::performRangedAttack()
{
    isAttacking = true;
    animData.currentFrame = 0;
    attackAnimationTimer = 0.0f;
    // Set the attack's initial position relative to the enemy
    attackPosition = {position.x + (animData.facingDirection == Direction::Right ? 20.0f : -20.0f), position.y};
}

void Enemy2::updateRangedAttack()
{
    if (!isAttacking) return;

    attackAnimationTimer += GetFrameTime();
    if (attackAnimationTimer >= FRAME_DURATION)
    {
        attackAnimationTimer -= FRAME_DURATION;
        animData.currentFrame++;
        if (animData.currentFrame >= AnimationData::FRAME_COUNT)
        {
            isAttacking = false;
            animData.currentFrame = 0;
            // Here you would typically create an actual projectile or apply damage
        }
    }

    // Update attack effect position if needed
    // attackPosition.x += (animData.facingDirection == Direction::Right ? 2.0f : -2.0f);
}

Enemy2::~Enemy2() {
    // Destructor implementation (if needed)
}