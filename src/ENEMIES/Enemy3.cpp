//
// Created by sadeh on 24.06.2024.
//

#include <regex>
#include "Enemy3.h"




Enemy3::Enemy3(gameplay* scene)
        : Enemy(scene, 2, 2, false, true, false,
                6.0f * 32.0f - 16.0f, 8 * 32 + 16, 8 * 32 - 16, 6 * 32 + 16),
          isAttacking(false)
{
    enemyType = "tacklespider";
    animData.entityType = "tacklespider";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;

    isAttacking = false;
    attackAnimationTimer = 0.0f;
    toothPosition = {0, 0};
    isToothFlying = false;
}


void Enemy3::draw()
{
    Enemy::draw();  // Draw the base enemy
    // Tackle Spider doesn't have a separate attack animation, so we don't need to draw anything extra

    //*NEW CODE*
    //drawHealthStatus();  // Draw the health status
    if (isAttacking) {
        drawTackleAttack();
    }
    if (isToothFlying) {
        drawToothAttack();
    }
}

void Enemy3::drawTackleAttack() {
    // The tackle attack uses the walk animation, so we don't need to draw anything extra here
}

Texture2D Enemy3::getCurrentTexture()
{
    return assestmanagergraphics::getAnimationTexture(animData.entityType, animData.currentAnimationState, animData.facingDirection);
}

void Enemy3::update() {
    Enemy::update();

    if (isAttacking) {
        animData.currentAnimationState = AnimationState::WALK;  // Use walk animation for tackle
    } else if (stopdown || stopleft || stopright || stopup) {
        animData.currentAnimationState = AnimationState::WALK;
    } else {
        animData.currentAnimationState = AnimationState::IDLE;
    }

    std::cout << "Enemy3 update: state = " << static_cast<int>(animData.currentAnimationState)
              << ", direction = " << static_cast<int>(animData.facingDirection) << std::endl;

    updateTackleAttack();
    updateToothAttack();
}

void Enemy3::performTackleAttack() {
    isAttacking = true;
    animData.currentFrame = 0;
    attackAnimationTimer = 0.0f;
}

void Enemy3::performToothAttack() {
    isToothFlying = true;
    animData.currentFrame = 0;
    attackAnimationTimer = 0.0f;
    toothPosition = position;
}

void Enemy3::updateTackleAttack() {
    if (!isAttacking) return;

    attackAnimationTimer += GetFrameTime();
    if (attackAnimationTimer >= FRAME_DURATION) {
        attackAnimationTimer -= FRAME_DURATION;
        animData.currentFrame++;
        if (animData.currentFrame >= AnimationData::FRAME_COUNT) {
            isAttacking = false;
            animData.currentFrame = 0;
        }
    }

    // Implement tackle attack logic here
    // This might involve moving the enemy quickly in a certain direction
    // and checking for collisions with the player
    position.x += (animData.facingDirection == Direction::Right ? 5.0f : -5.0f);
}

void Enemy3::updateToothAttack() {
    if (!isToothFlying) return;

    attackAnimationTimer += GetFrameTime();
    if (attackAnimationTimer >= FRAME_DURATION) {
        attackAnimationTimer -= FRAME_DURATION;
        animData.currentFrame++;
        if (animData.currentFrame >= AnimationData::FRAME_COUNT) {
            animData.currentFrame = 0;
        }
    }

    // Update tooth position
    float toothSpeed = 5.0f;
    switch (animData.facingDirection) {
        case Direction::Up:
            toothPosition.y -= toothSpeed;
            break;
        case Direction::Down:
            toothPosition.y += toothSpeed;
            break;
        case Direction::Left:
            toothPosition.x -= toothSpeed;
            break;
        case Direction::Right:
            toothPosition.x += toothSpeed;
            break;
    }

    // Check if tooth is out of bounds and deactivate if necessary
    // This is a simple example, you might want to adjust based on your game's needs
    if (toothPosition.x < 0 || toothPosition.x > GetScreenWidth() ||
        toothPosition.y < 0 || toothPosition.y > GetScreenHeight()) {
        isToothFlying = false;
    }
}

void Enemy3::drawToothAttack() {
    Texture2D toothTexture = assestmanagergraphics::getAnimationTexture(enemyType, AnimationState::SPIDERTOOTH, animData.facingDirection);

    Rectangle sourceRec = {
            static_cast<float>(animData.currentFrame * toothTexture.width / AnimationData::FRAME_COUNT), 0.0f,
            static_cast<float>(toothTexture.width / AnimationData::FRAME_COUNT),
            static_cast<float>(toothTexture.height)
    };
    Vector2 drawPos = {
            toothPosition.x - static_cast<float>(toothTexture.width) / (2.0f * AnimationData::FRAME_COUNT),
            toothPosition.y - static_cast<float>(toothTexture.height) / 2.0f
    };
    DrawTextureRec(toothTexture, sourceRec, drawPos, WHITE);
}

bool Enemy3::isNearMainCharacter() const
{
    // Implement logic to check if the main character is near
    // This could involve checking the distance between the enemy and the main character
    // Return true if the main character is within attack range
    return false; // Placeholder return
}

Enemy3::~Enemy3() {
    // Destructor implementation (if needed)
}