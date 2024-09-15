//
// Created by lrfri on 19.06.2024.
//

#include "bombs.h"
#include "../gameplay.h"

const float bombs::thrown_duration = 1.5f;
const float bombs::exploding_duration = 1.0f;
const float bombs::explosion_radius = 64.0f; //radius in which bomb gives damage
const int bombs::explosion_damage = 2; //change this to whatever damage the bomb gives
const float bombs::frame_duration = 0.1f;
const int bombs::thrown_frame_count = 8;
const int bombs::exploding_frame_count = 8;

bombs::bombs(gameplay* scene, Vector2 position) : gameobjects(scene) {
    std::cout << "Creating bomb. Scene address: " << scene << std::endl;
    _scene = scene;
    this->position = position;
    state = thrown;
    stateTimer = 0;
    currentFrame = 0;
    frameTimer = 0;
}

void bombs::update() {
    std::cout << "Entering bombs::update. This address: " << this << std::endl;
    std::cout << "Bomb state: " << state << ", Timer: " << stateTimer << std::endl;

    stateTimer += GetFrameTime();
    updateAnimation();

    switch (state) {
        case thrown:
            if (stateTimer >= thrown_duration) {
                std::cout << "Transitioning from thrown to exploding" << std::endl;
                state = exploding;
                stateTimer = 0;
                currentFrame = 0;
            }
            break;
        case exploding:
            if (stateTimer >= exploding_duration) {
                std::cout << "Applying damage" << std::endl;
                applyDamage();
                std::cout << "Transitioning to finished state" << std::endl;
                state = finished;
            }
            break;
        case finished:
            std::cout << "Bomb in finished state" << std::endl;
            break;
        default:
            std::cout << "Error: Invalid bomb state" << std::endl;
            break;
    }

    std::cout << "Exiting bombs::update. Final state: " << state << std::endl;
}


void bombs::updateAnimation() {
    frameTimer += GetFrameTime();
    float frameDuration = (state == thrown) ? 0.2f : 0.125f;  // Slower for thrown state, faster for exploding
    if (frameTimer >= frameDuration) {
        frameTimer = 0;
        currentFrame++;
        if (state == thrown && currentFrame >= thrown_frame_count) {
            currentFrame = 0;
        } else if (state == exploding && currentFrame >= exploding_frame_count) {
            currentFrame = exploding_frame_count - 1;  // Stay on last frame
        }
    }
}

void bombs::applyDamage() {
    std::cout << "Entering applyDamage. This address: " << this << std::endl;
    std::cout << "Applying damage: Position = (" << position.x << ", " << position.y << ")" << std::endl;//debug

    if (!_scene || !_scene->themaincharacter) {
        std::cout << "Scene or main character is null in applyDamage" << std::endl;
        return;  // Add this line to return early
    }//debug

    //if (!_scene) return;
    if (!_scene) {
        std::cout << "Error: _scene is null in applyDamage" << std::endl;
        return;
    }
    std::cout << "_scene address: " << _scene << std::endl;

    if (!_scene->themaincharacter) {
        std::cout << "Error: themaincharacter is null in applyDamage" << std::endl;
        return;
    }
    std::cout << "themaincharacter address: " << _scene->themaincharacter << std::endl;

    const std::vector<Enemy*>& enemies = _scene->getEnemies();
    maincharacter* player = _scene->themaincharacter;

    if (player && CheckCollisionCircles(position, explosion_radius, player->position, player->size)) {
        player->healthManager.takeDamage(explosion_damage);
    }

    for (Enemy* enemy : enemies) {
        if (enemy && CheckCollisionCircles(position, explosion_radius, enemy->position, enemy->size)) {
            enemy->healthManager.takeDamage(explosion_damage);
        }
    }
    std::cout << "Exiting applyDamage" << std::endl;
}

void bombs::draw() {

    if (state == finished) {
        std::cout << "Skipping draw for finished bomb at address: " << this << std::endl;
        return;
    }


    std::cout << "Entering bombs::draw. This address: " << this << ", State: " << state << std::endl;

    Texture2D currentTexture;
    int frameCount;
    switch (state) {
        case thrown:
            currentTexture = bomb_thrown;
            frameCount = thrown_frame_count;
            break;
        case exploding:
            currentTexture = bomb_exploding;
            frameCount = exploding_frame_count;
            break;
        default:
            std::cout << "Invalid bomb state in draw" << std::endl;
            return;
    }

    if (currentTexture.id == 0) {
        std::cout << "Invalid texture for bomb in state " << state << std::endl;
        return;
    }

    /* Draw explosion radius for debug
    if (state == exploding) {
        float alpha = (sinf(stateTimer * 10) + 1) / 4 + 0.5f;  // Creates a value between 0.5 and 1
        DrawCircleLines(position.x, position.y, explosion_radius, ColorAlpha(RED, alpha));
    }*/


    float frameWidth = currentTexture.width / frameCount;
    float frameHeight = currentTexture.height;

    Rectangle sourceRec = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
    Rectangle destRec = { position.x - frameWidth / 2, position.y - frameHeight / 2, frameWidth, frameHeight };

    DrawTexturePro(currentTexture, sourceRec, destRec, Vector2{0, 0}, 0, WHITE);

    std::cout << "Exiting bombs::draw" << std::endl;
}

