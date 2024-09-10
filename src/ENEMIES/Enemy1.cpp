//
// Created by sadeh on 13.06.2024.
//

#include "Enemy1.h"
#include <iostream>
#include <regex>
#include <algorithm>


/*std::string Enemy1::toLowerCase(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}*/

Enemy1::Enemy1(gameplay *scene)
        : Enemy(scene, 1, 1, true, false, false, //initialize health
                12 * 32 + 16, 10 * 32 - 16, 11 * 32 - 16, 6 * 32 + 16),
          isThrowing(false)
{
    enemyType = "Teddy";
    loadAnimations();
}

void Enemy1::draw() {
    Enemy::draw();

    if (isThrowing)
    {
        // Draw bomb throw animation
        std::string bombKey = "bomb_throw_" + std::string(facingDirection == Direction::Left ? "left" : "right");
        const AnimationInfo& bombAnim = animations[bombKey];

        Rectangle sourceRec = {
                static_cast<float>(currentFrame * bombAnim.texture.width / bombAnim.frameCount), 0.0f,
                static_cast<float>(bombAnim.texture.width / bombAnim.frameCount),
                static_cast<float>(bombAnim.texture.height)
        };
        Vector2 position = {
                this->position.x - static_cast<float>(bombAnim.texture.width) / (2.0f * bombAnim.frameCount),
                this->position.y - static_cast<float>(bombAnim.texture.height) / 2.0f
        };
        DrawTextureRec(bombAnim.texture, sourceRec, position, WHITE);
    }
}

void Enemy1::drawBombThrow() {
    if (!isThrowing || (facingDirection != Direction::Left && facingDirection != Direction::Right)) {
        return;
    }

    std::string bombKey = "teddy_bomb_throw_side_" + std::string(facingDirection == Direction::Left ? "left" : "right") + "_bomb";
    const AnimationInfo& bombAnim = animations[bombKey];
    Rectangle sourceRec = {
            static_cast<float>(currentFrame * bombAnim.texture.width / bombAnim.frameCount), 0.0f,
            static_cast<float>(bombAnim.texture.width / bombAnim.frameCount),
            static_cast<float>(bombAnim.texture.height)
    };
    Vector2 position = {
            bombPosition.x - static_cast<float>(bombAnim.texture.width) / (2.0f * bombAnim.frameCount),
            bombPosition.y - static_cast<float>(bombAnim.texture.height) / 2.0f
    };
    DrawTextureRec(bombAnim.texture, sourceRec, position, WHITE);
}

Texture2D Enemy1::getCurrentTexture()
{
    std::string state;
    switch (currentAnimationState) {
        case AnimationState::IDLE: state = "idle"; break;
        case AnimationState::WALK: state = "walk"; break;
        case AnimationState::ATTACK: state = "bomb_throw"; break;
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

    if (isThrowing && (facingDirection == Direction::Left || facingDirection == Direction::Right))
    {
        key += "_body";
    }

    return animations[key].texture;
}

void Enemy1::loadAnimations()
{
    std::vector<std::string> actions = {"Idle", "Walk", "Bomb throw"};
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

            if (action == "Bomb throw" && (direction == "side left" || direction == "side right"))
            {
                animations[key + "_body"] = AnimationInfo(
                        8,
                        0.1f,
                        loadTexture(action + " - body", direction)
                );
                animations[key + "_bomb"] = AnimationInfo(
                        8,
                        0.1f,
                        assestmanagergraphics::getTexture("Character - Enemy - Teddy - Bomb throw " + direction + " - bomb - animated")
                );
            }
        }
    }
}

void Enemy1::throwBomb() {
    isThrowing = true;
    currentFrame = 0;
    bombAnimationTimer = 0.0f;
    // Set the bomb's initial position relative to the enemy
    bombPosition = {position.x + (facingDirection == Direction::Right ? 20.0f : -20.0f), position.y};
}

void Enemy1::update() {
    Enemy::update();
    updateAnimation(GetFrameTime());
// Update currentAnimationState and facingDirection based on enemy state
    if (isThrowing) {
        currentAnimationState = AnimationState::ATTACK;
    } else if (!isThrowing) {
        currentAnimationState = AnimationState::WALK;
    } else {
        currentAnimationState = AnimationState::IDLE;
    }

    // Update facingDirection based on movement or target direction


    //updateBombThrow();

}

void Enemy1::updateBombThrow()
{
    if (!isThrowing) return;

    bombAnimationTimer += GetFrameTime();
    if (bombAnimationTimer >= animations["bomb_throw_left"].frameDuration)
    {
        bombAnimationTimer -= animations["bomb_throw_left"].frameDuration;
        currentFrame++;
        if (currentFrame >= animations["bomb_throw_left"].frameCount)
        {
            isThrowing = false;
            currentFrame = 0;
            // Here you would typically create an actual bomb object
        }
    }

    // Update bomb position if needed
    // bombPosition.x += (facingDirection == Direction::Right ? 2.0f : -2.0f);
}




Enemy1::~Enemy1() {
}
