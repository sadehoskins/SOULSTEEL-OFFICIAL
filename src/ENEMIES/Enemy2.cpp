//
// Created by sadeh on 13.06.2024.
//

#include <regex>
#include "Enemy2.h"

Enemy2::Enemy2(gameplay *scene)
        : Enemy(scene,2, 2, false, true, false,
                6.0f * 32.0f - 16.0f, 8 * 32 + 16, 8 * 32 - 16, 6 * 32 + 16),
          isAttacking(false){
    loadAnimations();
}

void Enemy2::draw() {
    Enemy::draw();

    if (isAttacking) {
        // Draw bomb
        std::string bombKey = "enemy2_attack_range_" + std::to_string(static_cast<int>(facingDirection));
        Texture2D rangeAttackTexture = assestmanagergraphics::getTexture(bombKey);
        // ... draw bomb logic ...
    }
}

void Enemy2::update() {
    Enemy::update();
    updateAnimation(GetFrameTime());
}

void Enemy2::loadAnimations() {
    std::vector<std::string> actions = {"Idle", "Walk", "Ranged"};
    std::vector<std::string> directions = {"back", "front", "side left", "side right"};

    for (const auto& action : actions) {
        for (const auto& direction : directions) {
            std::string key = "spider_" + toLowercase(action) + "_" +
                              (direction == "side left" ? "left" :
                               direction == "side right" ? "right" : direction);
            key = std::regex_replace(key, std::regex("\\s+"), "_");
            std::string fileName = "Character - Enemy - Spider - " + action + " " + direction;

            //*NEW CODE*
            animations[key] = AnimationInfo(
                    FRAME_COUNT,
                    FRAME_DURATION,
                    assestmanagergraphics::getCharacterTexture("enemies", fileName + " - animated")
            );
        }
    }
}


Texture2D Enemy2::getCurrentTexture() {
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
    if (isAttacking && (facingDirection == Direction::Left || facingDirection == Direction::Right)) {
        key += "_ranged";
    }

    auto it = animations.find(key);
    if (it != animations.end()) {
        return it->second.texture;
    }

    // Return a default texture if the animation is not found
    return animations["idle_front"].texture;
}

Enemy2::~Enemy2() {
}
