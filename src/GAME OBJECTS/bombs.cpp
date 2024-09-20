//
// Created by lrfri on 19.06.2024.
//

#include "bombs.h"
#include "../gameplay.h"

const float bombs::thrown_duration = 0.5f;
const float bombs::exploding_duration = 0.5f;
const float bombs::explosion_radius = 64.0f; //radius in which bomb gives damage
const int bombs::explosion_damage = 2; //change this to whatever damage the bomb gives
const float bombs::frame_duration = 0.1f;
const int bombs::thrown_frame_count = 8;
const int bombs::exploding_frame_count = 8;

bombs::bombs(gameplay* scene, Vector2 position) : gameobjects(scene) {
    this->position = position;
    state = thrown;
    stateTimer = 0;
    currentFrame = 0;
    frameTimer = 0;
}

void bombs::update() {
    stateTimer += GetFrameTime();
    updateAnimation();

    switch (state) {
        case thrown:
            if (stateTimer >= thrown_duration) {
                state = exploding;
                stateTimer = 0;
                currentFrame=0;
            }
            break;
        case exploding:
            if (stateTimer >= exploding_duration) {
                applyDamage();
                state = finished;
            }
            break;
        case finished:
            // The bomb will be deleted in the gameplay class
            break;
    }
}

void bombs::updateAnimation() {
    frameTimer += GetFrameTime();
    if (frameTimer >= frame_duration) {
        frameTimer = 0;
        currentFrame++;
        if (state == thrown && currentFrame >= thrown_frame_count) {
            currentFrame = 0;
        } else if (state == exploding && currentFrame >= exploding_frame_count) {
            currentFrame = 0;
        }
    }
}

void bombs::draw() {
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
        case finished:
            return;  // Don't draw anything
    }

    float frameWidth = currentTexture.width / frameCount;
    float frameHeight = currentTexture.height;

    Rectangle sourceRec = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
    Rectangle destRec = { position.x - frameWidth / 2, position.y - frameHeight / 2, frameWidth, frameHeight };
    DrawTexturePro(currentTexture, sourceRec, destRec, Vector2{0, 0}, 0, WHITE);
}

void bombs::applyDamage() {
    // Placeholder function for applying damage
    // TODO: Implement damage application to characters and enemies within explosion_radius
    // Implement this function later when damage system exists
}