///
// Created by sweet on 29.05.2024.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include "assestmanagergraphics.h"
#include <filesystem>
namespace fs = std::filesystem;

Texture2D LoadTextureAndLog(const std::string& path) {
    if (fs::exists(path)) {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load texture: %s", path.c_str());
        } else {
            TraceLog(LOG_INFO, "Successfully loaded texture: %s", path.c_str());
        }
        return texture;
    } else {
        TraceLog(LOG_WARNING, "Texture file does not exist: %s", path.c_str());
        return {0}; // Return an invalid texture
    }
}




bool assestmanagergraphics::m_texturesLoaded = false;
std::map<std::string, Texture2D> assestmanagergraphics::m_textures;
std::map<std::string, std::map<AnimationState, std::map<Direction, Texture2D>>> assestmanagergraphics::m_animations;
std::map<std::string, std::map<AnimationState, Texture2D>> assestmanagergraphics::m_objectAnimations;
std::map<std::string, Texture2D> assestmanagergraphics::m_singleFrameTextures;

/*/ Helper functions
std::string toLowercase(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}*/

void assestmanagergraphics::init() {
    if (m_texturesLoaded) {
        TraceLog(LOG_ERROR, "assetmanagergraphics::init() called more than once");
        return;
    }
    m_texturesLoaded = true;

    m_textures["ERROR"] = LoadTexture("assets/graphics/ERROR.png");
    //m_textures["tilesets/level1"] = LoadTexture("assets/graphics/tilesets/tilesets/level1.png");
    //m_textures["tileset/level1"] = LoadTexture("assets/graphics/tilesets/tileset_alles_Stand20240820.png");
    m_textures["tileset/final"] = LoadTexture("assets/graphics/tilesets/Tilesnek.png");

    //placeholder menu
    m_textures["background/background_blue"] = LoadTexture("assets/graphics/backgrounds/background_blue.png");
    m_textures["userinterface/button_close"] = LoadTexture("assets/graphics/userinterface/button_close.png");
    m_textures["userinterface/button_pause"] = LoadTexture("assets/graphics/userinterface/button_pause.png");
    m_textures["userinterface/button_play"] = LoadTexture("assets/graphics/userinterface/button_play.png");
    m_textures["userinterface/button_play_1"] = LoadTexture("assets/graphics/userinterface/button_play_1.png");
    m_textures["userinterface/button_settings"] = LoadTexture("assets/graphics/userinterface/button_settings.png");
    m_textures["userinterface/controller"] = LoadTexture("assets/graphics/userinterface/controller.png");
    m_textures["userinterface/flag_DE_5"] = LoadTexture("assets/graphics/userinterface/flag_DE_5.png");
    m_textures["userinterface/flag_ENG_2"] = LoadTexture("assets/graphics/userinterface/flag_ENG_2.png");
    m_textures["userinterface/grasses"] = LoadTexture("assets/graphics/userinterface/grasses.png");
    m_textures["tilesets/grassesfour"] = LoadTexture("assets/graphics/tilesets/grassesfour.png");
    m_textures["tilesets/greyboxing1"] = LoadTexture("assets/graphics/tilesets/tileset_greyboxing_room1.png");
    m_textures["userinterface/heart"] = LoadTexture("assets/graphics/userinterface/heart.png");
    m_textures["userinterface/heart_smaller"] = LoadTexture("assets/graphics/userinterface/heart_smaller.png");
    m_textures["userinterface/journal_background"] = LoadTexture("assets/graphics/userinterface/journal_background.png");
    m_textures["userinterface/journal_background_newsize"] = LoadTexture("assets/graphics/userinterface/journal_background_newsize.png");
    m_textures["userinterface/robot_anuki_3"] = LoadTexture("assets/graphics/userinterface/robot_anuki_3.png");
    m_textures["userinterface/soul_destiny_3"] = LoadTexture("assets/graphics/userinterface/soul_destiny_3.png");
    m_textures["userinterface/soundless"] = LoadTexture("assets/graphics/userinterface/soundless.png");
    m_textures["userinterface/soundmore"] = LoadTexture("assets/graphics/userinterface/soundmore.png");
    m_textures["userinterface/tastatur"] = LoadTexture("assets/graphics/userinterface/tastatur.png");
    m_textures["userinterface/testimage"] = LoadTexture("assets/graphics/userinterface/testimage.png");
    m_textures["userinterface/tile_pink"] = LoadTexture("assets/graphics/userinterface/tile_pink.png");
    m_textures["userinterface/title_3"] = LoadTexture("assets/graphics/userinterface/title_3.png");
    m_textures["userinterface/title_4"] = LoadTexture("assets/graphics/userinterface/title_4.png");

    //final menu
    m_textures["userinterface/start"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - Start - static.png");
    m_textures["userinterface/start_selected"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - pressed Start - static.png");
    m_textures["userinterface/settings"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - Settings - static.png");
    m_textures["userinterface/settings_selected"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - pressed Settings - static.png");
    m_textures["userinterface/close"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - Close - static.png");
    m_textures["userinterface/close_selected"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - pressed Close - static.png");
    m_textures["userinterface/pause"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - Pause - static.png");
    m_textures["userinterface/new"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - New - static.png");
    m_textures["userinterface/new_selected"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - pressed New - static.png");
    m_textures["userinterface/continue"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - Continue - static.png");
    m_textures["userinterface/continue_selected"] = LoadTexture("assets/graphics/userinterface/UI - Menu Button - pressed Continue - static.png");
    m_textures["userinterface/logo_soulsteel"] = LoadTexture("assets/graphics/userinterface/Logo_resized.png");
    m_textures["userinterface/logo_team"] = LoadTexture("assets/graphics/userinterface/Team Logo White_resized.png");
    m_textures["background/background"] = LoadTexture("assets/graphics/backgrounds/Menu background_resized.png");
    m_textures["userinterface/arrowleft"] = LoadTexture("assets/graphics/userinterface/Ui_-_Journal_-_Arrow_left_-_static_resized.png");
    m_textures["userinterface/arrowright"] = LoadTexture("assets/graphics/userinterface/Ui_-_Journal_-_Arrow_right_-_static_resized.png");
    m_textures["userinterface/closejournal"] = LoadTexture("assets/graphics/userinterface/Ui_-_Journal_-_Closing_X_-_static_resized.png");
    m_textures["userinterface/background_journal"] = LoadTexture("assets/graphics/userinterface/UI_-_Journal_-_open_-_static_resized.png");
    m_textures["userinterface/journal_pic"] = LoadTexture("assets/graphics/userinterface/journal_image_resized.png");

    //ability icons
    m_textures["userinterface/icon_dash"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Soul dash - static.png");
    m_textures["userinterface/icon_dust"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Soul dust - static.png");
    m_textures["userinterface/icon_stone"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Robot Interaction - static.png");
    m_textures["userinterface/icon_bomb"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Robot bomb - static.png");

    //health
    m_textures["userinterface/heart_robot"]= LoadTexture("assets/graphics/userinterface/UI - HP heart - Robot - static.png");
    m_textures["userinterface/heart_soul"]= LoadTexture("assets/graphics/userinterface/UI - HP heart - Soul - static.png");

    //bombs
    m_textures["item/bomb_thrown"] = LoadTexture("assets/graphics/items/bomb_lit.png");
    m_textures["item/bomb_exploding"] = LoadTexture("assets/graphics/items/bomb_explotion.png");

    //firebowls
    m_textures["item/animated_firebowl"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Fire bowl - animated - activated.png");

    //stones
    m_textures["item/stone"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - MovableBlock - Static.png");

    // Load block textures
    m_textures["item/yellow_block"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Yellow Block - Static.png");
    m_textures["item/yellow_block_turning_on"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Yellow Block - Animated - Turn on.png");
    m_textures["item/yellow_block_turning_off"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Yellow Block - Animated - Turn off.png");
    m_textures["item/blue_block"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Blue Block - Static.png");
    m_textures["item/blue_block_turning_on"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Blue Block - Animated - Turn on.png");
    m_textures["item/blue_block_turning_off"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Blue Block - Animated - Turn off.png");
    m_textures["item/blue_switch_on"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Blue Switch - Animated.png");
    m_textures["item/blue_switch_off"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Blue Switch - Static.png");
    m_textures["item/yellow_switch_on"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Yellow Switch - Animated.png");
    m_textures["item/yellow_switch_off"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Yellow Switch - Static.png");


    //load switching animation textures
    m_textures["Switch-Animation/Soul_to_Robot"] = LoadTexture("assets/graphics/characters/soul/soul_switch.png");
    m_textures["Switch-Animation/Robot_to_Soul"] = LoadTexture("assets/graphics/characters/robot/robot_switch.png");

    m_textures["placeholder/standby-robot"]= LoadTexture("assets/graphics/characters/robot/robot_notpossessed.png");

    loadObjectAnimations();
    loadSingleFrameTextures();
    loadCharacterAnimations();
    loadEnemyAnimations();
}


void assestmanagergraphics::loadCharacterAnimations() {
    std::string basePath = "assets/graphics/characters/";
    std::vector<std::string> characters = {"soul", "robot"};
    std::vector<std::string> states = {"idle", "walk", "normal", "ranged", "dust", "dash"};
    std::vector<std::string> directions = {"back", "front", "left", "right"};

    for (const auto& character : characters) {
        for (const auto& state : states) {
            for (const auto& dir : directions) {
                AnimationState animState;
                if (state == "idle") animState = AnimationState::IDLE;
                else if (state == "walk") animState = AnimationState::WALK;
                else if (state == "normal") animState = AnimationState::ATTACK_NORMAL;
                else if (state == "ranged") animState = AnimationState::ATTACK_RANGED;
                else if (state == "dust") animState = AnimationState::DUST;
                else if (state == "dash") animState = AnimationState::DASH;
                else continue; // Skip if not a valid state for this character

                Direction direction = (dir == "back") ? Direction::Up :
                                      (dir == "front") ? Direction::Down :
                                      (dir == "left") ? Direction::Left : Direction::Right;

                std::string path = basePath + character + "/" + character + "_" + state + "_" + dir + ".png";
                Texture2D texture = LoadTexture(path.c_str());
                m_animations[character][animState][direction] = texture;

                std::cout << "Loaded " << character << " animation: " << path << std::endl;
                std::cout << "Texture ID: " << texture.id << ", Width: " << texture.width << ", Height: " << texture.height << std::endl;

                if (character == "soul" && state == "dust") {
                    // Special case for soul dust effect
                    path = basePath + character + "/dust_dust_" + dir + ".png";
                } else {
                    path = basePath + character + "/" + character + "_" + state + "_" + dir + ".png";
                }

                m_animations[character][animState][direction] = LoadTexture(path.c_str());

                // Print debug information
                std::cout << "Loaded " << character << " animation: " << path << std::endl;

            }
        }
    }

    // Load special animations
    // Soul switch animation
    m_animations["soul"][AnimationState::SWITCH][Direction::Down] = LoadTexture((basePath + "soul/soul_switch.png").c_str());

    // Robot switch animation
    m_animations["robot"][AnimationState::SWITCH][Direction::Down] = LoadTexture((basePath + "robot/robot_switch.png").c_str());

    // Robot special animations
    m_animations["robot"][AnimationState::RANGED_EFFECT][Direction::Left] = LoadTexture((basePath + "robot/screwers_ranged_left.png").c_str());
    m_animations["robot"][AnimationState::RANGED_EFFECT][Direction::Right] = LoadTexture((basePath + "robot/screwers_ranged_right.png").c_str());
    m_animations["robot"][AnimationState::FLYINGSCREWS][Direction::Down] = LoadTexture((basePath + "robot/screws_flyingscrews.png").c_str());


    //
// Load soul dust body animations
    m_animations["soul"][AnimationState::DUST][Direction::Up] = LoadTexture("assets/graphics/characters/soul/soul_dust_back.png");
    m_animations["soul"][AnimationState::DUST][Direction::Down] = LoadTexture("assets/graphics/characters/soul/soul_dust_front.png");
    m_animations["soul"][AnimationState::DUST][Direction::Left] = LoadTexture("assets/graphics/characters/soul/soul_dust_left.png");
    m_animations["soul"][AnimationState::DUST][Direction::Right] = LoadTexture("assets/graphics/characters/soul/soul_dust_right.png");

    // Load dust effect animations
    m_animations["dust"][AnimationState::DUST][Direction::Up] = LoadTexture("assets/graphics/characters/soul/dust_dust_back.png");
    m_animations["dust"][AnimationState::DUST][Direction::Down] = LoadTexture("assets/graphics/characters/soul/dust_dust_front.png");
    m_animations["dust"][AnimationState::DUST][Direction::Left] = LoadTexture("assets/graphics/characters/soul/dust_dust_left.png");
    m_animations["dust"][AnimationState::DUST][Direction::Right] = LoadTexture("assets/graphics/characters/soul/dust_dust_right.png");





    std::cout << "Loaded special animations" << std::endl;
}

//*NEW CODE*
void assestmanagergraphics::loadEnemyAnimations() {
    std::cout << "Starting to load enemy animations..." << std::endl;

    std::vector<std::string> enemyTypes = {"enemy1", "enemy2", "enemy3"};
    std::vector<std::string> states = {"idle", "walk", "normal", "ranged"};
    std::vector<std::string> directions = {"back", "front", "left", "right"};

    for (const auto& enemyType : enemyTypes) {
        std::string entityName = (enemyType == "enemy1") ? "teddy" :
                                 (enemyType == "enemy2") ? "spider" :
                                 (enemyType == "enemy3") ? "tacklespider" : enemyType;

        for (const auto& state : states) {
            for (const auto& dir : directions) {
                std::string fileName = entityName + "_" + state + "_" + dir + ".png";
                std::string path = "assets/graphics/characters/enemies/" + enemyType + "/" + fileName;

                std::cout << "Attempting to load texture: " << path << std::endl;

                Texture2D texture = LoadTexture(path.c_str());
                if (texture.id == 0) {
                    std::cout << "Failed to load texture: " << path << std::endl;
                    if (!FileExists(path.c_str())) {
                        std::cout << "File does not exist: " << path << std::endl;
                    }
                } else {
                    std::cout << "Successfully loaded texture: " << path << " (ID: " << texture.id << ")" << std::endl;

                    AnimationState animState;
                    if (state == "idle") animState = AnimationState::IDLE;
                    else if (state == "walk") animState = AnimationState::WALK;
                    else if (state == "normal") animState = AnimationState::ATTACK_NORMAL;
                    else if (state == "ranged") animState = AnimationState::ATTACK_RANGED;

                    Direction direction;
                    if (dir == "back") direction = Direction::Up;
                    else if (dir == "front") direction = Direction::Down;
                    else if (dir == "left") direction = Direction::Left;
                    else if (dir == "right") direction = Direction::Right;

                    m_animations[entityName][animState][direction] = texture;
                    std::cout << "Stored texture for " << entityName << ", state: " << static_cast<int>(animState) << ", direction: " << static_cast<int>(direction) << std::endl;
                }
            }
        }

        // Load spidertooth ranged attack animations for Enemy3
        if (enemyType == "enemy3") {
            for (const auto& dir : directions) {
                std::string fileName = "spidertooth_ranged_" + dir + ".png";
                std::string path = "assets/graphics/characters/enemies/" + enemyType + "/" + fileName;

                std::cout << "Attempting to load spidertooth texture: " << path << std::endl;

                Texture2D texture = LoadTexture(path.c_str());
                if (texture.id == 0) {
                    std::cout << "Failed to load spidertooth texture: " << path << std::endl;
                } else {
                    std::cout << "Successfully loaded spidertooth texture: " << path << " (ID: " << texture.id << ")" << std::endl;

                    Direction direction;
                    if (dir == "back") direction = Direction::Up;
                    else if (dir == "front") direction = Direction::Down;
                    else if (dir == "left") direction = Direction::Left;
                    else if (dir == "right") direction = Direction::Right;

                    m_animations["tacklespider"][AnimationState::SPIDERTOOTH][direction] = texture;
                }
            }
        }
    }

    std::cout << "Finished loading enemy animations." << std::endl;
}

void assestmanagergraphics::loadObjectAnimations() {
    std::string basePath = "assets/graphics/items/";

    m_objectAnimations["bomb"][AnimationState::EXPLOTION] =     m_animations["robot"][AnimationState::SWITCH][Direction::Down] = LoadTextureAndLog(basePath + "robot_switch.png");
    (basePath + "bomb_explotion.png");
    m_objectAnimations["bomb"][AnimationState::LIT] =     m_animations["robot"][AnimationState::SWITCH][Direction::Down] = LoadTextureAndLog(basePath + "robot_switch.png");
    (basePath + "bomb_lit.png");
}

void assestmanagergraphics::loadSingleFrameTextures() {
    std::string basePath = "assets/graphics/items/";

    m_singleFrameTextures["souldust"] =     m_animations["robot"][AnimationState::SWITCH][Direction::Down] = LoadTextureAndLog(basePath + "robot_switch.png");
    (basePath + "souldust.png");
    m_singleFrameTextures["stone"] =     m_animations["robot"][AnimationState::SWITCH][Direction::Down] = LoadTextureAndLog(basePath + "robot_switch.png");
    (basePath + "stone.png");
}

Texture2D assestmanagergraphics::getTexture(const std::string &name) {
    if (m_textures.find(name) != m_textures.end()) {
        return m_textures[name];
    }

    TraceLog(LOG_WARNING, "Texture not found: %s", name.c_str());
    return m_textures["ERROR"];
}

Texture2D assestmanagergraphics::getAnimationTexture(const std::string& entityType, AnimationState state, Direction direction) {
    std::cout << "Requesting texture for: " << entityType << ", State: " << static_cast<int>(state)
              << ", Direction: " << static_cast<int>(direction) << std::endl;

    if (m_animations.count(entityType) > 0 &&
        m_animations[entityType].count(state) > 0 &&
        m_animations[entityType][state].count(direction) > 0) {
        Texture2D texture = m_animations[entityType][state][direction];
        std::cout << "Returning texture ID: " << texture.id << " for " << entityType << std::endl;
        return texture;
    }

    std::cout << "Animation texture not found: " << entityType << ", " << static_cast<int>(state) << ", " << static_cast<int>(direction) << std::endl;
    return m_textures["ERROR"]; // Return a default "error" texture
}

Texture2D assestmanagergraphics::getObjectTexture(const std::string& objectName, AnimationState state) {
    if (m_objectAnimations.count(objectName) > 0 &&
        m_objectAnimations[objectName].count(state) > 0) {
        return m_objectAnimations[objectName][state];
    }
    TraceLog(LOG_WARNING, "Object texture not found: %s, %d", objectName.c_str(), static_cast<int>(state));
    return m_textures["ERROR"];
}

Texture2D assestmanagergraphics::getSingleFrameTexture(const std::string& name) {
    if (m_singleFrameTextures.find(name) != m_singleFrameTextures.end()) {
        return m_singleFrameTextures[name];
    }
    TraceLog(LOG_WARNING, "Single frame texture not found: %s", name.c_str());
    return m_textures["ERROR"];
}

void assestmanagergraphics::drawTextureAtlas(const std::string& entityType, AnimationState state, Direction direction, int x, int y) {
    Texture2D texture = getAnimationTexture(entityType, state, direction);
    DrawTexture(texture, x, y, WHITE);

    // Draw frame boundaries
    int frameWidth = texture.width / 8; // Assuming 8 frames
    for (int i = 1; i < 8; i++) {
        DrawLine(x + i * frameWidth, y, x + i * frameWidth, y + texture.height, RED);
    }
}