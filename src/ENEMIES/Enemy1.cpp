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
    m_enemyType = "Teddy";
    loadAnimations();
}

void Enemy1::draw() {
    Enemy::draw();

    if (isThrowing) {
        // Draw bomb
        std::string bombKey = "enemy1_bomb_throw_" + std::to_string(static_cast<int>(facingDirection));
        Texture2D bombTexture = assestmanagergraphics::getTexture(bombKey);
        // ... draw bomb logic ...
    }
}

void Enemy1::drawBombThrow() {
    if (!isThrowing || (facing != Direction::Left && facing != Direction::Right)) {
        return;
    }

    std::string bombKey = "teddy_bomb_throw_side_" + std::string(facing == Direction::Left ? "left" : "right") + "_bomb";
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

Texture2D Enemy1::getCurrentTexture() {
    std::string state;
    switch (currentAnimationState) {
        case AnimationState::IDLE: state = "Idle"; break;
        case AnimationState::WALK: state = "Walk"; break;
        case AnimationState::ATTACK: state = "Bomb throw"; break;
        default: state = "Idle";
    }

    std::string direction;
    switch (facingDirection) {
        case Direction::Up: direction = "back"; break;
        case Direction::Down: direction = "front"; break;
        case Direction::Left: direction = "side left"; break;
        case Direction::Right: direction = "side right"; break;
    }

    std::string key = "Character - Enemy - " + m_enemyType + " - " + state + " " + direction;

    if (state == "Bomb throw" && (direction == "side left" || direction == "side right")) {
        key += " - body";
    }

    return assestmanagergraphics::getCharacterTexture("enemies", key + " - animated");
}

void Enemy1::loadAnimations() {
    std::vector<std::string> actions = {"Idle", "Walk", "Bomb throw"};
    std::vector<std::string> directions = {"back", "front", "side left", "side right"};

    for (const auto& action : actions) {
        for (const auto& direction : directions) {
            std::string key = "Character - Enemy - " + m_enemyType + " - " + action + " " + direction;

            animations[key] = AnimationInfo(
                    FRAME_COUNT,
                    FRAME_DURATION,
                    assestmanagergraphics::getCharacterTexture("enemies", key + " - animated")
            );

            if (action == "Bomb throw" && (direction == "side left" || direction == "side right")) {
                animations[key + " - body"] = AnimationInfo(
                        FRAME_COUNT,
                        FRAME_DURATION,
                        assestmanagergraphics::getCharacterTexture("enemies", key + " - body - animated")
                );
                animations[key + " - bomb"] = AnimationInfo(
                        FRAME_COUNT,
                        FRAME_DURATION,
                        assestmanagergraphics::getCharacterTexture("enemies", key + " - bomb - animated")
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
    bombPosition = {position.x + (facing == Direction::Right ? 20.0f : -20.0f), position.y};
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


    //updateAnimation();
    //updateBombThrow();


    // Add logic to decide when to throw a bomb
    //for example when enemy is near character
}

void Enemy1::updateBombThrow() {
    if (!isThrowing) return;

    bombAnimationTimer += GetFrameTime();
    if (bombAnimationTimer >= FRAME_DURATION) {
        bombAnimationTimer -= FRAME_DURATION;
        currentFrame++;
        if (currentFrame >= FRAME_COUNT) {
            isThrowing = false;
            currentFrame = 0;
            // Here you would typically create an actual bomb object
        }
    }

    // Update bomb position if needed
    // bombPosition.x += (facing == Direction::Right ? 2.0f : -2.0f);
}

Enemy1::~Enemy1() {
}
