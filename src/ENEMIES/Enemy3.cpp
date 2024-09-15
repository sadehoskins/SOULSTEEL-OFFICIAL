//
// Created by sadeh on 24.06.2024.
//

#include <regex>
#include "Enemy3.h"




Enemy3::Enemy3(gameplay *scene)
        : Enemy(scene, 2, 2, false, true, false,
                6.0f * 32.0f - 16.0f, 8 * 32 + 16, 8 * 32 - 16, 6 * 32 + 16),
          isShooting(false), shootingRange(200.0f), spiderToothSpeed(5.0f),
          shootCooldown(2.0f), shootTimer(0.0f)
{
    enemyType = "tacklespider";
    animData.entityType = "tacklespider";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;
}

void Enemy3::drawSpiderToothAttack()
{
    Texture2D spiderToothTexture = assestmanagergraphics::getAnimationTexture(animData.entityType, AnimationState::SPIDERTOOTH, animData.facingDirection);
    Rectangle sourceRec = {
            0, 0,
            static_cast<float>(spiderToothTexture.width),
            static_cast<float>(spiderToothTexture.height)
    };
    Rectangle destRec = {
            spiderToothPosition.x,
            spiderToothPosition.y,
            static_cast<float>(spiderToothTexture.width),
            static_cast<float>(spiderToothTexture.height)
    };
    DrawTexturePro(spiderToothTexture, sourceRec, destRec, {0, 0}, 0, WHITE);
}

Texture2D Enemy3::getCurrentTexture()
{
    return assestmanagergraphics::getAnimationTexture(animData.entityType, animData.currentAnimationState, animData.facingDirection);
}

void Enemy3::update()
{
    if (!healthManager.isAlive()) {
        // Handle death
        return;
    }

    Enemy::update();
    updateAnimation(GetFrameTime());

    // Check if main character is in range
    Vector2 characterPos = _scene->themaincharacter->position;
    float distanceToCharacter = Vector2Distance(position, characterPos);

    if (distanceToCharacter <= shootingRange)
    {
        if (!isShooting && shootTimer <= 0)
        {
            shootSpiderTooth();
        }
    }

    updateSpiderToothAttack();
    shootTimer -= GetFrameTime();
}

void Enemy3::shootSpiderTooth()
{
    isShooting = true;
    spiderToothPosition = position;
    Vector2 characterPos = _scene->themaincharacter->position;
    spiderToothDirection = Vector2Normalize(Vector2Subtract(characterPos, position));
    shootTimer = shootCooldown;
}

void Enemy3::updateSpiderToothAttack()
{
    if (isShooting)
    {
        spiderToothPosition = Vector2Add(spiderToothPosition, Vector2Scale(spiderToothDirection, spiderToothSpeed));

        // Check if the spidertooth is out of range or hits the character
        if (Vector2Distance(position, spiderToothPosition) > shootingRange ||
            CheckCollisionCircles(spiderToothPosition, 5, _scene->themaincharacter->position, _scene->themaincharacter->size))
        {
            isShooting = false;
            // Apply damage to the character if hit
            if (CheckCollisionCircles(spiderToothPosition, 5, _scene->themaincharacter->position, _scene->themaincharacter->size))
            {
                _scene->themaincharacter->healthManager.takeDamage(enemyDamage);
            }
        }
    }
}

void Enemy3::draw()
{
    Enemy::draw();
    if (isShooting)
    {
        drawSpiderToothAttack();
    }
}

Enemy3::~Enemy3() {
    // Destructor implementation (if needed)
}