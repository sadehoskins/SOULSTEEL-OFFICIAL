//
// Created by sadeh on 24.06.2024.
//

#include <regex>
#include "Enemy3.h"

//*NEW CODE*
const float Enemy3::SPIDERTOOTH_FRAME_DURATION = 0.1f;


Enemy3::Enemy3(gameplay *scene, const Vector2& initialPosition)
        : Enemy(scene, 2, 2, false, true, false,
                initialPosition.x, initialPosition.y, initialPosition.x, initialPosition.y),
          isShooting(false), shootingRange(200.0f), spiderToothSpeed(5.0f),
          shootCooldown(2.0f), shootTimer(0.0f), spiderToothFrame(0), spiderToothFrameTimer(0.0f)
{
    enemyType = "tacklespider";
    animData.entityType = "tacklespider";
    animData.currentAnimationState = AnimationState::IDLE;
    animData.facingDirection = Direction::Down;
    animData.currentFrame = 0;
    controltype = ControlType::Path;
    position = initialPosition;
}


void Enemy3::drawSpiderToothAttack()
{
    Texture2D spiderToothTexture = assestmanagergraphics::getAnimationTexture(animData.entityType, AnimationState::SPIDERTOOTH, animData.facingDirection);

    float frameWidth = static_cast<float>(spiderToothTexture.width) / SPIDERTOOTH_FRAME_COUNT;
    Rectangle sourceRec = {
            spiderToothFrame * frameWidth, 0,
            frameWidth,
            static_cast<float>(spiderToothTexture.height)
    };
    Rectangle destRec = {
            spiderToothPosition.x - frameWidth / 2,
            spiderToothPosition.y - spiderToothTexture.height / 2,
            frameWidth,
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

    Vector2 oldPosition = position;
    Enemy::update();  // This includes the path movement logic

    updateAnimationBasedOnMovement(oldPosition);

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
    // Set the spiderToothDirection based on the enemy's facing direction
    switch (animData.facingDirection)
    {
        case Direction::Up:
            spiderToothDirection = {0, -1};
            break;
        case Direction::Down:
            spiderToothDirection = {0, 1};
            break;
        case Direction::Left:
            spiderToothDirection = {-1, 0};
            break;
        case Direction::Right:
            spiderToothDirection = {1, 0};
            break;
    }
    shootTimer = shootCooldown;
    spiderToothFrame = 0;
    spiderToothFrameTimer = 0.0f;
}

void Enemy3::updateSpiderToothAttack()
{
    if (isShooting)
    {
        spiderToothPosition = Vector2Add(spiderToothPosition, Vector2Scale(spiderToothDirection, spiderToothSpeed));

        // Update spider tooth animation
        spiderToothFrameTimer += GetFrameTime();
        if (spiderToothFrameTimer >= SPIDERTOOTH_FRAME_DURATION)
        {
            spiderToothFrame = (spiderToothFrame + 1) % SPIDERTOOTH_FRAME_COUNT;
            spiderToothFrameTimer -= SPIDERTOOTH_FRAME_DURATION;
        }

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

    drawHealthStatus();  // Draw the health status

    if (isShooting)
    {
        drawSpiderToothAttack();
    }
}

Enemy3::~Enemy3() {
    // Destructor implementation (if needed)
}