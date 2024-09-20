///
// Created by sweet on 29.05.2024.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include "assestmanagergraphics.h"

bool assestmanagergraphics::m_texturesLoaded = false;
std::map<std::string, Texture2D> assestmanagergraphics::m_textures;
std::map<std::string, std::map<std::string, Texture2D>> assestmanagergraphics::m_characterAnimations;

// Helper functions
std::string toLowercase(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

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


    //ability icons
    m_textures["userinterface/icon_dash"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Soul dash - static.png");
    m_textures["userinterface/icon_dust"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Soul dust - static.png");
    m_textures["userinterface/icon_stone"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Robot Interaction - static.png");
    m_textures["userinterface/icon_bomb"]= LoadTexture("assets/graphics/userinterface/UI - Button symbol - Robot bomb - static.png");

    m_textures["userinterface/heart_robot"]= LoadTexture("assets/graphics/userinterface/UI - HP heart - Robot - static.png");
    m_textures["userinterface/heart_soul"]= LoadTexture("assets/graphics/userinterface/UI - HP heart - Soul - static.png");

    // Load item textures
    m_textures["item/bomb"] = LoadTexture("assets/graphics/items/bomb.png");
    m_textures["item/bomb_activated"] = LoadTexture("assets/items/bomb_abouttoexplode.png");
    m_textures["item/bomb_exploded"] = LoadTexture("assets/items/bomb_exploded.png");
    m_textures["item/souldust"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Fire bowl - animated - activated.png");
    m_textures["ability/souldust_deactivated"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Fire bowl - static - unactivated.png");

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
    m_textures["Switch-Animation/Soul_to_Robot"] = LoadTexture("assets/graphics/characters/Switch-Animation/Character - Robot+Soul - Switch Soul to Robot - animated.png");
    m_textures["Switch-Animation/Robot_to_Soul"] = LoadTexture("assets/graphics/characters/Switch-Animation/Character - Robot+Soul - Switch Robot to Soul - animated.png");


    m_textures["placeholder/standby-robot"]= LoadTexture("assets/graphics/characters/robot/Character - Robot - Offline - static.png");


    //robot functions
    //bombs
    m_textures["item/bomb_thrown"]=LoadTexture("assets/graphics/items/Character object - Bomb - animated.png");
    m_textures["item/bomb_exploding"]=LoadTexture("assets/graphics/items/Character object - Explosion - animated.png");
    //soul functions
    //soul dust*/
    m_textures["item/souldust"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Fire bowl - animated - activated.png");
    m_textures["ability/souldust_deactivated"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - Fire bowl - static - unactivated.png");
    //soul dash
    m_textures["characters/soul/dash_front"] = LoadTexture("assets/graphics/characters/soul/Character - Soul - Dash front - animated.png");
    m_textures["characters/soul/dash_back"] = LoadTexture("assets/graphics/characters/soul/Character - Soul - Dash back - animated.png");
    m_textures["characters/soul/dash_left"] = LoadTexture("assets/graphics/characters/soul/Character - Soul - Dash side left - animated.png");
    m_textures["characters/soul/dash_right"] = LoadTexture("assets/graphics/characters/soul/Character - Soul - Dash side right - animated.png");
    //stones
    m_textures["item/stone"] = LoadTexture("assets/graphics/PNG - and Spritesheets/Objects/Tile - Object - MovableBlock - Static.png");

    // Enemy1 (Teddy) textures
    m_textures["enemy1_idle_back"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Idle back - animated.png");
    m_textures["enemy1_idle_front"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Idle front - animated.png");
    m_textures["enemy1_idle_left"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Idle side left - animated.png");
    m_textures["enemy1_idle_right"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Idle side right - animated.png");

    m_textures["enemy1_walk_back"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Walk back - animated.png");
    m_textures["enemy1_walk_front"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Walk front - animated.png");
    m_textures["enemy1_walk_left"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Walk side left - animated.png");
    m_textures["enemy1_walk_right"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Walk side right - animated.png");

    m_textures["enemy1_bomb_throw_back"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Bomb throw back - animated.png");
    m_textures["enemy1_bomb_throw_front"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Bomb throw front - animated.png");
    m_textures["enemy1_bomb_throw_left"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Bomb throw side left - body - animated.png");
    m_textures["enemy1_bomb_throw_right"] = LoadTexture("assets/graphics/characters/enemies/enemy1/Character - Enemy - Teddy - Bomb throw side right - body - animated.png");

    // Enemy2 (Spider) textures
    m_textures["enemy2_idle_back"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Idle back - animated.png");
    m_textures["enemy2_idle_front"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Idle front - animated.png");
    m_textures["enemy2_idle_left"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Idle side left - animated.png");
    m_textures["enemy2_idle_right"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Idle side right - animated.png");

    m_textures["enemy2_walk_back"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Walk back - animated.png");
    m_textures["enemy2_walk_front"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Walk front - animated.png");
    m_textures["enemy2_walk_left"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Walk side left - animated.png");
    m_textures["enemy2_walk_right"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Walk side right - animated.png");

    m_textures["enemy2_ranged_back"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Ranged back - animated.png");
    m_textures["enemy2_ranged_front"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Ranged front - animated.png");
    m_textures["enemy2_ranged_left"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Ranged side left - animated.png");
    m_textures["enemy2_ranged_right"] = LoadTexture("assets/graphics/characters/enemies/enemy2/Character - Enemy - Spider - Ranged side right - animated.png");

    // Enemy3 (Tackle Spider) textures
    m_textures["enemy3_idle_back"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Idle back - animated.png");
    m_textures["enemy3_idle_front"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Idle front - animated.png");
    m_textures["enemy3_idle_left"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Idle side left - animated.png");
    m_textures["enemy3_idle_right"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Idle side right - animated.png");

    m_textures["enemy3_walk_back"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Walk back - animated.png");
    m_textures["enemy3_walk_front"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Walk front - animated.png");
    m_textures["enemy3_walk_left"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Walk side left - animated.png");
    m_textures["enemy3_walk_right"] = LoadTexture("assets/graphics/characters/enemies/enemy3/Character - Enemy - Tackle Spider - Walk side right - animated.png");



    //bombtexture Teddy
    m_textures["bomb_teddy_left"]  = LoadTexture("assets/graphics/items/Character - Enemy - Teddy - Bomb throw side left - bomb – animated.png");
    m_textures["bomb_teddy_right"]  = LoadTexture("assets/graphics/items/Character - Enemy - Teddy - Bomb throw side right - bomb – animated.png");

    // Load character animations
    loadCharacterAnimations("soul");
    loadCharacterAnimations("robot");
    //loadCharacterAnimations("enemies");
}

void assestmanagergraphics::loadCharacterAnimations(const std::string &characterName) {
    std::string basePath = "assets/graphics/characters/" + characterName + "/";

    std::vector<std::pair<std::string, std::string>> animations;

    if (characterName == "soul") {
        std::vector<std::string> actions = {"Dash", "Idle", "Walk"};
        std::vector<std::string> directions = {"back", "front", "side left", "side right"};
        for (const auto &action: actions) {
            for (const auto &direction: directions) {
                std::string key = toLowercase(action) + "_" +
                                  (direction == "side left" ? "left" :
                                   direction == "side right" ? "right" : direction);
                std::string fileName = "Character - Soul - " + action + " " + direction + " - animated";
                animations.push_back({key, fileName});
            }
        }

        // Special case for Dust animations
        std::vector<std::string> dustDirections = {"back", "front", "side left", "side right"};
        for (const auto &direction: dustDirections) {
            std::string key = "dust_" + (direction == "side left" ? "left" :
                                         direction == "side right" ? "right" : direction);
            animations.push_back(
                    {key + "_character", "Character - Soul - Dust " + direction + " - character - animated"});
            animations.push_back({key + "_dust", "Character - Soul - Dust " + direction + " - dust - animated"});
        }
    } else if (characterName == "robot") {
        std::vector<std::string> actions = {"Idle", "Walk", "Melee", "Ranged"};
        std::vector<std::string> directions = {"Back", "Front", "side left", "side right"};

        for (const auto &action: actions) {
            for (const auto &direction: directions) {
                std::string key = toLowercase(action) + "_" +
                                  (direction == "side left" ? "left" :
                                   direction == "side right" ? "right" : toLowercase(direction));
                std::string fileName = "Character - Robot - " + action + " " + direction;
                if (action == "Idle" || action == "Walk") {
                    if (direction == "Front" || startsWith(direction, "side")) {
                        animations.push_back({key + "_normal", fileName + " - normal - animated"});
                        animations.push_back({key + "_possessed", fileName + " - possessed - animated"});
                    } else {
                        animations.push_back({key, fileName + " - animated"});
                    }
                } else if (action == "Melee" || action == "Ranged") {
                    if (startsWith(direction, "side")) {
                        animations.push_back({key + "_arm", fileName + " - Arm - animated"});
                        animations.push_back({key + "_body", fileName + " - Body - animated"});
                    } else {
                        animations.push_back({key, fileName + " - animated"});
                    }
                }
            }
        }

                    }


    for (const auto &[key, fileName]: animations) {
        std::string path = basePath + fileName + ".png";
        TraceLog(LOG_INFO, "Attempting to load texture: %s", path.c_str());
        m_characterAnimations[characterName][key] = LoadTexture(path.c_str());
        if (m_characterAnimations[characterName][key].id == 0) {
            TraceLog(LOG_WARNING, "Failed to load texture: %s", path.c_str());
        } else {
            TraceLog(LOG_INFO, "Successfully loaded texture: %s", path.c_str());
        }

    }

}
Texture2D assestmanagergraphics::getTexture(const std::string &name) {
    if (m_textures.find(name) != m_textures.end()) {
        return m_textures[name];
    }

    TraceLog(LOG_WARNING, "Texture not found: %s", name.c_str());
    return m_textures["ERROR"];
}

Texture2D assestmanagergraphics::getCharacterTexture(const std::string &characterName, const std::string &animationName) {
    // Check if the character exists in the map
    if (m_characterAnimations.find(characterName) != m_characterAnimations.end()) {
        // Check if the animation exists for this character
        if (m_characterAnimations[characterName].find(animationName) != m_characterAnimations[characterName].end()) {
            return m_characterAnimations[characterName][animationName];
        }
    }

    // Handle Switch-Animation separately
    if (characterName == "Switch-Animation") {
        std::string textureName = animationName == "Character - Robot+Soul - Switch Soul to Robot - animated"
                                  ? "Switch-Animation/Soul_to_Robot"
                                  : "Switch-Animation/Robot_to_Soul";
        if (m_textures.find(textureName) != m_textures.end()) {
            return m_textures[textureName];
        }
    }

    // If not found, load the texture
    std::string path = "assets/graphics/characters/" + characterName + "/" + animationName + ".png";
    Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        TraceLog(LOG_WARNING, "Character texture not found: %s, %s", characterName.c_str(), animationName.c_str());
        return m_textures["ERROR"];
    }

    // Store the loaded texture
    m_characterAnimations[characterName][animationName] = texture;
    return texture;
}