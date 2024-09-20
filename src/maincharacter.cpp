// Created by lrfri on 20.05.2024.
//

#include "maincharacter.h"
#include "scene.h"
#include "Enums.h"

const float maincharacter::FRAME_DURATION = 0.1f;
const float maincharacter::DASH_ANIMATION_SPEED = 0.02f;
const float maincharacter::bomb_cooldown = 1.0f;
//const int maincharacter::MAX_HEALTH;
const float maincharacter::ATTACK_DURATION = 0.5f;
const float maincharacter::ATTACK_COOLDOWN = 1.0f;

int maincharacter::attackPower = 2;

void maincharacter::attack(Enemy *target) {
    //target->health -= Enemy::attackPower;

}

void calculateDamage(maincharacter &maincharacter, int damage) {
    maincharacter.health -= damage;
    if (maincharacter.health < 0) {
        maincharacter.health = 0;
    }
}

void maincharacter::collisionwall() {
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
}

void maincharacter::collisionenemies() {
    const std::vector<Enemy *> &enemies = _scene->getEnemies();

    for (int i = 0; i < enemies.size(); i++) {
        if (Collision::checkCollision(*this, *enemies[i])) {
            Rectangle enemyRect = enemies[i]->getCollisionRectangle();
            for (int j = 0; j < 4; j++) {
                Vector2 touchPoint = Vector2Clamp(position, {enemyRect.x, enemyRect.y},
                                                  {enemyRect.x + enemyRect.width, enemyRect.y + enemyRect.height});
                Vector2 pushForce = Vector2Subtract(position, touchPoint);
                float overlapDistance = size - Vector2Length(pushForce);
                if (overlapDistance <= 1) {
                    break;
                }
                this->health--;
                pushForce = Vector2Normalize(pushForce);
                pushForce = Vector2Scale(pushForce, overlapDistance);
                position = Vector2Add(position, pushForce);
            }
        }
    }
}

void maincharacter::collisionbars() {
    if (!souldashactivated) {
        for (int i = 0; _scene->touchesBars(position, size) && i < 4; i++) {
            Rectangle touchedBars = _scene->getTouchedBars(position, size);
            Vector2 touchPoint = Vector2Clamp(position, {touchedBars.x, touchedBars.y},
                                              {touchedBars.x + touchedBars.width, touchedBars.y + touchedBars.height});
            Vector2 pushForce = Vector2Subtract(position, touchPoint);
            float overlapDistance = size - Vector2Length(pushForce);
            if (overlapDistance <= 0) {
                break;
            }
            pushForce = Vector2Normalize(pushForce);
            pushForce = Vector2Scale(pushForce, overlapDistance);
            position = Vector2Add(position, pushForce);
        }
    }
}

void maincharacter::collisionabyss() {
    if (_scene->touchesAbyss(position, size)) {
        if (currentmodus == robotmodus || (currentmodus == soulmodus && !souldashactivated)) {
            // Robot or non-dashing soul falls into abyss, soul using dash is fine
            //health--; // Lose one heart
            felldown = true;
            position = lastSafePosition; // Reset position

            //falling animation or something here

        }
    } else {
        updateLastSafePosition();
        felldown = false;
    }
}

void maincharacter::draw() {
    Texture2D currentTexture = getCurrentTexture();

    if (currentState == AnimationState::SWITCH) {
        int currentFrame = static_cast<int>((switchAnimationTimer / SWITCH_ANIMATION_DURATION) * SWITCH_FRAME_COUNT) % SWITCH_FRAME_COUNT;

        float frameWidth = static_cast<float>(currentTexture.width) / SWITCH_FRAME_COUNT;
        Rectangle sourceRec = {
                currentFrame * frameWidth,
                0.0f,
                frameWidth,
                static_cast<float>(currentTexture.height)
        };

        Vector2 drawPosition = {
                position.x - frameWidth / 2,
                position.y - currentTexture.height / 2
        };

        DrawTextureRec(currentTexture, sourceRec, drawPosition, WHITE);
    } else {
        // Existing drawing logic for other states
        drawsoul();  // or drawrobot() depending on your current mode
    }

    if (currentState == AnimationState::DUST) {
        drawDustAnimation();
    }

    if (felldown) {
        DrawText("You fell into the abyss. You lost one life.", 10 * 15, 7 * 15, 20, RED);
    }
    //Draw Health for debug purposes
    DrawText(TextFormat("Health: %i",this->health),position.x,position.y,10,BLACK);
}

void maincharacter::drawsoul() {

    //draws current frame of soul animation
    Texture2D currentTexture = getCurrentTexture();

    DrawTextureRec(currentTexture, frameRec,
                   {position.x - frameRec.width / 2, position.y - frameRec.height / 2},
                   WHITE);

    if (souldustcanbeused()) {
        DrawText("Press L to use Soul Dust", position.x - 50, position.y - 40, 10, YELLOW);
    }

}

void maincharacter::drawrobot() {

    //DrawCircle(position.x, position.y, size,GRAY);
    DrawTexture(characterRobotTexture, position.x - 16, position.y - 32, WHITE);

    if (currentAnimationState == AnimationState::ATTACK) {
        // Draw attack animation
        std::string direction;
        switch (currentDirection) {
            case Direction::Up: direction = "back"; break;
            case Direction::Down: direction = "front"; break;
            case Direction::Left: direction = "side_left"; break;
            case Direction::Right: direction = "side_right"; break;
        }

        Texture2D bodyTexture = assestmanagergraphics::getCharacterTexture("robot", "Character - Robot - Melee " + direction + " - Body - animated");
        Texture2D armTexture = assestmanagergraphics::getCharacterTexture("robot", "Character - Robot - Melee " + direction + " - Arm - animated");

        int frame = static_cast<int>((attackTimer / ATTACK_DURATION) * 8) % 8;
        Rectangle sourceRect = { frame * 32.0f, 0, 32, 32 };
        Vector2 origin = { 16, 16 };

        DrawTexturePro(bodyTexture, sourceRect, (Rectangle){ position.x, position.y, 32, 32 }, origin, 0, WHITE);
        DrawTexturePro(armTexture, sourceRect, (Rectangle){ position.x, position.y, 32, 32 }, origin, 0, WHITE);
    } else {
        // Draw normal robot texture
        DrawTexture(characterRobotTexture, position.x - 16, position.y - 32, WHITE);
    }
}



//g
Rectangle maincharacter::getCollisionRectangle() const {
    return Rectangle{position.x - size / 2, position.y - size / 2, size, size};
}

Texture2D maincharacter::getCurrentTexture() {
    std::string state;
    std::string key;

    switch (currentState) {
        case AnimationState::IDLE:
            state = "idle_";
            break;
        case AnimationState::WALK:
            state = "walk_";
            break;
        case AnimationState::DASH:
            state = "dash_";
            break;
        case AnimationState::DUST:
            state = "dust_";
            break;
        case AnimationState::SWITCH:
            // Handle the switching state
            return assestmanagergraphics::getCharacterTexture(
                    "Switch-Animation",
                    (currentmodus == soulmodus)
                    ? "Character - Robot+Soul - Switch Soul to Robot - animated"
                    : "Character - Robot+Soul - Switch Robot to Soul - animated"
            );
        default:
            state = "idle_";
    }

    std::string direction;
    switch (currentDirection) {
        case Direction::Up:
            direction = "back";
            break;
        case Direction::Down:
            direction = "front";
            break;
        case Direction::Left:
            direction = "left";
            break;
        case Direction::Right:
            direction = "right";
            break;
    }

    key = state + direction;

    std::string characterName = (currentmodus == soulmodus) ? "soul" : "robot";

    if (characterName == "robot") {
        if (state == "idle_" || state == "walk_") {
            if (direction == "front" || direction == "left" || direction == "right") {
                key += isCharacterPossessed() ? "_possessed" : "_normal";
            }
        } else if ((state == "melee_" || state == "ranged_") && (direction == "left" || direction == "right")) {
            key += "_body"; // or "_arm" depending on your needs
        }
    }

    return assestmanagergraphics::getCharacterTexture(characterName, key);
}

int getHealth(const maincharacter &maincharacter) {
    return maincharacter.health;
}

maincharacter::maincharacter(gameplay *scene) : _scene(scene) {
    position = {32 * 12, 32 * 6};  // Set initial position
    currentState = AnimationState::IDLE;
    currentDirection = static_cast<Direction>(Up);
    currentFrame = 0;
    frameCounter = 0.0f;
    currentmodus = soulmodus;
    lookingdirection = Down;

    // Set initial frame rectangle
    Texture2D currentTexture = getCurrentTexture();
    float frameWidth = static_cast<float>(currentTexture.width) / FRAME_COUNT;
    float frameHeight = static_cast<float>(currentTexture.height);
    frameRec = {0, 0, frameWidth, frameHeight};

    isPossessed = false;
}

bool maincharacter::canSwitchToRobot() const {
    if (currentmodus != soulmodus) return true;  // Can always switch from robot to soul

    Vector2 robotPos = _scene->getRobotPosition();
    float takeoverRadius = _scene->getTakeoverRadius();

    return CheckCollisionPointCircle(position, robotPos, takeoverRadius);
}

bool maincharacter::isCharacterPossessed() const {
    return isPossessed;
}

void maincharacter::setPossessionStatus(bool possessed) {
    isPossessed = possessed;
}


void maincharacter::maincharacterwalking() {
    Vector2 movement = {0, 0};
    bool moved = false;

    if (IsKeyDown(KEY_S)) {
        movement.y = stepsize;
        lookingdirection = Direction::Down;
        moved = true;
    }
    if (IsKeyDown(KEY_W)) {
        movement.y = -stepsize;
        lookingdirection = Direction::Up;
        moved = true;
    }
    if (IsKeyDown(KEY_A)) {
        movement.x = -stepsize;
        lookingdirection = Direction::Left;
        moved = true;
    }
    if (IsKeyDown(KEY_D)) {
        movement.x = stepsize;
        lookingdirection = Direction::Right;
        moved = true;
    }

    if (moved) {
        Vector2 newPosition = {position.x + movement.x, position.y + movement.y};
        Vector2 currentTile = {std::floor(position.x / 32), std::floor(position.y / 32)};
        Vector2 newTile = {std::floor(newPosition.x / 32), std::floor(newPosition.y / 32)};

        bool canMove = true;
        if (_scene->touchesWall(newPosition, size) || _scene->isBlockAt(newTile) || _scene->isSwitchAt(newTile)) {
            canMove = false;
        } else if (currentTile.x != newTile.x || currentTile.y != newTile.y) {
            // Only check for stone collision if we're moving to a new tile
            if (_scene->touchesStone(newTile)) {
                if (currentmodus == robotmodus&& IsKeyDown(KEY_K)) {
                    Vector2 pushDirection = {newTile.x - currentTile.x, newTile.y - currentTile.y};
                    Stone* stone = _scene->getStoneAt(newTile);
                    if (stone != nullptr) {
                        //std::cout << "Attempting to push stone" << std::endl;
                        if (stone->tryMove(pushDirection)) {
                            //std::cout << "Stone pushed successfully" << std::endl;
                            // Move the character only if the stone was successfully pushed
                            position = newPosition;
                        } else {
                            //std::cout << "Failed to push stone" << std::endl;
                            canMove = false;
                        }
                    } else {
                        //std::cout << "Warning: Null stone detected at tile (" << newTile.x << ", " << newTile.y << ")" << std::endl;
                        canMove = false;
                    }
                } else {
                    //std::cout << "Cannot push stone in soul mode" << std::endl;
                    canMove = false;
                }
            } else {
                // If there's no stone, the character can move freely
                position = newPosition;
            }
        } else {
            // Moving within the same tile
            position = newPosition;
        }

        if (!canMove) {
            //std::cout << "Movement blocked" << std::endl;
        }
    }
}


void setAttackPower(int attack) {
    int attackPower = attack;
}

void maincharacter::souldash() {

    if (currentState != AnimationState::DASH) {
        // Start of new dash
        currentState = AnimationState::DASH;
        dashProgress = 0.0f;
        dashStartPosition = position;
        dashEndPosition = position;

        switch (currentDirection) {
            case Direction::Up:
                dashEndPosition.y -= DASH_DISTANCE;
                break;
            case Direction::Right:
                dashEndPosition.x += DASH_DISTANCE;
                break;
            case Direction::Down:
                dashEndPosition.y += DASH_DISTANCE;
                break;
            case Direction::Left:
                dashEndPosition.x -= DASH_DISTANCE;
                break;
        }
    }

    dashProgress += DASH_ANIMATION_SPEED;
    currentFrame = static_cast<int>(dashProgress * DASH_FRAME_COUNT) % DASH_FRAME_COUNT;

    // Calculate new position
    Vector2 newPosition = Vector2Lerp(dashStartPosition, dashEndPosition, dashProgress);

    //checks wall collision
    if (!_scene->touchesWall(newPosition, size)) {
        position = newPosition;
    } else {
        currentState = AnimationState::IDLE;
        dashProgress = 1.0f; //ensures dah ends
    }


    /* Check if the new position is valid (not inside a wall)
    for (int i = 0; i <= stepsizesouldash; i++) {
        Vector2 checkPosition = Vector2Lerp(position, newPosition, (float)i / stepsizesouldash);
        if (_scene->touchesWall(checkPosition, size)) {
            // If we hit a wall, stop at the previous valid position
            newPosition = Vector2Lerp(position, newPosition, (float)(i-1) / stepsizesouldash);
            break;
        }
    }*/

    //position = newPosition;
    //updateDashAnimation(GetFrameTime());

    //controlls speed of dash animation
    frameCounter += DASH_ANIMATION_SPEED;
    if (frameCounter >= 1.0f) {
        frameCounter = 0.0f;
        currentFrame++;
        if (currentFrame >= FRAME_COUNT) {
            currentFrame = 0;
            currentState = AnimationState::IDLE;  // Ends dash after one full animation cycle
        }
    }
// Update the frame rectangle for drawing
    Texture2D currentTexture = getCurrentTexture();
    float frameWidth = static_cast<float>(currentTexture.width) / DASH_FRAME_COUNT;
    float frameHeight = static_cast<float>(currentTexture.height);
    frameRec = {currentFrame * frameWidth, 0, frameWidth, frameHeight};

    // End dash when animation completes
    if (dashProgress >= 1.0f) {
        currentState = AnimationState::IDLE;
    }
}

bool maincharacter::souldustcanbeused() const {
    return _scene->isAdjacentToFirebowl(position);
}

void maincharacter::souldust() {
    if (souldustcanbeused() && IsKeyPressed(KEY_L)) {
        auto [bowlX, bowlY] = _scene->getNearestFirebowlTile(position);
        if (bowlX != -1 && bowlY != -1 && !_scene->isFirebowlActivated(bowlX, bowlY)) {
            _scene->activateFirebowl(bowlX, bowlY);
        }
    }
}

void maincharacter::startDusting() {
    if (souldustcanbeused()) {
        isDusting = true;
        dustAnimationTimer = 0.0f;
        dustPosition = position;
        currentState = AnimationState::DUST; // Assuming you have a DUST state in your State enum
    }
}

void maincharacter::updateDustAnimation(float deltaTime) {
    if (isDusting) {
        dustAnimationTimer += deltaTime;
        if (dustAnimationTimer >= DUST_ANIMATION_DURATION) {
            isDusting = false;
            currentState = AnimationState::IDLE; // Or whatever state should come after dusting
        }
    }
}

void maincharacter::drawDustAnimation() {
    if (isDusting) {
        Texture2D characterDustTexture = getCurrentTexture();
        Texture2D dustEffectTexture = assestmanagergraphics::getCharacterTexture("soul", "dust_" + std::to_string(static_cast<int>(currentDirection)) + "_dust");

        int currentFrame = static_cast<int>((dustAnimationTimer / DUST_ANIMATION_DURATION) * DUST_FRAME_COUNT) % DUST_FRAME_COUNT;

        // Draw character dust animation
        DrawTextureRec(characterDustTexture,
                       Rectangle{static_cast<float>(currentFrame * characterDustTexture.width / DUST_FRAME_COUNT), 0.0f,
                                 static_cast<float>(characterDustTexture.width / DUST_FRAME_COUNT),
                                 static_cast<float>(characterDustTexture.height)},
                       Vector2{position.x - static_cast<float>(characterDustTexture.width) / (2.0f * DUST_FRAME_COUNT),
                               position.y - static_cast<float>(characterDustTexture.height) / 2.0f},
                       WHITE);

        // Draw dust effect animation
        DrawTextureRec(dustEffectTexture,
                       Rectangle{static_cast<float>(currentFrame * dustEffectTexture.width / DUST_FRAME_COUNT), 0.0f,
                                 static_cast<float>(dustEffectTexture.width / DUST_FRAME_COUNT),
                                 static_cast<float>(dustEffectTexture.height)},
                       Vector2{dustPosition.x - static_cast<float>(dustEffectTexture.width) / (2.0f * DUST_FRAME_COUNT),
                               dustPosition.y - static_cast<float>(dustEffectTexture.height) / 2.0f},
                       WHITE);
    }
}

void maincharacter::update() {
    Vector2 oldPosition = position;
    maincharacterwalking();

    switch (currentmodus) {
        case soulmodus:
            if (IsKeyPressed(KEY_SPACE) && !isSwitching && canSwitchToRobot()) {
                isSwitching = true;
                switchAnimationTimer = 0.0f;
                currentState = AnimationState::SWITCH;
            }

            if (IsKeyPressed(KEY_I) && currentState != AnimationState::DASH && !isSwitching) {
                souldashactivated = true;
                souldash();
            } else if (currentState == AnimationState::DASH) {
                souldash();
            } else {
                souldashactivated = false;
            }
            souldust();
            break;
        case robotmodus:
            if (IsKeyPressed(KEY_SPACE) && !isSwitching) {
                isSwitching = true;
                switchAnimationTimer = 0.0f;
                currentState = AnimationState::SWITCH;
            }

            if (IsKeyPressed(KEY_B) && !isSwitching) {
                if (_scene->isAdjacentToSwitch(position)) {
                    Vector2 characterTile = {std::floor(position.x / 32), std::floor(position.y / 32)};
                    _scene->toggleSwitchAt(characterTile);
                }
            }
            if (IsKeyPressed(KEY_J) && (GetTime() - lastBombThrowTime) >= bomb_cooldown && !isSwitching) {
                throwBomb();
            }
            break;
    }

    if (IsKeyPressed(KEY_U) && currentmodus == robotmodus && GetTime() - lastAttackTime >= ATTACK_COOLDOWN) {
        performMeleeAttack();
    }

    if (currentAnimationState == AnimationState::ATTACK) {
        attackTimer += GetFrameTime();
        if (attackTimer >= ATTACK_DURATION) {
            currentAnimationState = AnimationState::IDLE;
            attackTimer = 0.0f;
        }

    }

    if (isSwitching) {
        switchAnimationTimer += GetFrameTime();
        if (switchAnimationTimer >= SWITCH_ANIMATION_DURATION) {
            isSwitching = false;
            currentmodus = (currentmodus == soulmodus) ? robotmodus : soulmodus;
            currentState = AnimationState::IDLE;
        }
    } else {
        if (Vector2Equals(oldPosition, position) && currentState != AnimationState::DASH) {
            currentState = AnimationState::IDLE;
        } else if (currentState != AnimationState::DASH) {
            currentState = AnimationState::WALK;
        }

        if (IsKeyDown(KEY_W)) currentDirection = Direction::Up;
        else if (IsKeyDown(KEY_S)) currentDirection = Direction::Down;
        else if (IsKeyDown(KEY_A)) currentDirection = Direction::Left;
        else if (IsKeyDown(KEY_D)) currentDirection = Direction::Right;
    }

    collisionwall();
    collisionenemies();
    collisionbars();
    collisionabyss();
    updateLastSafePosition();

    if (currentState == AnimationState::SWITCH) {
        // The animation update is handled by the switchAnimationTimer
    } else {
        updateAnimation(GetFrameTime());
    }
    updateDustAnimation(GetFrameTime());
}

void maincharacter::updateAnimation(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= FRAME_DURATION) {
        frameCounter = 0.0f;
        currentFrame++;
        if (currentFrame >= FRAME_COUNT) currentFrame = 0;
    }

    // Update frameRec based on current frame
    Texture2D currentTexture = getCurrentTexture();
    float frameWidth = static_cast<float>(currentTexture.width) / FRAME_COUNT;
    float frameHeight = static_cast<float>(currentTexture.height);
    frameRec = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
}

void maincharacter::updateDashAnimation(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= FRAME_DURATION) {
        frameCounter = 0.0f;
        currentFrame++;
        if (currentFrame >= FRAME_COUNT) {
            currentFrame = 0;
            currentState = AnimationState::IDLE;  // Ends dash after one full animation cycle
        }
    }

    Texture2D currentTexture = getCurrentTexture();
    float frameWidth = static_cast<float>(currentTexture.width) / FRAME_COUNT;
    float frameHeight = static_cast<float>(currentTexture.height);
    frameRec = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
}

void maincharacter::updateLastSafePosition() {
    if (!_scene->touchesAbyss(position, size)) {
        lastSafePosition = position;
    }
}

void maincharacter::throwBomb() {
    //std::cout << "Throwing bomb!" << std::endl;
    Vector2 bombPosition;
    switch (currentDirection) {
        case Direction::Up:
            bombPosition = {position.x, position.y - bombthrowing_range};
            break;
        case Direction::Down:
            bombPosition = {position.x, position.y + bombthrowing_range};
            break;
        case Direction::Right:
            bombPosition = {position.x + bombthrowing_range, position.y};
            break;
        case Direction::Left:
            bombPosition = {position.x - bombthrowing_range, position.y};
            break;
    }
    bombs* newBomb = new bombs(_scene, bombPosition);
    _scene->addBomb(newBomb);
    lastBombThrowTime = GetTime();
}

//*NEW CODE*
/*
void maincharacter::takeDamage(int amount) {
    m_health = std::max(0, m_health - amount);
}

void maincharacter::heal(int amount) {
    m_health = std::min(MAX_HEALTH, m_health + amount);
}

bool maincharacter::isAlive() const {
    return m_health > 0;
}

float maincharacter::getHealthPercentage() const {
    return static_cast<float>(m_health) / MAX_HEALTH;
}
 */

    void maincharacter::performMeleeAttack() {
        currentAnimationState = AnimationState::ATTACK;
        attackTimer = 0.0f;
        lastAttackTime = GetTime();

        // Check for enemies in range and apply damage
        for (auto& enemy : _scene->getEnemies()) {
            if (CheckCollisionCircles(position, size + 32.0f, enemy->position, enemy->size)) {
                //enemy->takeDamage(2); // Apply 2 damage to the enemy
            }
        }
    }