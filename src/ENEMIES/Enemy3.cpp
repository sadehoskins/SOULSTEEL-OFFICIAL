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
    enemyType = "Tackle Spider";
    loadAnimations();
}



void Enemy3::draw()
{
    Enemy::draw();  // Draw the base enemy
    // Tackle Spider doesn't have a separate attack animation, so we don't need to draw anything extra
}

void Enemy3::loadAnimations()
{
    std::vector<std::string> actions = {"Idle", "Walk"};
    std::vector<std::string> directions = {"back", "front", "side left", "side right"};

    for (const auto& action : actions)
    {
        for (const auto& direction : directions)
        {
            std::string key = toLowercase(action) + "_" +
                              (direction == "side left" ? "left" :
                               direction == "side right" ? "right" : direction);

            animations[key] = AnimationInfo(
                    8,  // Assuming 8 frames for all animations
                    0.1f,  // Assuming 0.1s duration per frame
                    loadTexture(action, direction)
            );
        }
    }
}

Texture2D Enemy3::getCurrentTexture()
{
    std::string state = (currentAnimationState == AnimationState::IDLE) ? "idle" : "walk";
    std::string direction;
    switch(facingDirection) {
        case Direction::Left: direction = "left"; break;
        case Direction::Right: direction = "right"; break;
        case Direction::Up: direction = "back"; break;
        case Direction::Down: direction = "front"; break;
    }
    std::string key = state + "_" + direction;

    return animations[key].texture;
}

void Enemy3::update()
{
    Enemy::update();
    updateAnimation(GetFrameTime());

    // Update currentAnimationState and facingDirection based on enemy state
    if (isAttacking)
    {
        currentAnimationState = AnimationState::WALK;  // Use walk animation for tackle
    }
    else if (!isAttacking || (currentAnimationState == AnimationState::IDLE))
    {
        currentAnimationState = AnimationState::WALK;
    }
    else
    {
        currentAnimationState = AnimationState::IDLE;
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
     position.x += (facingDirection == Direction::Right ? 5.0f : -5.0f);

    // Check if the tackle attack is complete
    //if (/* tackle attack complete condition */)
    {
        isAttacking = false;
    }
}





Enemy3::~Enemy3() {
}