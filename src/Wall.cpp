//
// Created by sadeh on 11.07.2024.
//

#include "Wall.h"
Wall::Wall(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {
    // Constructor implementation
}

Wall::~Wall() {
    // Destructor implementation
}

Rectangle Wall::getCollisionRectangle() const {
    // Return the collision rectangle based on wall position and size
    return { x, y, width, height };
}