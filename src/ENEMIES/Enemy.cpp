//
// Created by lrfri on 04.06.2024.
//

#include <algorithm>
#include "Enemy.h"
#include "../gameplay.h"
#include "Enemy1.h"
#include "raymath.h"
#include "../Wall.h"



Enemy::Enemy(gameplay* scene, int hp, int damage, bool melee, bool ranged, bool armed,
             float left, float down, float right, float up)
        : _scene(scene), healthManager(hp), enemyDamage(damage),  currentAnimationState(AnimationState::IDLE),
          facingDirection(Direction::Down), animationTimer(0.0f), currentFrame(0)
{
    //this->health = hp; //initialization of health.
}

void Enemy::calculatePathAsRectangle() {
    path.clear();
    path.push_back({stopleft, stopup});
    path.push_back({stopright, stopup});
    path.push_back({stopright, stopdown});
    path.push_back({stopleft, stopdown});
}

bool Enemy::checkCollision(const Wall &wall) { //Oli-> looks prettier than in Utils
    return Collision::checkCollision(*this, wall);
}

Rectangle Enemy::getCollisionRectangle() const{
    return Rectangle{position.x-8,position.y-8,16,16};
}

float Enemy::getHealthPercentage() const {
    return healthManager.getHealthPercentage();
}

void Enemy::heal(int amount) {
    healthManager.heal(amount);
}

bool Enemy::isAlive() const {
    return healthManager.isAlive();
}

void Enemy::moveOnPath() {

    if (currentPathIndex >= path.size()) {
        currentPathIndex = 0;
    }
    Vector2 Delta{};
    Vector2 target = path[currentPathIndex];
    Delta = Vector2Subtract(target, position);

    if (Vector2Length(Delta) > stepsize) {
        Delta = Vector2Scale(Vector2Normalize(Delta), stepsize);
    }
    position = Vector2Add(position, Delta);

    // if statement for checking if the enemy reached current target
    if (Vector2Distance(position, target) <= stepsize * 2) {
        currentPathIndex++;
    }
}

void Enemy::moveRandomly() {
}

void Enemy::takeDamage(int amount) {
    healthManager.takeDamage(amount);
}

std::string Enemy::toLowercase(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

















void Enemy::attack(maincharacter* target) {
    target->health -= maincharacter::attackPower;
}

void Enemy::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= FRAME_DURATION) {
        currentFrame = (currentFrame + 1) % FRAME_COUNT;
        animationTimer -= FRAME_DURATION;
    }
}

void Enemy::draw() {
    Texture2D currentTexture = getCurrentTexture();
    Rectangle sourceRec = {
            static_cast<float>(currentFrame * currentTexture.width / FRAME_COUNT), 0.0f,
            static_cast<float>(currentTexture.width / FRAME_COUNT),
            static_cast<float>(currentTexture.height)
    };
    Rectangle destRec = {
            position.x - static_cast<float>(currentTexture.width) / (2.0f * FRAME_COUNT),
            position.y - static_cast<float>(currentTexture.height) / 2.0f,
            static_cast<float>(currentTexture.width) / FRAME_COUNT,
            static_cast<float>(currentTexture.height)
    };
    DrawTexturePro(currentTexture, sourceRec, destRec, {0, 0}, 0, WHITE);
}

void Enemy::setAnimation(const std::string& animationKey) {
    if (animations.find(animationKey) != animations.end() && animationKey != currentAnimationKey) {
        currentAnimationKey = animationKey;
        animationTimer = 0.0f;
        currentFrame = 0;
    }
}





void Enemy::update() {

    if (!isAlive()) {
        // Handle enemy death (e.g., remove from game, play death animation, etc.)
        return;
    }
    if (controltype == ControlType::Path) {
        moveOnPath();
    } else if (controltype == ControlType::Random) {
        moveRandomly();
    }
    //Collision Wall
    for (int i = 0; _scene->touchesWall(position, size) && i < 4; i++) {
        Rectangle touchedWall = _scene->getTouchedWall(position, size);
        Vector2 touchPoint = Vector2Clamp(position, {touchedWall.x, touchedWall.y},
                                          {touchedWall.x + touchedWall.width, touchedWall.y + touchedWall.height});
        Vector2 pushForce = Vector2Subtract(position, touchPoint);
        float overlapDistance = size - Vector2Length(pushForce);
        if (overlapDistance <= 0) {
            break;
        }
        pushForce = Vector2Normalize(pushForce);
        pushForce = Vector2Scale(pushForce, overlapDistance);
        position = Vector2Add(position, pushForce);
    }
    updateAnimation(GetFrameTime());
    static constexpr int FRAME_COUNT = 8; // Add this line, adjust the value as needed
    static constexpr float FRAME_DURATION = 0.1f; // Add this line, adjust the value as needed

    float animationTimer;
    int currentFrame;
}