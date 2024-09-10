//
// Created by lrfri on 14.05.2024.
//
#include "gameplay.h"
#include "tileson.hpp"
#include "raymath.h"
#include "raylib.h"
#include "mainmenu.h"
#include "pausescreen.h"
#include "journal.h"
#include "ingameoptions.h"
#include "maincharacter.h"
#include "maincharactermodus.h"
#include "ENEMIES/Enemy.h"
#include "ENEMIES/Enemy1.h"
#include "ENEMIES/Enemy2.h"
#include "ENEMIES/Enemy3.h"
#include "GAME OBJECTS/gameobjects.h"
#include "GAME OBJECTS/robot.h"
#include "maincharactermodus.h"
#include "maincharacter.h"
#include "Wall.h"
#include "GAME OBJECTS/yellowblock.h"
#include "GAME OBJECTS/blueblock.h"
//test

void gameplay::update() {

// Check maincharacter health
    if (!themaincharacter->healthManager.isAlive()) {
        // Draw game over message
        const char* gameOverText = "GAME OVER";
        int fontSize = 40;
        int textWidth = MeasureText(gameOverText, fontSize);
        DrawText(gameOverText, GetScreenWidth()/2 - textWidth/2, GetScreenHeight()/2 - fontSize/2, fontSize, RED);
        return; // Stop updating the game
    }

    // Update and check enemies
    for (auto it = enemies.begin(); it != enemies.end();) {
        Enemy* enemy = *it;
        if (!enemy->healthManager.isAlive()) {
            // Handle enemy death
            delete enemy; // Free the memory
            it = enemies.erase(it); // Remove from the vector and get the next iterator
        } else {
            enemy->update(); // Update the enemy if it's alive
            ++it;
        }
    }



    updateAllenemies();
    for (int i = 0; i < gameobjects.size(); i++) {
        gameobjects[i]->update();
    }

    //switch between soul in soul-mode and soul in robot-mode
    if (IsKeyPressed(KEY_SPACE)) {
        switch (currentmodus) {
            case soulmodus:
                if (soulcantakeover()) {
                    currentmodus = robotmodus;
                    soulhasntchangedformsyet = false;
                    soulleavesrobot = false;
                    soulentersrobot = true;
                }
                break;
            case robotmodus:
                currentmodus = soulmodus;
                soulleavesrobot = true;
                soulentersrobot = false;
                break;
        }
    }

    //bombs
    for (auto it = activeBombs.begin(); it != activeBombs.end();) {
        (*it)->update();
        if ((*it)->state == bombs::finished) {
            delete *it;
            it = activeBombs.erase(it);
        } else {
            ++it;
        }
    }

    //enables room-switch and checks which version of the character is the one leaving the room
    doRoomSwitch();


    themaincharacter->update();
    therobot->update();
    updateStones();
    updateBlocks();
    updateSwitches();

}

void gameplay::doRoomSwitch() {
    switch (room) {
        case 1:
            if (themaincharacter->position.y <= (doorfromroom1to2)) {
                if (areAllFirebowlsActivatedInRoom(1)) {
                    showDoorIsLockedMessage = false;
                    room = 2;
                    if (currentmodus == soulmodus) {
                        soulisinroom = 2;
                    } else {
                        robotisinroom = 2;
                    }
                    themaincharacter->position.y = startposroom1to2;
                    reloadRoom();
                }
            } else if (themaincharacter->position.y <= (doortextarea)&&!(areAllFirebowlsActivatedInRoom(1))) {
                showDoorIsLockedMessage = true;
            } else {
                showDoorIsLockedMessage = false;
            }
            break;
        case 2:
            if (themaincharacter->position.y <= doorfromroom2to3) {
                room = 3;
                if (currentmodus == soulmodus) {
                    soulisinroom = 3;
                } else {
                    robotisinroom = 3;
                }

                reloadRoom();

                themaincharacter->position.
                        y = startposroom2to3;
            }
            if (themaincharacter->position.x >= doorfromroom2to4) {
                room = 4;
                if (currentmodus == soulmodus) {
                    soulisinroom = 4;
                } else {
                    robotisinroom = 4;
                }

                reloadRoom();

                themaincharacter->position.
                        x = startposroom2to4;
            }
            if (themaincharacter->position.y >= (doorfromroom2to1)) {
                room = 1;
                if (currentmodus == soulmodus) {
                    soulisinroom = 1;
                } else {
                    robotisinroom = 1;
                }
                hasbeeninroom1before = true;

                reloadRoom();

                themaincharacter->position.
                        y = startposroom2to1;
            }
            break;
        case 3:
            if (themaincharacter->position.y >= (doorfromroom3to2)) {
                room = 2;
                if (currentmodus == soulmodus) {
                    soulisinroom = 2;
                } else {
                    robotisinroom = 2;
                }

                reloadRoom();

                themaincharacter->position.
                        y = startposroom3to2;
            } else if (themaincharacter->position.y <= (doorfromroom4to5)) {
                if (areAllFirebowlsActivatedInRoom(3)) {
                    room = 5;
                    if (currentmodus == soulmodus) {
                        soulisinroom = 5;
                    } else {
                        robotisinroom = 5;
                    }

                    reloadRoom();

                    themaincharacter->position.
                            y = startposroom4to5;
                    showDoorIsLockedMessage = false;
                } else if (themaincharacter->position.y <= doortextarea) {
                    showDoorIsLockedMessage = true;
                }
            } else {
                showDoorIsLockedMessage = false;
            }
            break;
        case 4:
            if (themaincharacter->position.x <= (doorfromroom4to2)) {
                room = 2;
                if (currentmodus == soulmodus) {
                    soulisinroom = 2;
                } else {
                    robotisinroom = 2;
                }

                reloadRoom();

                themaincharacter->position.
                        x = startposroom4to2;
            }
            break;
        case 5:
            if (themaincharacter->position.y >= doorfromroom5to4) {
                room = 3;
                if (currentmodus == soulmodus) {
                    soulisinroom = 3;
                } else {
                    robotisinroom = 3;
                }

                reloadRoom();

                themaincharacter->position.
                        y = startposroom5to4;
            } else if (themaincharacter->position.x <= doorfromroom5to7 ||
                       themaincharacter->position.y <= doorfromroom5to6) {
                showDemoMessage = true;
            } else {
                showDemoMessage = false;
            }
            break;
/*case 6:
    if (themaincharacter->position.y >= doorfromroom6to5) {
        room = 5;
        if (currentmodus == soulmodus) {
            soulisinroom = 6;
        } else { robotisinroom = 6; }
        reloadRoom();
        themaincharacter->position.y = startposroom6to5;
    }
    break;*/
    }

}

bool gameplay::isAdjacentToFirebowl(Vector2 pos) const {
    int tileX = pos.x / 32;
    int tileY = pos.y / 32;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; // Skip the tile the character is on
            int checkX = tileX + dx;
            int checkY = tileY + dy;
            if (room == 1) {
                if (getTileAt(checkX * 32, checkY * 32) == firebowl0ID) {
                    return true;
                }
            } else {
                if (getTileAt(checkX * 32, checkY * 32) == firebowl1ID) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::pair<int, int> gameplay::getNearestFirebowlTile(Vector2 pos) const {
    int tileX = pos.x / 32;
    int tileY = pos.y / 32;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int checkX = tileX + dx;
            int checkY = tileY + dy;
            if (getTileAt(checkX * 32, checkY * 32) == firebowl0ID ||
                getTileAt(checkX * 32, checkY * 32) == firebowl1ID) {
                return {checkX, checkY};
            }
        }
    }
    return {-1, -1}; // Return invalid coordinates if no firebowl is found
}

void gameplay::activateFirebowl(int x, int y) {
    if (!isFirebowlActivated(x, y)) {
        activatedFirebowls.push_back({x, y, room});
        activeFirebowlAnimations.push_back({x, y, 0.0f, 0, room});
    }
}

bool gameplay::areAllFirebowlsActivatedInRoom(int roomNumber) const {
    int requiredCount = 0;
    if (roomNumber == 1) {
        requiredCount = 2;  // Room 1 requires 2 firebowls
    } else if (roomNumber == 3) {
        requiredCount = 1;  // Room 3 requires only 1 firebowl
    }
    //conditions for other rooms if needed HERE

    int activatedCount = 0;
    for (const auto &bowl: activatedFirebowls) {
        if (bowl.room == roomNumber) {
            activatedCount++;
        }
    }
    return activatedCount >= requiredCount;
}

bool gameplay::isFirebowlActivated(int x, int y) const {
    for (const auto &bowl: activatedFirebowls) {
        if (bowl.x == x && bowl.y == y && bowl.room == room) {
            return true;
        }
    }
    return false;
}

void gameplay::updateAllenemies() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i]->update();
    }
}

scene *gameplay::evaluateSceneChange() {
    if (IsKeyPressed(KEY_M)) {
        return new mainmenu();
    } else if (IsKeyPressed(KEY_P)) {
        return new pausescreen(this);
    } else if (IsKeyPressed(KEY_T)) {
        return new journal(this);
    } else if (IsKeyPressed(KEY_O)) {
        return new ingameoptions(this);
    }
    return this;
}

void gameplay::draw() {
    ClearBackground(GRAY);

    //draws the map
    int tilesetCols = 1;
    int tileSize = 32;

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int data = tiles[y * mapWidth + x];
            if (data < 0) continue;
            Rectangle src = {(float) (data % tilesetCols * tileSize), (float) (data / tilesetCols * tileSize),
                             (float) (tileSize), (float) (tileSize)};
            Rectangle dest = {(float) (x * tileSize), (float) (y * tileSize), (float) (tileSize), (float) (tileSize)};
            //hier kann man "static_cast<float>" durch (float) ersetzen -> ist C, aber geht hier auch
            //DrawTexturePro(tilesetgrass, src, dest, {}, 0, WHITE);
            DrawTexturePro(tileset_final, src, dest, {}, 0, WHITE);


        }

        for (Enemy *enemy: enemies) {
            enemy->draw();
        }
    }
    themaincharacter->draw();
    drawmaincharacter();

    for (int i = 0; i < gameobjects.size(); i++) {
        gameobjects[i]->draw();
    }

    drawtextonscreen();
    drawhealthhearts();
    drawicons();
    drawStones();
    drawBlocks();
    drawSwitches();
    drawActivatedFirebowls(GetFrameTime());
    if (showDoorIsLockedMessage) {
        if (room == 1) {
            DrawText("This door is locked!\nActivate the firebowls to open.", 9 * 32, 4 * 32, 15, DARKBLUE);
        }
        if (room == 3) {
            DrawText("This door is locked!\nActivate the firebowl to open.", 9 * 32 + 16, 5 * 32 + 16, 15, WHITE);
        }
    }

    if (IsKeyDown(KEY_H)) {
        this->drawDebug();
    }
    //bombs
    for (auto &bomb: activeBombs) {
        bomb->draw();
    }
}

void gameplay::drawActivatedFirebowls(float deltaTime) {
    for (auto &bowl: activeFirebowlAnimations) {
        if (bowl.room == room) {  // Only draw if in current room
            int frameWidth = activatedFirebowlTexture.width / 8;
            Rectangle srcRect = {static_cast<float>(bowl.currentFrame * frameWidth), 0, static_cast<float>(frameWidth),
                                 static_cast<float>(activatedFirebowlTexture.height)};
            DrawTexturePro(activatedFirebowlTexture, srcRect,
                           {static_cast<float>(bowl.x * 32), static_cast<float>(bowl.y * 32), 32, 32}, {0, 0}, 0,
                           WHITE);

            // Update animation
            bowl.animationTimer += deltaTime;
            if (bowl.animationTimer >= activatedFirebowlAnimationSpeed) {
                bowl.currentFrame = (bowl.currentFrame + 1) % 8;
                bowl.animationTimer -= activatedFirebowlAnimationSpeed;
            }
        }
    }
}

void gameplay::drawmaincharacter() {
    //draws the character (in the correct form)
    switch (currentmodus) {
        case soulmodus:
            themaincharacter->drawsoul();
            break;
        case robotmodus:
            themaincharacter->drawrobot();
            break;
    }

    //only shows inactive robot when soul is in soul mode, also enables soul to leave robot behind and go to other rooms alone
    if (robotisinroom == room && currentmodus == soulmodus) {
        therobot->draw();
    }

    //updates the position of the inactive robot to the position where the soul leaves the robot
    //also shows inactive robot at the start of the game
    if (soulleavesrobot || soulhasntchangedformsyet) {
        therobot->position = {themaincharacter->position.x, themaincharacter->position.y};
        soulleavesrobot = false;
        soulhasntchangedformsyet = false;
    }
}

void gameplay::drawtextonscreen() {
    DrawText("Press O to go to options.", 10, 400, 10, WHITE);
    DrawText("Press P to pause the game.", 10, 420, 10, WHITE);
    DrawText("Press M to go back to the main menu.", 10, 440, 10, WHITE);
    DrawText("Press T to open the journal.", 10, 460, 10, WHITE);

    if (showDemoMessage == true) {
        DrawText("This is the end of the SoulSteel Demo version.", 200, 120, 20, WHITE);
    }
}

void gameplay::drawicons() {
    DrawTexture(icon_dash, 23 * 32, 16, WHITE);
    DrawTexture(icon_dust, 23 * 32 + 16, 32, WHITE);
    DrawTexture(icon_bomb, 23 * 32 - 16, 32, WHITE);
    DrawTexture(icon_stone, 23 * 32, 32 + 16, WHITE);
}

void gameplay::drawhealthhearts() {
    switch (currentmodus) {
        case soulmodus:
            DrawTexture(heart_soul, 10, 20, WHITE);
            DrawTexture(heart_soul, 50, 20, WHITE);
            DrawTexture(heart_soul, 90, 20, WHITE);
            break;
        case robotmodus:
            DrawTexture(heart_robot, 10, 20, WHITE);
            DrawTexture(heart_robot, 50, 20, WHITE);
            DrawTexture(heart_robot, 90, 20, WHITE);
    }
}

void gameplay::drawDebug() { //draws red outlines around the wall tiles for example to help detect collision problems
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (getTileAt(x * 32.0, y * 32.0) == 1) {
                DrawRectangleLines(x * 32, y * 32, 32, 32, RED);
            }
        }
    }
}

//gets the data from the map needed to draw it
gameplay::gameplay() : scene(this) {
    //std::cout << "Gameplay instance created\n";
    tson::Tileson tileson;
    themaincharacter = new class maincharacter(this);
    therobot = new robot(this);
    reloadRoom();
}

void gameplay::reloadRoom() {
    loadMap();
    enemies.clear();
    resetStonesToInitialPositions();
    //deactivateFirebowls(); needed if you want the firebowls to deactive when leaving the room -> you have to turn them on again when you are back in that room

    //stones
    if (stonesInRooms.find(room) == stonesInRooms.end()) {
        switch (room) {
            case 2:
                spawnStone(2, {23 * 32, 8 * 32});

                spawnStone(2, {10 * 32, 3 * 32});
                spawnStone(2, {11 * 32, 4 * 32});
                spawnStone(2, {12 * 32, 3 * 32});
                spawnStone(2, {13 * 32, 4 * 32});
                spawnStone(2, {14 * 32, 3 * 32});
                break;
                // Add cases for other rooms if needed
        }
    }
    //blocks
    // Only spawn blocks and switches if they haven't been created for this room before
    if (blocksInRooms.find(room) == blocksInRooms.end()) {
        switch (room) {
            case 3:
                spawnBlock({2 * 32, 3 * 32}, false);  // Blue block top left corner 1
                spawnBlock({1 * 32, 4 * 32}, false);  // Blue block top left corner 2
                spawnBlock({12 * 32, 3 * 32}, false);  // Blue block top middle

                spawnBlock({11 * 32, 3 * 32}, true);  // Yellow block top middle top line 1
                spawnBlock({13 * 32, 3 * 32}, true);  // Yellow block top middle top line 2
                spawnBlock({11 * 32, 4 * 32}, true);  // Yellow block top middle second line 1
                spawnBlock({12 * 32, 4 * 32}, true);  // Yellow block top middle second line 2
                spawnBlock({13 * 32, 4 * 32}, true);  // Yellow block top middle second line 3

                spawnBlock({17 * 32, 7 * 32}, true);  // Yellow block middle by fence 1
                spawnBlock({17 * 32, 8 * 32}, true);  // Yellow block middle by fence 2

                spawnSwitch({1 * 32, 3 * 32}, true);  // Yellow switch
                spawnSwitch({22 * 32, 12 * 32}, false);  // Blue switch
                //std::cout << "Switches spawned at (1, 3) and (22, 12)" << std::endl;
                break;
        }
    }
    //enemies
    switch (room) {
        case 1:

            break;
        case 2: {

            //attack

            //initialize enemies when room is loaded and reloaded
            if (std::find(enemyID.begin(), enemyID.end(), 201) == enemyID.end()) {


                Enemy1 *enemy1 = new Enemy1(this);
                enemy1->controltype = ControlType::Path;
                enemy1->id = 201;
                enemy1->position.x = 5 * 32;
                enemy1->position.y = 4 * 32;
                enemy1->stopleft = 9 * 32; //creates new stop points for Enemy1 instance enemy1
                enemy1->stopdown = 9 * 32;
                enemy1->stopright = 12 * 32;
                enemy1->stopup = 6 * 32 + 16;
                enemy1->calculatePathAsRectangle();
                enemies.push_back(enemy1);

                //attack
                //enemy1->setAttackPower(1);
                //themaincharacter.attack(enemy1);
                //enemy1.attack(themaincharacter); //attack on maincharacter
                //enemy1->attack(maincharacter);

                if (enemy1->health == 0) {
                    enemyID.push_back(enemy1->id);
                }
            }

            if (std::find(enemyID.begin(), enemyID.end(), 202) == enemyID.end()) {

                Enemy2 *enemy2 = new Enemy2(this);
                enemy2->controltype = ControlType::Random;
                enemy2->id = 202;
                enemy2->position.x = 22 * 32 + 16;
                enemy2->position.y = 8 * 32;
                enemy2->stopleft = 11 * 32; //creates new stop points for Enemy1 instance enemy1
                enemy2->stopdown = 6 * 32;
                enemy2->stopright = 8 * 32;
                enemy2->stopup = 3 * 32 + 16;
                enemies.push_back(enemy2);

                if (enemy2->health == 0) {
                    enemyID.push_back(enemy2->id);
                }
            }

            if (std::find(enemyID.begin(), enemyID.end(), 203) == enemyID.end()) {

                Enemy3 *enemy3 = new Enemy3(this);
                enemy3->controltype = ControlType::Path;
                enemy3->id = 203;
                enemy3->position.x = 12 * 32;
                enemy3->position.y = 9 * 32;
                enemies.push_back(enemy3);


                float polygons = 180;
                for (int i = 0; i < polygons; i++) {
                    float angle = (float) i * PI * 2 / polygons;    //walks in circle
                    enemy3->path.push_back({cos(angle) * 64 + 12 * 32, sin(angle) * 64 + 7 * 32});
                }

                if (enemy3->health == 0) {
                    enemyID.push_back(enemy3->id);
                }
            }

            //attack character



            //creates enemies for room 2
            /*enemies.push_back((new Enemy1(this)));
                    //creates enemies with diffrent stop points


            enemies.push_back((new Enemy2(this)));
            enemies.push_back((new Enemy3(this)));



            gameplay scene; //implements gameplay instance

            Enemy1 enemy1(&scene); //creates Enemy1 instance
            enemy1.controltype = Path; // sets control type

            Enemy2 enemy2(&scene);
            enemy2.controltype = Random;

            Enemy3 enemy3(&scene);
            enemy3.controltype = Path;*/
            //code



        }
            break;

        case 3: {
        }
            break;
        case 4: {

        }
            break;
        case 5: {

        }
            break;
        case 6: {

        }
            break;
    }
}

void gameplay::deactivateFirebowls() {
    //this is needed, if you want the firebowls to be deactived again, once you leave the room. without this code, the firebowl will still be active when you come back to the room
    // Clear activated firebowls not in the current room
    activatedFirebowls.erase(
            std::remove_if(activatedFirebowls.begin(), activatedFirebowls.end(),
                           [this](const auto &bowl) { return bowl.room != room; }),
            activatedFirebowls.end()
    );

    activeFirebowlAnimations.erase(
            std::remove_if(activeFirebowlAnimations.begin(), activeFirebowlAnimations.end(),
                           [this](const auto &bowl) { return bowl.room != room; }),
            activeFirebowlAnimations.end()
    );
}

void gameplay::loadMap() {
    tson::Tileson tileson;
    tiles.clear();
    std::string mapFile;
    //load map
    switch (room) {
        case 1:
            mapFile = "assets/graphics/tilesets/room1.tmj";
            break;
        case 2:
            mapFile = "assets/graphics/tilesets/room2.tmj";
            break;
        case 3:
            mapFile = "assets/graphics/tilesets/room5.tmj";
            break;
        case 4:
            mapFile = "assets/graphics/tilesets/room3.tmj";
            break;
        case 5:
            mapFile = "assets/graphics/tilesets/room4.tmj";
            break;
        case 6:
            mapFile = "assets/graphics/tilesets/room5test.tmj";
            break;
    }
    auto map = tileson.parse(mapFile);
    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cout << map->getStatusMessage();
    }
    auto layer = map->getLayer("Kachelebene 1");

    for (int y = 0; y < layer->getSize().y; y++) {
        for (int x = 0; x < layer->getSize().x; x++) {
            tiles.push_back(layer->getData()[y * layer->getSize().x + x]);
            tiles.back()--;
        }
    }
    mapWidth = layer->getSize().x;
    mapHeight = layer->getSize().y;

}

int gameplay::getTileAt(float x, float y) const {
    //catch out of bounds
    if (x < 0 || y < 0 || x >= mapWidth * 32 || y >= mapHeight * 32) {
        return 0;
    }
    int tileX = x / 32;
    int tileY = y / 32;
    return tiles[tileY * mapWidth + tileX];
}

bool gameplay::touchesWall(Vector2 pos, float size) {
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = getTileAt(x * 32, y * 32);
            bool touchesAny = false;
            for (const auto &group: firebowlsIDs) {
                for (int id: group) {
                    if (id == tileID) {
                        if (CheckCollisionCircleRec(pos, size,
                                                    Rectangle{(float) x * 32, (float) y * 32, (float) 32,
                                                              (float) 32})) {
                            return true;
                        }
                        touchesAny = true;
                        break;
                    }
                }
                if (touchesAny) {
                    break;
                }
            }
            for (const auto &group: wallIDs) {
                for (int id: group) {
                    if (id == tileID) {
                        if (CheckCollisionCircleRec(pos, size,
                                                    Rectangle{(float) x * 32, (float) y * 32, (float) 32,
                                                              (float) 32})) {
                            return true;
                        }
                        touchesAny = true;
                        break;
                    }
                }
                if (touchesAny) {
                    break;
                }
            }
            if (!areAllFirebowlsActivatedInRoom(1)) {
                for (const auto &group: factorydoor) {
                    for (int id: group) {
                        if (id == tileID) {
                            if (CheckCollisionCircleRec(pos, size,
                                                        Rectangle{(float) x * 32, (float) y * 32, (float) 32,
                                                                  (float) 32})) {
                                return true;
                            }
                            touchesAny = true;
                            break;
                        }
                    }
                    if (touchesAny) {
                        break;
                    }
                }
            }
        }
    }
    return false;
}

bool gameplay::soulcantakeover() {
    if (CheckCollisionPointCircle({themaincharacter->position.x, themaincharacter->position.y},
                                  {therobot->position.x, therobot->position.y}, takeoverradius)) {
        return true;
    } else {
        return false;
    }
}

Vector2 gameplay::getRobotPosition() const {
    return therobot->position;
}

float gameplay::getTakeoverRadius() const {
    return takeoverradius;
}

Rectangle gameplay::getTouchedWall(Vector2 position, float radius) {
    //check all walls
    //on contact, note distance to wall
    //return wall with shortest distance
    float shortestDistance = 1000000;
    Rectangle closestWall{};
    bool foundWall = false;
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = getTileAt(x * 32, y * 32);
            for (const auto &group: firebowlsIDs) {
                for (int id: group) {
                    if (tileID == id) {
                        Rectangle wall{static_cast<float>(x * 32), static_cast<float>(y * 32), 32, 32};
                        Vector2 wallTouchPoint = Vector2Clamp(position, Vector2{wall.x, wall.y},
                                                              Vector2{wall.x + wall.width, wall.y + wall.height});
                        float distance = Vector2Distance(position, wallTouchPoint);
                        if (distance < shortestDistance) {
                            shortestDistance = distance;
                            closestWall = wall;
                            foundWall = true;
                        }
                        break;
                    }
                }
            }
            for (const auto &group: wallIDs) {
                for (int id: group) {
                    if (tileID == id) {
                        Rectangle wall{static_cast<float>(x * 32), static_cast<float>(y * 32), 32, 32};
                        Vector2 wallTouchPoint = Vector2Clamp(position, Vector2{wall.x, wall.y},
                                                              Vector2{wall.x + wall.width,
                                                                      wall.y + wall.height});
                        float distance = Vector2Distance(position, wallTouchPoint);
                        if (distance < shortestDistance) {
                            shortestDistance = distance;
                            closestWall = wall;
                            foundWall = true;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (foundWall) {
        return
                closestWall;
    } else {
        return Rectangle{
        };
    }
}


bool gameplay::isTileYouCantPushStoneOnto(int tileID) const {
    //all tile IDs that represent walls
    static const std::vector<std::vector<int>> wallIDs;

    return std::any_of(wallIDs.begin(), wallIDs.end(),
                       [tileID](const std::vector<int> &wallSet) {
                           return std::find(wallSet.begin(), wallSet.end(), tileID) != wallSet.end();
                       });
}

bool gameplay::touchesBars(Vector2 pos, float size) {
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = getTileAt(x * 32, y * 32);
            bool touchesAny = false;
            for (const auto &group: fencesIDs) {
                for (int id: group) {
                    if (id == tileID) {
                        if (CheckCollisionCircleRec(pos, size,
                                                    Rectangle{(float) x * 32, (float) y * 32, (float) 32,
                                                              (float) 32})) {
                            return true;
                        }
                        touchesAny = true;
                        break;
                    }
                }
                if (touchesAny) {
                    break;
                }
            }
        }
    }
    return false;
}

Rectangle gameplay::getTouchedBars(Vector2 position, float radius) {
    float shortestDistance = 1000000;
    Rectangle closestBar{};
    bool foundBars = false;
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = getTileAt(x * 32, y * 32);
            for (const auto &group: fencesIDs) {
                for (int id: group) {
                    if (tileID == id) {
                        Rectangle bars{static_cast<float>(x * 32), static_cast<float>(y * 32), 32, 32};
                        Vector2 barsTouchPoint = Vector2Clamp(position, Vector2{bars.x, bars.y},
                                                              Vector2{bars.x + bars.width,
                                                                      bars.y + bars.height});
                        float distance = Vector2Distance(position, barsTouchPoint);
                        if (distance < shortestDistance) {
                            shortestDistance = distance;
                            closestBar = bars;
                            foundBars = true;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (foundBars) {
        return closestBar;
    } else {
        return Rectangle{};
    }
}


bool gameplay::touchesAbyss(Vector2 pos, float size) {
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            bool touchesAny = false;
            int tileID = getTileAt(x * 32, y * 32);
            for (const auto &group: allabyssIDs) {
                for (int id: group) {
                    if (id == tileID) {
                        if (CheckCollisionCircleRec(pos, size,
                                                    Rectangle{(float) x * 32, (float) y * 32, (float) 32,
                                                              (float) 32})) {
                            return true;
                        }
                        touchesAny = true;
                        break;
                    }
                }
                if (touchesAny) {
                    break;
                }
            }
        }
    }
    return false;
}

Rectangle gameplay::getTouchedAbyss(Vector2 position, float radius) {
    float shortestDistance = 1000000;
    Rectangle closestAbyss{};
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileID = getTileAt(x * 32, y * 32);
            for (const auto &group: allabyssIDs) {
                for (int id: group) {
                    if (tileID == id) {
                        Rectangle abyss{static_cast<float>(x * 32), static_cast<float>(y * 32), 32, 32};
                        Vector2 barsTouchPoint = Vector2Clamp(position, Vector2{abyss.x, abyss.y},
                                                              Vector2{abyss.x + abyss.width,
                                                                      abyss.y + abyss.height});
                        float distance = Vector2Distance(position, barsTouchPoint);
                        if (distance < shortestDistance) {
                            shortestDistance = distance;
                            closestAbyss = abyss;

                        }
                        return closestAbyss;
                    }
                }
            }
        }
    }
    return Rectangle{0, 0, 0, 0}; // Returns an empty rectangle if no abyss is touched
}

const std::vector<Enemy *> &gameplay::getEnemies() const {
    return enemies;
}

void gameplay::clearEnemies() {
    for (Enemy *enemy: enemies) {
        delete enemy;
    }
    enemies.clear();
}

void gameplay::spawnStone(int room, Vector2 position) {
    Stone *newStone = new Stone(this, position);
    newStone->setScene(this);
    stonesInRooms[room].push_back(newStone);
    initialStonePositions[room].push_back({newStone, position});
}

void gameplay::updateStones() {
    if (stonesInRooms.find(room) != stonesInRooms.end()) {
        for (auto &stone: stonesInRooms[room]) {
            stone->update();
        }
    }
}

void gameplay::drawStones() {
    //std::cout << "Drawing stones for room " << room << std::endl;
    if (stonesInRooms.find(room) != stonesInRooms.end()) {
        //std::cout << "Number of stones in room: " << stonesInRooms[room].size() << std::endl;
        for (const auto &stone: stonesInRooms[room]) {
            stone->draw();
            //std::cout << "Stone drawn at position: (" << stone->position.x << ", " << stone->position.y << ")" << std::endl;
        }
    } else {
        //std::cout << "No stones in this room" << std::endl;
    }
}

bool gameplay::touchesStone(Vector2 tilePosition) const {
    //std::cout << "Checking for stone at tile position: (" << tilePosition.x << ", " << tilePosition.y << ")" << std::endl;
    if (stonesInRooms.find(room) != stonesInRooms.end()) {
        for (const auto &stone: stonesInRooms.at(room)) {
            Vector2 stoneTilePos = stone->getTilePosition();
            //std::cout << "Stone at tile position: (" << stoneTilePos.x << ", " << stoneTilePos.y << ")" << std::endl;
            if (stoneTilePos.x == tilePosition.x && stoneTilePos.y == tilePosition.y) {
                //std::cout << "Stone collision detected" << std::endl;
                return true;
            }
        }
    }
    //std::cout << "No stone collision detected" << std::endl;
    return false;
}

Stone *gameplay::getStoneAt(Vector2 tilePosition) const {
    //std::cout << "Getting stone at tile position: (" << tilePosition.x << ", " << tilePosition.y << ")" << std::endl;
    if (stonesInRooms.find(room) != stonesInRooms.end()) {
        for (auto &stone: stonesInRooms.at(room)) {
            Vector2 stoneTilePos = stone->getTilePosition();
            //std::cout << "Checking stone at tile position: (" << stoneTilePos.x << ", " << stoneTilePos.y << ")" << std::endl;
            if (std::abs(stoneTilePos.x - tilePosition.x) < 0.1f &&
                std::abs(stoneTilePos.y - tilePosition.y) < 0.1f) {
                //std::cout << "Stone found" << std::endl;
                return stone;
            }
        }
    }
    //std::cout << "No stone found" << std::endl;
    return nullptr;
}

void gameplay::resetStonesToInitialPositions() {
    if (initialStonePositions.find(room) != initialStonePositions.end()) {
        for (auto &stonePair: initialStonePositions[room]) {
            Stone *stone = stonePair.first;
            Vector2 initialPos = stonePair.second;
            stone->position = initialPos;
        }
    }
}

void gameplay::spawnBlock(Vector2 position, bool isYellow) {
    int tileX = static_cast<int>(position.x / 32);
    int tileY = static_cast<int>(position.y / 32);

    // Check if the tile is walkable
    if (getTileAt(tileX * 32, tileY * 32) == floor0ID ||
        (getTileAt(tileX * 32, tileY * 32) >= grass0ID && getTileAt(tileX * 32, tileY * 32) <= grass6ID)) {

        movableblocks *newBlock;
        if (isYellow) {
            newBlock = new yellowblock(this, position);
            //std::cout << "Spawned yellow block at (" << position.x << ", " << position.y << ")" << std::endl;
        } else {
            newBlock = new blueblock(this, position);
            //std::cout << "Spawned blue block at (" << position.x << ", " << position.y << ")" << std::endl;
        }
        blocksInRooms[room].push_back(newBlock);
    } else {
        //std::cout << "Failed to spawn block at (" << position.x << ", " << position.y << ") - not a walkable tile" << std::endl;
    }
}

void gameplay::updateBlocks() {
    if (blocksInRooms.find(room) != blocksInRooms.end()) {
        for (auto &block: blocksInRooms[room]) {
            block->update();
        }
    }
}

void gameplay::drawBlocks() {
    //std::cout << "Drawing blocks for room " << room << std::endl;
    if (blocksInRooms.find(room) != blocksInRooms.end()) {
        //std::cout << "Number of blocks in room: " << blocksInRooms[room].size() << std::endl;
        for (const auto &block: blocksInRooms[room]) {
            block->draw();
            //std::cout << "Block drawn at position: (" << block->position.x << ", " << block->position.y << ")" << std::endl;
        }
    } else {
        //std::cout << "No blocks in this room" << std::endl;
    }
}

bool gameplay::isBlockAt(Vector2 tilePosition) const {
    if (blocksInRooms.find(room) != blocksInRooms.end()) {
        for (const auto &block: blocksInRooms.at(room)) {
            Vector2 blockTilePos = {std::floor(block->position.x / 32), std::floor(block->position.y / 32)};
            if (blockTilePos.x == tilePosition.x && blockTilePos.y == tilePosition.y) {
                return !block->canWalkThrough();
            }
        }
    }
    return false;
}

void gameplay::toggleBlockAt(Vector2 tilePosition) {
    if (blocksInRooms.find(room) != blocksInRooms.end()) {
        for (auto &block: blocksInRooms.at(room)) {
            Vector2 blockTilePos = {std::floor(block->position.x / 32), std::floor(block->position.y / 32)};
            if (blockTilePos.x == tilePosition.x && blockTilePos.y == tilePosition.y) {
                block->toggle();
                break;
            }
        }
    }
}

void gameplay::spawnSwitch(Vector2 position, bool isYellow) {
    int tileX = static_cast<int>(position.x / 32);
    int tileY = static_cast<int>(position.y / 32);

    // Check if the tile is walkable
    if (getTileAt(tileX * 32, tileY * 32) == floor0ID ||
        (getTileAt(tileX * 32, tileY * 32) >= grass0ID && getTileAt(tileX * 32, tileY * 32) <= grass6ID)) {

        movableblocks *newSwitch;
        if (isYellow) {
            newSwitch = new yellowblock(this, {static_cast<float>(tileX * 32), static_cast<float>(tileY * 32)},
                                        true);
            //std::cout << "Spawned yellow switch at (" << tileX * 32 << ", " << tileY * 32 << ")" << std::endl;
        } else {
            newSwitch = new blueblock(this, {static_cast<float>(tileX * 32), static_cast<float>(tileY * 32)},
                                      true);
            //std::cout << "Spawned blue switch at (" << tileX * 32 << ", " << tileY * 32 << ")" << std::endl;
        }
        switchesInRooms[room].push_back(newSwitch);
    } else {
        /*std::cout << "Failed to spawn switch at (" << position.x << ", " << position.y << ") - not a walkable tile"
                  << std::endl;*/
    }
}

void gameplay::updateSwitches() {
    if (switchesInRooms.find(room) != switchesInRooms.end()) {
        for (auto &switchBlock: switchesInRooms[room]) {
            switchBlock->update();
        }
    }
}

void gameplay::drawSwitches() {
    //std::cout << "Drawing switches for room " << room << std::endl;
    if (switchesInRooms.find(room) != switchesInRooms.end()) {
        //std::cout << "Number of switches in room: " << switchesInRooms[room].size() << std::endl;
        for (const auto &switchBlock: switchesInRooms[room]) {
            switchBlock->draw();
            /*std::cout << "Switch drawn at position: (" << switchBlock->position.x << ", " << switchBlock->position.y
                      << ")" << std::endl;*/
        }
    } else {
        //std::cout << "No switches in this room" << std::endl;
    }
}

bool gameplay::isSwitchAt(Vector2 tilePosition) const {
    if (switchesInRooms.find(room) != switchesInRooms.end()) {
        for (const auto &switchBlock: switchesInRooms.at(room)) {
            Vector2 switchTilePos = {std::floor(switchBlock->position.x / 32),
                                     std::floor(switchBlock->position.y / 32)};
            if (switchTilePos.x == tilePosition.x && switchTilePos.y == tilePosition.y) {
                return true;
            }
        }
    }
    return false;
}

void gameplay::toggleSwitchAt(Vector2 tilePosition) {
    //std::cout << "Attempting to toggle switch near (" << tilePosition.x << ", " << tilePosition.y << ")" << std::endl;
    if (switchesInRooms.find(room) != switchesInRooms.end()) {
        for (auto &switchBlock: switchesInRooms.at(room)) {
            Vector2 switchTilePos = {std::floor(switchBlock->position.x / 32),
                                     std::floor(switchBlock->position.y / 32)};
            //std::cout << "Checking switch at (" << switchTilePos.x << ", " << switchTilePos.y << ")" << std::endl;
            if (abs(switchTilePos.x - tilePosition.x) <= 1 && abs(switchTilePos.y - tilePosition.y) <= 1) {
                switchBlock->toggle();
                //std::cout << "Switch toggled at (" << switchTilePos.x << ", " << switchTilePos.y << ")" << std::endl;
                // Toggle all blocks of the same color
                for (auto &block: blocksInRooms[room]) {
                    if ((dynamic_cast<yellowblock *>(switchBlock) && dynamic_cast<yellowblock *>(block)) ||
                        (dynamic_cast<blueblock *>(switchBlock) && dynamic_cast<blueblock *>(block))) {
                        block->toggle();
                    }
                }
                return;
            }
        }
    }
    //std::cout << "No switch found to toggle" << std::endl;
}

bool gameplay::isAdjacentToSwitch(Vector2 position) const {
    int tileX = static_cast<int>(position.x / 32);
    int tileY = static_cast<int>(position.y / 32);

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; // Skip the tile the character is on
            int checkX = tileX + dx;
            int checkY = tileY + dy;
            if (isSwitchAt({static_cast<float>(checkX), static_cast<float>(checkY)})) {
                return true;
            }
        }
    }
    return false;
}

void gameplay::addBomb(bombs *bomb) {
    //std::cout << "Adding bomb to game!" << std::endl;
    activeBombs.push_back(bomb);
}