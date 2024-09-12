// Created by lrfri on 20.05.2024.
//

#include "maincharacter.h"
#include "scene.h"
#include "Enums.h"
#include "GAME OBJECTS/bombs.h"

const float maincharacter::FRAME_DURATION = 0.1f;
const float maincharacter::DASH_ANIMATION_SPEED = 0.02f;
const float maincharacter::bomb_cooldown = 1.0f;

const float maincharacter::ATTACK_DURATION = 0.5f;
const float maincharacter::ATTACK_COOLDOWN = 1.0f;

int maincharacter::attackPower = 2;

maincharacter::maincharacter(gameplay *scene) : _scene(scene), healthManager(10) { //initialize health
    position = {32 * 12, 32 * 6};  // Set initial position
    currentState = AnimationState::IDLE; //set initial animation state
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



void calculateDamage(maincharacter &maincharacter, int damage) {
    maincharacter.health -= damage;
    if (maincharacter.health < 0) {
        maincharacter.health = 0;
    }
}

bool maincharacter::canSwitchToRobot() const {
    if (currentmodus != soulmodus) return true;  // Can always switch from robot to soul

    Vector2 robotPos = _scene->getRobotPosition();
    float takeoverRadius = _scene->getTakeoverRadius();

    return CheckCollisionPointCircle(position, robotPos, takeoverRadius);
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
            if (immunityTimer <= 0.0f) {
                healthManager.takeDamage(1);
                std::cout << "Health after enemy collision: " << healthManager.getCurrentHealth() << "/" << healthManager.getMaxHealth() << std::endl;
                immunityTimer = IMMUNITY_DURATION;

                // Push the character away from the enemy
                Vector2 pushDirection = Vector2Subtract(position, enemies[i]->position);
                pushDirection = Vector2Normalize(pushDirection);
                position = Vector2Add(position, Vector2Scale(pushDirection, 10.0f)); // Push 10 pixels away
            }

            // Always push the character slightly to prevent sticking
            Vector2 separationVector = Vector2Subtract(position, enemies[i]->position);
            separationVector = Vector2Normalize(separationVector);
            position = Vector2Add(position, Vector2Scale(separationVector, 1.0f));
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
        if (_scene->touchesAbyss(position, size)) {
            if (currentmodus == robotmodus || (currentmodus == soulmodus && !souldashactivated)) {
                if (immunityTimer <= 0.0f) {
                    felldown = true;
                    position = lastSafePosition; // Reset position
                    healthManager.takeDamage(1); // Apply damage
                    std::cout << "Health after abyss damage: " << healthManager.getCurrentHealth() << "/"
                              << healthManager.getMaxHealth() << std::endl;
                    immunityTimer = IMMUNITY_DURATION;
                    abyssMessageTimer = ABYSS_MESSAGE_DURATION;
                }
            }
        } else {
            updateLastSafePosition();
            felldown = false;
        }
    }

}

void maincharacter::draw() {

    std::cout << "Current mode: " << (currentmodus == robotmodus ? "Robot" : "Soul") << std::endl;


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

    if (abyssMessageTimer > 0.0f) {
        DrawText("You fell into the abyss. You lost one life.", 10 * 15, 7 * 15, 20, RED);
    }

    // Draw health
    DrawText(TextFormat("Health: %i/%i", healthManager.getCurrentHealth(), healthManager.getMaxHealth()), 10, 10, 20, WHITE);

    // Optionally, indicate immunity
    if (immunityTimer > 0.0f) {
        DrawText("IMMUNE", position.x - 30, position.y - 40, 20, YELLOW);
    }


}


void maincharacter::drawDustAnimation() {
    if (isDusting) {
        Texture2D dustTexture = assestmanagergraphics::getAnimationTexture("soul", AnimationState::DUST, currentDirection);

        int currentFrame = static_cast<int>((dustAnimationTimer / DUST_ANIMATION_DURATION) * DUST_FRAME_COUNT) % DUST_FRAME_COUNT;

        // Draw character dust animation
        Rectangle characterSourceRec = {
                static_cast<float>(currentFrame * dustTexture.width / DUST_FRAME_COUNT), 0.0f,
                static_cast<float>(dustTexture.width / DUST_FRAME_COUNT),
                static_cast<float>(dustTexture.height)
        };
        Vector2 characterDrawPos = {
                position.x - static_cast<float>(dustTexture.width) / (2.0f * DUST_FRAME_COUNT),
                position.y - static_cast<float>(dustTexture.height) / 2.0f
        };
        DrawTextureRec(dustTexture, characterSourceRec, characterDrawPos, WHITE);

        // Draw dust effect animation
        Rectangle dustSourceRec = {
                static_cast<float>(currentFrame * dustTexture.width / DUST_FRAME_COUNT), 0.0f,
                static_cast<float>(dustTexture.width / DUST_FRAME_COUNT),
                static_cast<float>(dustTexture.height)
        };
        Vector2 dustDrawPos = {
                dustPosition.x - static_cast<float>(dustTexture.width) / (2.0f * DUST_FRAME_COUNT),
                dustPosition.y - static_cast<float>(dustTexture.height) / 2.0f
        };
        DrawTextureRec(dustTexture, dustSourceRec, dustDrawPos, WHITE);
    }
}

void maincharacter::drawsoul() {
    std::string entityType = "soul";
    Texture2D bodyTexture;

    if (currentState == AnimationState::DUST) {
        // Use the soul_dust_* textures for the body during dust attack
        bodyTexture = assestmanagergraphics::getAnimationTexture(entityType, AnimationState::DUST, currentDirection);
    } else {
        bodyTexture = assestmanagergraphics::getAnimationTexture(entityType, currentState, currentDirection);
    }

    int frameWidth = bodyTexture.width / 8;  // Assuming 8 frames per animation
    int frameHeight = bodyTexture.height;

    Rectangle sourceRect = {
            static_cast<float>(currentFrame * frameWidth),
            0.0f,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
    };
    Rectangle destRect = {
            position.x,
            position.y,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
    };
    Vector2 origin = {
            static_cast<float>(frameWidth) / 2.0f,
            static_cast<float>(frameHeight) / 2.0f
    };

    // Always draw the character body
    DrawTexturePro(bodyTexture, sourceRect, destRect, origin, 0.0f, WHITE);

    // Draw dust effect if active
    if (currentState == AnimationState::DUST) {
        Texture2D dustTexture = assestmanagergraphics::getAnimationTexture("dust", AnimationState::DUST, currentDirection);

        int dustFrameWidth = dustTexture.width / DUST_FRAME_COUNT;
        int dustFrameHeight = dustTexture.height;
        Rectangle dustSourceRect = {
                static_cast<float>(currentFrame * dustFrameWidth),
                0.0f,
                static_cast<float>(dustFrameWidth),
                static_cast<float>(dustFrameHeight)
        };

        // Calculate dust effect position based on facing direction
        Vector2 dustOffset;
        switch (currentDirection) {
            case Direction::Up:    dustOffset = {0, -static_cast<float>(frameHeight)/4.0f}; break;
            case Direction::Down:  dustOffset = {0, static_cast<float>(frameHeight)/4.0f}; break;
            case Direction::Left:  dustOffset = {-static_cast<float>(frameWidth)/4.0f, 0}; break;
            case Direction::Right: dustOffset = {static_cast<float>(frameWidth)/4.0f, 0}; break;
        }

        Rectangle dustDestRect = {
                position.x + dustOffset.x,
                position.y + dustOffset.y,
                static_cast<float>(dustFrameWidth),
                static_cast<float>(dustFrameHeight)
        };
        Vector2 dustOrigin = {
                static_cast<float>(dustFrameWidth) / 2.0f,
                static_cast<float>(dustFrameHeight) / 2.0f
        };

        DrawTexturePro(dustTexture, dustSourceRect, dustDestRect, dustOrigin, 0.0f, WHITE);
    }

    // Debug drawing
    DrawRectangleLines(
            static_cast<int>(position.x - frameWidth / 2),
            static_cast<int>(position.y - frameHeight / 2),
            frameWidth,
            frameHeight,
            RED
    );
    DrawText(
            TextFormat("Frame: %d, State: %d", currentFrame, static_cast<int>(currentState)),
            static_cast<int>(position.x) - 50,
            static_cast<int>(position.y) + 40,
            10,
            RED
    );

    if (souldustcanbeused()) {
        DrawText("Press L to use Soul Dust", position.x - 50, position.y - 40, 10, YELLOW);
    }
}

void maincharacter::drawrobot() {
    std::string entityType = "robot";  // Explicitly set the entity type
    Texture2D bodyTexture = assestmanagergraphics::getAnimationTexture(entityType, currentState, currentDirection);

    std::cout << "Drawing robot. Texture ID: " << bodyTexture.id << std::endl;


    int frameWidth = bodyTexture.width / 8;  //  8 frames per animation
    int frameHeight = bodyTexture.height;

    Rectangle sourceRect = {
            static_cast<float>(currentFrame * frameWidth),
            0.0f,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
    };
    Rectangle destRect = {
            position.x,
            position.y,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
    };
    Vector2 origin = {
            static_cast<float>(frameWidth) / 2.0f,
            static_cast<float>(frameHeight) / 2.0f
    };

    // Draw only one body texture
    DrawTexturePro(bodyTexture, sourceRect, destRect, origin, 0.0f, WHITE);

    // Draw debug info
    DrawRectangleLines(
            static_cast<int>(position.x - frameWidth / 2),
            static_cast<int>(position.y - frameHeight / 2),
            frameWidth,
            frameHeight,
            RED
    );
    DrawText(
            TextFormat("Frame: %d, State: %d", currentFrame, static_cast<int>(currentState)),
            static_cast<int>(position.x) - 50,
            static_cast<int>(position.y) + 40,
            10,
            RED
    );
}

Rectangle maincharacter::getCollisionRectangle() const {
    return Rectangle{position.x - size / 2, position.y - size / 2, size, size};
}

Texture2D maincharacter::getCurrentTexture() {
    std::string entityType = (currentmodus == soulmodus) ? "soul" : "robot";
    AnimationState state = currentState;
    Direction direction = currentDirection;

    if (currentState == AnimationState::SWITCH) {
        return assestmanagergraphics::getAnimationTexture(
                entityType,
                AnimationState::SWITCH,
                Direction::Down  // Switch animation doesn't have directions
        );
    }

    Texture2D texture = assestmanagergraphics::getAnimationTexture(entityType, state, direction);

    // Print debug information
    std::cout << "Getting texture for: " << entityType << ", State: " << static_cast<int>(state)
              << ", Direction: " << static_cast<int>(direction) << std::endl;
    std::cout << "Texture ID: " << texture.id << std::endl;

    return texture;
}

int getHealth(const maincharacter &maincharacter) {
    return maincharacter.healthManager.getCurrentHealth();
}

bool maincharacter::isCharacterPossessed() const {
    return isPossessed;
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


void maincharacter::setPossessionStatus(bool possessed) {
    isPossessed = possessed;
}

void maincharacter::startDusting() {
    if (souldustcanbeused()) {
        isDusting = true;
        dustAnimationTimer = 0.0f;
        dustPosition = position;
        currentState = AnimationState::DUST; //  DUST state in enum.h
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
        dashProgress = 1.0f; //ensures dash ends
    }


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
    if (souldustcanbeused() && currentState == AnimationState::DUST) {
        auto [bowlX, bowlY] = _scene->getNearestFirebowlTile(position);
        if (bowlX != -1 && bowlY != -1 && !_scene->isFirebowlActivated(bowlX, bowlY)) {
            _scene->activateFirebowl(bowlX, bowlY);
        }
    }
}

void maincharacter::throwBomb() {
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

void maincharacter::update() {
    Vector2 oldPosition = position;
    maincharacterwalking();

    if (!healthManager.isAlive()) {
        // Handle game over state
        return; // Exit the update function early
    }


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

            //*NEW CODE*
            if (IsKeyPressed(KEY_L) && souldustcanbeused()) {
                currentState = AnimationState::DUST;
                currentFrame = 0;
                dustAnimationTimer = 0.0f;
            }

            if (currentState == AnimationState::DUST) {
                dustAnimationTimer += GetFrameTime();
                if (dustAnimationTimer >= FRAME_DURATION) {
                    dustAnimationTimer -= FRAME_DURATION;
                    currentFrame++;
                    if (currentFrame >= DUST_FRAME_COUNT) {
                        currentState = AnimationState::IDLE;
                        currentFrame = 0;
                    }
                }
            }

            souldust();
            break;

        case robotmodus:
            if (IsKeyPressed(KEY_SPACE) && !isSwitching) {
                isSwitching = true;
                switchAnimationTimer = 0.0f;
                currentState = AnimationState::SWITCH;
            }

            if (IsKeyPressed(KEY_U) && !isSwitching) {
                if (_scene->isAdjacentToSwitch(position)) {
                    Vector2 characterTile = {std::floor(position.x / 32), std::floor(position.y / 32)};
                    _scene->toggleSwitchAt(characterTile);
                }
            }
            if (IsKeyPressed(KEY_J) && (GetTime() - lastBombThrowTime) >= bomb_cooldown && !isSwitching) {
                throwBomb();
            }

            if (IsKeyPressed(KEY_U) && GetTime() - lastAttackTime >= ATTACK_COOLDOWN) {
                std::cout << "Initiating melee attack" << std::endl;
                performMeleeAttack();
                currentState = AnimationState::ATTACK_NORMAL;
            }
            break;
    }

    if (currentmodus == robotmodus) {
        if (IsKeyPressed(KEY_U) && GetTime() - lastAttackTime >= ATTACK_COOLDOWN) {
            std::cout << "Initiating melee attack" << std::endl;
            performMeleeAttack();
            currentState = AnimationState::ATTACK_NORMAL;
            std::cout << "State set to ATTACK in update: " << static_cast<int>(currentState) << std::endl;
        }
    }

    if (currentState == AnimationState::ATTACK_NORMAL) {
        attackTimer += GetFrameTime();
        std::cout << "Attack timer: " << attackTimer << std::endl;
        if (attackTimer >= ATTACK_DURATION) {
            std::cout << "Attack animation finished" << std::endl;
            currentState = AnimationState::IDLE;
            attackTimer = 0.0f;
            currentAnimationState = AnimationState::IDLE;
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
        std::cout << "Before state update. Current state: " << static_cast<int>(currentState) << std::endl;
        if (Vector2Equals(oldPosition, position) && currentState != AnimationState::DASH && currentState != AnimationState::ATTACK_NORMAL && currentState != AnimationState::DUST) {
            currentState = AnimationState::IDLE;
            std::cout << "State set to IDLE" << std::endl;
        } else if (currentState != AnimationState::DASH && currentState != AnimationState::ATTACK_NORMAL && currentState != AnimationState::DUST) {
            currentState = AnimationState::WALK;
            std::cout << "State set to WALK" << std::endl;
        }
        std::cout << "After state update. Current state: " << static_cast<int>(currentState) << std::endl;

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

    std::cout << "End of update. Current state: " << static_cast<int>(currentState)
              << ", Current mode: " << (currentmodus == robotmodus ? "Robot" : "Soul") << std::endl;

    //update timers
    float deltaTime = GetFrameTime();
    if (immunityTimer > 0.0f) {
        immunityTimer -= deltaTime;
    }

    //update abyss message timer
    if (abyssMessageTimer > 0.0f) {
        abyssMessageTimer -= GetFrameTime();
    }

    AnimationState oldState;
    if (oldState != currentState) {
        std::cout << "State changed from " << static_cast<int>(oldState)
                  << " to " << static_cast<int>(currentState) << std::endl;
    }

}


void maincharacter::updateAnimation(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= FRAME_DURATION) {
        frameCounter = 0.0f;
        currentFrame = (currentFrame + 1) % FRAME_COUNT;
    }

    // Synchronize attack animation
    if (currentState == AnimationState::ATTACK_NORMAL || currentState == AnimationState::ATTACK_RANGED) {
        currentFrame = static_cast<int>((attackTimer / ATTACK_DURATION) * FRAME_COUNT) % FRAME_COUNT;
    }
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

void maincharacter::updateDustAnimation(float deltaTime) {
    if (isDusting) {
        dustAnimationTimer += deltaTime;
        if (dustAnimationTimer >= DUST_ANIMATION_DURATION) {
            isDusting = false;
            currentState = AnimationState::IDLE; // Or diffrent state
        }
    }
}

void maincharacter::updateLastSafePosition() {
    if (!_scene->touchesAbyss(position, size)) {
        lastSafePosition = position;
    }
}

void maincharacter::performMeleeAttack() {
    std::cout << "Performing melee attack!" << std::endl;
    std::cout << "Current state before attack: " << static_cast<int>(currentState) << std::endl;
    currentAnimationState = AnimationState::ATTACK_NORMAL;
    currentState = AnimationState::ATTACK_NORMAL;
    std::cout << "Current state after setting to ATTACK: " << static_cast<int>(currentState) << std::endl;
    attackTimer = 0.0f;
    lastAttackTime = GetTime();

    // Check for enemies in range and apply damage
    for (auto& enemy : _scene->getEnemies()) {
        if (CheckCollisionCircles(position, size + 32.0f, enemy->position, enemy->size)) {
            enemy->takeDamage(2); // Apply 2 damage to the enemy
            std::cout << "Hit enemy!" << std::endl;
        }
    }
}

