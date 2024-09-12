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
    animData.entityType = "Tackle Spider";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;
}


void Enemy3::draw()
{
    Enemy::draw();  // Draw the base enemy
    // Tackle Spider doesn't have a separate attack animation, so we don't need to draw anything extra

    //*NEW CODE*
    drawHealthStatus();  // Draw the health status
}

Texture2D Enemy3::getCurrentTexture()
{
    return assestmanagergraphics::getAnimationTexture(animData.entityType, animData.currentAnimationState, animData.facingDirection);
}

void Enemy3::update()
{
    Enemy::update();
    updateAnimation(GetFrameTime());

    // Update currentAnimationState and facingDirection based on enemy state
    if (isAttacking)
    {
        animData.currentAnimationState = AnimationState::WALK;  // Use walk animation for tackle
    }
    else if (!isAttacking || (animData.currentAnimationState == AnimationState::IDLE))
    {
        animData.currentAnimationState = AnimationState::WALK;
    }
    else
    {
        animData.currentAnimationState = AnimationState::IDLE;
    }

    // Update facingDirection based on movement or target direction

    updateTackleAttack();
}

void Enemy3::performTackleAttack()
{
    isAttacking = true;
    // Set up any necessary variables for the tackle attack
}

void Enemy3::updateTackleAttack()
{
    if (!isAttacking) return;

    // Implement tackle attack logic here
    // This might involve moving the enemy quickly in a certain direction
    // and checking for collisions with the player

    // Example:
    position.x += (animData.facingDirection == Direction::Right ? 5.0f : -5.0f);

    // Check if the tackle attack is complete
    //if (/* tackle attack complete condition */)
    {
        isAttacking = false;
    }
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