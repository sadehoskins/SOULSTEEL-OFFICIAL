//
// Created by sadeh on 13.06.2024.
//

#include <regex>
#include "Enemy2.h"

Enemy2::Enemy2(gameplay *scene)
        : Enemy(scene,2, 2, false, true, false,
                6.0f * 32.0f - 16.0f, 8 * 32 + 16, 8 * 32 - 16, 6 * 32 + 16),
          isAttacking(false){
    enemyType = "Spider";
    loadAnimations();
}

void Enemy2::draw()
{
    Enemy::draw();  // Draw the base enemy

    if (isAttacking)
    {
        drawRangedAttack();
    }
}

void Enemy2::drawRangedAttack()
{
    std::string effectKey = "ranged_effect_" + std::string(facingDirection == Direction::Left ? "left" :
                                                           facingDirection == Direction::Right ? "right" :
                                                           facingDirection == Direction::Up ? "back" : "front");
    const AnimationInfo& effectAnim = animations[effectKey];

    Rectangle sourceRec = {
            static_cast<float>(currentFrame * effectAnim.texture.width / effectAnim.frameCount), 0.0f,
            static_cast<float>(effectAnim.texture.width / effectAnim.frameCount),
            static_cast<float>(effectAnim.texture.height)
    };
    Vector2 position = {
            attackPosition.x - static_cast<float>(effectAnim.texture.width) / (2.0f * effectAnim.frameCount),
            attackPosition.y - static_cast<float>(effectAnim.texture.height) / 2.0f
    };
    DrawTextureRec(effectAnim.texture, sourceRec, position, WHITE);
}



void Enemy2::loadAnimations()
{
    std::vector<std::string> actions = {"Idle", "Walk", "Ranged"};
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

            if (action == "Ranged")
            {
                std::string effectKey = "ranged_effect_" +
                                        (direction == "side left" ? "left" :
                                         direction == "side right" ? "right" : direction);
                animations[effectKey] = AnimationInfo(
                        8,
                        0.1f,
                        assestmanagergraphics::getTexture("Character object - Enemy - Spider - Ranged attack " + direction + " - animated")
                );
            }
        }
    }
}


Texture2D Enemy2::getCurrentTexture()
{
    std::string state;
    switch (currentAnimationState) {
        case AnimationState::IDLE: state = "idle"; break;
        case AnimationState::WALK: state = "walk"; break;
        case AnimationState::ATTACK: state = "ranged"; break;
        default: state = "idle";
    }

    std::string direction;
    switch (facingDirection) {
        case Direction::Up: direction = "back"; break;
        case Direction::Down: direction = "front"; break;
        case Direction::Left: direction = "left"; break;
        case Direction::Right: direction = "right"; break;
    }

    std::string key = state + "_" + direction;
    return animations[key].texture;
}

void Enemy2::update()
{
    Enemy::update();
    updateAnimation(GetFrameTime());

    // Update currentAnimationState and facingDirection based on enemy state
    if (isAttacking)
    {
        currentAnimationState = AnimationState::ATTACK;
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

    updateRangedAttack();
}

void Enemy2::performRangedAttack()
{
    isAttacking = true;
    currentFrame = 0;
    attackAnimationTimer = 0.0f;
    // Set the attack's initial position relative to the enemy
    attackPosition = {position.x + (facingDirection == Direction::Right ? 20.0f : -20.0f), position.y};
}

void Enemy2::updateRangedAttack()
{
    if (!isAttacking) return;

    attackAnimationTimer += GetFrameTime();
    if (attackAnimationTimer >= animations["ranged_effect_left"].frameDuration)
    {
        attackAnimationTimer -= animations["ranged_effect_left"].frameDuration;
        currentFrame++;
        if (currentFrame >= animations["ranged_effect_left"].frameCount)
        {
            isAttacking = false;
            currentFrame = 0;
            // Here you would typically create an actual projectile or apply damage
        }
    }

    // Update attack effect position if needed
    // attackPosition.x += (facingDirection == Direction::Right ? 2.0f : -2.0f);
}




Enemy2::~Enemy2() {
}
