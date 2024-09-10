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
        std::string direction;
        switch (currentDirection) {
            case Direction::Up:
                direction = "back";
                break;
            case Direction::Down:
                direction = "front";
                break;
            case Direction::Left:
                direction = "side left";
                break;
            case Direction::Right:
                direction = "side right";
                break;
        }

        std::string characterDustTextureName = "Character - Soul - Dust " + direction + " - character - animated";
        std::string dustEffectTextureName = "Character - Soul - Dust " + direction + " - dust - animated";

        Texture2D characterDustTexture = assestmanagergraphics::getCharacterTexture("soul", characterDustTextureName);
        Texture2D dustEffectTexture = assestmanagergraphics::getCharacterTexture("soul", dustEffectTextureName);

        int currentFrame = static_cast<int>((dustAnimationTimer / DUST_ANIMATION_DURATION) * DUST_FRAME_COUNT) % DUST_FRAME_COUNT;

        // Draw character dust animation
        Rectangle characterSourceRec = {
                static_cast<float>(currentFrame * characterDustTexture.width / DUST_FRAME_COUNT), 0.0f,
                static_cast<float>(characterDustTexture.width / DUST_FRAME_COUNT),
                static_cast<float>(characterDustTexture.height)
        };
        Vector2 characterDrawPos = {
                position.x - static_cast<float>(characterDustTexture.width) / (2.0f * DUST_FRAME_COUNT),
                position.y - static_cast<float>(characterDustTexture.height) / 2.0f
        };
        DrawTextureRec(characterDustTexture, characterSourceRec, characterDrawPos, WHITE);

        // Draw dust effect animation
        Rectangle dustSourceRec = {
                static_cast<float>(currentFrame * dustEffectTexture.width / DUST_FRAME_COUNT), 0.0f,
                static_cast<float>(dustEffectTexture.width / DUST_FRAME_COUNT),
                static_cast<float>(dustEffectTexture.height)
        };
        Vector2 dustDrawPos = {
                dustPosition.x - static_cast<float>(dustEffectTexture.width) / (2.0f * DUST_FRAME_COUNT),
                dustPosition.y - static_cast<float>(dustEffectTexture.height) / 2.0f
        };
        DrawTextureRec(dustEffectTexture, dustSourceRec, dustDrawPos, WHITE);
    }
}

void maincharacter::drawsoul() {
    Texture2D currentTexture = getCurrentTexture();

    if (currentState == AnimationState::DUST) {
        // Draw Soul Dust animation
        std::string direction;
        switch (currentDirection) {
            case Direction::Up: direction = "back"; break;
            case Direction::Down: direction = "front"; break;
            case Direction::Left: direction = "side left"; break;
            case Direction::Right: direction = "side right"; break;
        }

        std::string characterDustTextureName = "Character - Soul - Dust " + direction + " - character - animated";
        std::string dustEffectTextureName = "Character - Soul - Dust " + direction + " - dust - animated";

        Texture2D characterDustTexture = assestmanagergraphics::getCharacterTexture("soul", characterDustTextureName);
        Texture2D dustEffectTexture = assestmanagergraphics::getCharacterTexture("soul", dustEffectTextureName);

        int frameWidth = characterDustTexture.width / DUST_FRAME_COUNT;
        Rectangle sourceRec = {
                static_cast<float>(currentFrame * frameWidth), 0.0f,
                static_cast<float>(frameWidth),
                static_cast<float>(characterDustTexture.height)
        };
        Vector2 origin = { frameWidth / 2.0f, characterDustTexture.height / 2.0f };

        // Draw character dust animation
        DrawTexturePro(characterDustTexture, sourceRec,
                       (Rectangle){ position.x, position.y, static_cast<float>(frameWidth), static_cast<float>(characterDustTexture.height) },
                       origin, 0, WHITE);

        // Draw dust effect animation
        DrawTexturePro(dustEffectTexture, sourceRec,
                       (Rectangle){ position.x, position.y, static_cast<float>(frameWidth), static_cast<float>(dustEffectTexture.height) },
                       origin, 0, WHITE);

        std::cout << "Drawing Soul Dust animation. Frame: " << currentFrame << std::endl;
    } else {
        // Draw normal soul animation
        DrawTextureRec(currentTexture, frameRec,
                       {position.x - frameRec.width / 2, position.y - frameRec.height / 2},
                       WHITE);
    }

    if (souldustcanbeused()) {
        DrawText("Press L to use Soul Dust", position.x - 50, position.y - 40, 10, YELLOW);
    }

    // Debug information
    DrawText(TextFormat("State: %d", static_cast<int>(currentState)), position.x - 50, position.y + 40, 10, RED);
    DrawText(TextFormat("Frame: %d", currentFrame), position.x - 50, position.y + 55, 10, RED);
}

void maincharacter::drawrobot() {
    std::cout << "Drawing robot. Current state: " << static_cast<int>(currentState) << std::endl;

    if (currentState == AnimationState::ATTACK) {
        std::cout << "Drawing attack animation" << std::endl;
        std::string direction;
        switch (currentDirection) {
            case Direction::Up: direction = "back"; break;
            case Direction::Down: direction = "front"; break;
            case Direction::Left: direction = "side left"; break;
            case Direction::Right: direction = "side right"; break;
        }

        std::string bodyAnimName = "Character - Robot - Melee " + direction;
        std::string armAnimName = bodyAnimName;

        if (direction == "side left" || direction == "side right") {
            bodyAnimName += " - Body";
            armAnimName += " - Arm";
        }

        bodyAnimName += " - animated";
        armAnimName += " - animated";

        std::cout << "Requesting body texture: " << bodyAnimName << std::endl;
        Texture2D bodyTexture = assestmanagergraphics::getCharacterTexture("robot", bodyAnimName);
        std::cout << "Requesting arm texture: " << armAnimName << std::endl;
        Texture2D armTexture = assestmanagergraphics::getCharacterTexture("robot", armAnimName);

        int frame = static_cast<int>((attackTimer / ATTACK_DURATION) * 8) % 8;
        Rectangle sourceRect = { frame * 32.0f, 0, 32, 32 };
        Vector2 origin = { 16, 32 };  // Adjust origin to bottom center of sprite

        // Adjust the position to center the animation on the character's feet
        Vector2 drawPos = { position.x, position.y };

        if (bodyTexture.id != 0) {
            std::cout << "Drawing body texture" << std::endl;
            DrawTexturePro(bodyTexture, sourceRect, (Rectangle){ drawPos.x, drawPos.y, 32, 32 }, origin, 0, WHITE);
        } else {
            std::cout << "Drawing error rectangle for body" << std::endl;
            DrawRectangle(drawPos.x - 16, drawPos.y - 32, 32, 32, PINK);
            DrawRectangleLines(drawPos.x - 16, drawPos.y - 32, 32, 32, BLACK);
        }

        if (armTexture.id != 0 && (direction == "side left" || direction == "side right")) {
            std::cout << "Drawing arm texture" << std::endl;
            DrawTexturePro(armTexture, sourceRect, (Rectangle){ drawPos.x, drawPos.y, 32, 32 }, origin, 0, WHITE);
        }
    } else {
        std::cout << "Drawing normal robot texture" << std::endl;
        Texture2D currentTexture = getCurrentTexture();
        if (currentTexture.id != 0) {
            Rectangle sourceRect = { currentFrame * 32.0f, 0, 32, 32 };
            Vector2 drawPos = { position.x - 16, position.y - 32 };  // Adjust to draw from top-left corner
            DrawTextureRec(currentTexture, sourceRect, drawPos, WHITE);
        } else {
            std::cout << "Drawing error rectangle for normal texture" << std::endl;
            DrawRectangle(position.x - 16, position.y - 32, 32, 32, PINK);
            DrawRectangleLines(position.x - 16, position.y - 32, 32, 32, BLACK);
        }
    }
}

Rectangle maincharacter::getCollisionRectangle() const {
    return Rectangle{position.x - size / 2, position.y - size / 2, size, size};
}

Texture2D maincharacter::getCurrentTexture() {
    std::string state;
    std::string key;

    switch (currentState) {
        case AnimationState::IDLE:
            state = "Idle";
            break;
        case AnimationState::WALK:
            state = "Walk";
            break;
        case AnimationState::DASH:
            state = "Dash";
            break;
        case AnimationState::DUST:
            state = "Dust";
            break;
        case AnimationState::ATTACK:
            state = "Melee";
            break;
        case AnimationState::SWITCH:
            return assestmanagergraphics::getCharacterTexture(
                    "Switch-Animation",
                    (currentmodus == soulmodus)
                    ? "Character - Robot+Soul - Switch Soul to Robot - animated"
                    : "Character - Robot+Soul - Switch Robot to Soul - animated"
            );
        default:
            state = "Idle";
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
            direction = "side left";
            break;
        case Direction::Right:
            direction = "side right";
            break;
    }

    std::string characterName = (currentmodus == soulmodus) ? "soul" : "robot";

    std::string animationName = std::string("Character - ") +
                                (characterName == "soul" ? "Soul" : "Robot") +
                                " - " + state + " " + direction;

    if (characterName == "robot") {
        if (state == "Idle" || state == "Walk") {
            if (direction == "front" || direction == "side left" || direction == "side right") {
                animationName += isCharacterPossessed() ? " - possessed" : " - normal";
            }
        } else if (state == "Melee" && (direction == "side left" || direction == "side right")) {
            animationName += " - Body";
        }
    } else if (characterName == "soul" && state == "Dust") {
        animationName += " - character";
    }

    animationName += " - animated";

    std::cout << "Requesting texture: " << characterName << " - " << animationName << std::endl;
    return assestmanagergraphics::getCharacterTexture(characterName, animationName);
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

            if (IsKeyPressed(KEY_B) && !isSwitching) {
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
                currentState = AnimationState::ATTACK;
            }
            break;
    }

    if (currentmodus == robotmodus) {
        if (IsKeyPressed(KEY_U) && GetTime() - lastAttackTime >= ATTACK_COOLDOWN) {
            std::cout << "Initiating melee attack" << std::endl;
            performMeleeAttack();
            currentState = AnimationState::ATTACK;
            std::cout << "State set to ATTACK in update: " << static_cast<int>(currentState) << std::endl;
        }
    }

    if (currentState == AnimationState::ATTACK) {
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
        if (Vector2Equals(oldPosition, position) && currentState != AnimationState::DASH && currentState != AnimationState::ATTACK && currentState != AnimationState::DUST) {
            currentState = AnimationState::IDLE;
            std::cout << "State set to IDLE" << std::endl;
        } else if (currentState != AnimationState::DASH && currentState != AnimationState::ATTACK && currentState != AnimationState::DUST) {
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
}


void maincharacter::updateAnimation(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= FRAME_DURATION) {
        frameCounter = 0.0f;
        currentFrame++;
        if (currentFrame >= FRAME_COUNT) currentFrame = 0;
    }

    // Updates frameRec based on current frame
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
    currentAnimationState = AnimationState::ATTACK;
    currentState = AnimationState::ATTACK;
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

