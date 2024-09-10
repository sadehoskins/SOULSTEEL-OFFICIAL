//
// Created by sadeh on 11.07.2024.
//

#include "Utils.h"

// Collision detection functions

/*bool Collision::checkCollision(const maincharacter& maincharacter, const Enemy& enemy) {
    return CheckCollisionRecs(maincharacter.getCollisionRectangle(), enemy.getCollisionRectangle());
}

bool Collision::checkCollision(const maincharacter& maincharacter, const Wall& wall) {
    return CheckCollisionRecs(maincharacter.getCollisionRectangle(), wall.getCollisionRectangle());
}

bool Collision::checkCollision(const Enemy& enemy, const Wall& wall) {
    return CheckCollisionRecs(enemy.getCollisionRectangle(), wall.getCollisionRectangle());
}*/

bool Collision::checkCollision(const maincharacter& character, const Enemy& enemy) {
    // Implementation for collision between character and enemy
    Rectangle charRect = character.getCollisionRectangle();
    Rectangle enemyRect = enemy.getCollisionRectangle();
    return CheckCollisionRecs(charRect, enemyRect);
}

bool Collision::checkCollision(const maincharacter& character, const Wall& wall) {
    // Implementation for collision between character and wall
    Rectangle charRect = character.getCollisionRectangle();
    Rectangle wallRect = wall.getCollisionRectangle();
    return CheckCollisionRecs(charRect, wallRect);
}

bool Collision::checkCollision(const Enemy& enemy, const Wall& wall) {
    // Implementation for collision between enemy and wall
    Rectangle enemyRect = enemy.getCollisionRectangle();
    Rectangle wallRect = wall.getCollisionRectangle();
    return CheckCollisionRecs(enemyRect, wallRect);
}
