//
// Created by lrfri on 22.08.2024.
//

#include "stone.h"
#include "../gameplay.h"
#include "../assestmanagergraphics.h"

Stone::Stone(gameplay* scene, Vector2 initialPosition) : gameobjects(scene) {
    position = initialPosition;
    stoneTexture = assestmanagergraphics::getTexture("item/stone");
    size = 32;
}

void Stone::update() {
    //doesn't do anything frame by frame
}

void Stone::draw() {
    DrawTexture(stoneTexture, position.x, position.y, WHITE);
}

bool Stone::tryMove(Vector2 direction) {
    if (_scene == nullptr) {
        //std::cout << "Error: Stone's _scene is null" << std::endl;
        return false;
    }

    Vector2 newPosition = {position.x + direction.x * 32,position.y + direction.y * 32};

    Vector2 newTile = {std::floor(newPosition.x / 32), std::floor(newPosition.y / 32)};

    /*std::cout << "Attempting to move stone from (" << position.x << ", " << position.y << ") to (" << newPosition.x << ", " << newPosition.y << ")" << std::endl;*/

    // Add bounds checking
    if (newTile.x < 0 || newTile.x >= _scene->mapWidth || newTile.y < 0 || newTile.y >= _scene->mapHeight) {
        //std::cout << "Stone move failed: Out of bounds" << std::endl;
        return false;
    }

    // Check for wall collision
    int tileID = _scene->getTileAt(newPosition.x, newPosition.y);
    if (_scene->isTileYouCantPushStoneOnto(tileID)) {
        //std::cout << "Stone move failed: Wall collision (Tile ID: " << tileID << ")" << std::endl;
        return false;
    }

    // Check for collision with other stones
    if (_scene->touchesStone(newTile)) {
        //std::cout << "Stone move failed: Stone collision" << std::endl;
        return false;
    }

    // If we've passed all checks, move the stone
    position = newPosition;
    //std::cout << "Stone moved successfully to (" << position.x << ", " << position.y << ")" << std::endl;
    return true;
}