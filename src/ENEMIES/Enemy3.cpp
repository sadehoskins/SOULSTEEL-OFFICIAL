//
// Created by sadeh on 24.06.2024.
//

#include <regex>
#include "Enemy3.h"




Enemy3::Enemy3(gameplay *scene)
        : Enemy(scene,2, 2, false, true, false,
                6.0f * 32.0f - 16.0f, 8 * 32 + 16, 8 * 32 - 16, 6 * 32 + 16){
    loadAnimations();
}

void Enemy3::update() {
    Enemy::update();
    updateAnimation(GetFrameTime());

}

void Enemy3::draw() {
    Texture2D texture = getCurrentTexture();
    Rectangle sourceRec = {
            static_cast<float>(currentFrame * texture.width / FRAME_COUNT), 0.0f,
            static_cast<float>(texture.width / FRAME_COUNT),
            static_cast<float>(texture.height)
    };
    Vector2 drawPosition = {
            position.x - static_cast<float>(texture.width) / (2.0f * FRAME_COUNT),
            position.y - static_cast<float>(texture.height) / 2.0f
    };
    DrawTextureRec(texture, sourceRec, drawPosition, WHITE);



}

void Enemy3::loadAnimations() {
    std::vector<std::string> actions = {"Idle", "Walk"};
    std::vector<std::string> directions = {"back", "front", "side left", "side right"};

    for (const auto& action : actions) {
        for (const auto& direction : directions) {
            std::string key = "tackle_spider_" + toLowercase(action) + "_" +
                              (direction == "side left" ? "left" :
                               direction == "side right" ? "right" : direction);
            key = std::regex_replace(key, std::regex("\\s+"), "_");
            std::string fileName = "Character - Enemy - Tackle Spider - " + action + " " + direction;

            animations[key] = AnimationInfo(
                    FRAME_COUNT,
                    FRAME_DURATION,
                    assestmanagergraphics::getCharacterTexture("enemies", fileName + " - animated")
            );
        }
    }
}

Texture2D Enemy3::getCurrentTexture() {
    std::string action = (currentAnimationState == AnimationState::IDLE) ? "idle" : "walk";
    std::string direction;
    switch(facing) {
        case Direction::Left: direction = "left"; break;
        case Direction::Right: direction = "right"; break;
        case Direction::Up: direction = "back"; break;
        case Direction::Down: direction = "front"; break;
    }
    std::string key = "tackle_spider_" + action + "_" + direction;

    if (animations.find(key) != animations.end()) {
        return animations[key].texture;
    }
    return animations["tackle_spider_idle_front"].texture; // Default texture
}



Enemy3::~Enemy3() {
}

void Enemy3::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= FRAME_DURATION) {
        currentFrame = (currentFrame + 1) % FRAME_COUNT;
        animationTimer -= FRAME_DURATION;
    }
}
