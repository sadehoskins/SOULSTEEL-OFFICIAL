//
// Created by sweet on 29.05.2024.
//

#ifndef RAYLIBSTARTER_ASSESTMANAGERGRAPHICS_H
#define RAYLIBSTARTER_ASSESTMANAGERGRAPHICS_H


#include <map>
#include "raylib.h"
#include <string>
#include "Enums.h"

class assestmanagergraphics {
public:
    static void init();
    static Texture2D getTexture(const std::string &name);
    static Texture2D getAnimationTexture(const std::string& entityType, AnimationState state, Direction direction);
    static Texture2D getObjectTexture(const std::string& objectName, AnimationState state);
    static Texture2D getSingleFrameTexture(const std::string& name);

private:
    static void loadAnimations();
    static void loadCharacterAnimations();
    static void loadEnemyAnimations();
    static void loadObjectAnimations();
    static void loadSingleFrameTextures();
    static bool m_texturesLoaded;
    static std::map<std::string, Texture2D> m_textures;
    static std::map<std::string, std::map<AnimationState, std::map<Direction, Texture2D>>> m_animations;
    static std::map<std::string, std::map<AnimationState, Texture2D>> m_objectAnimations;
    static std::map<std::string, Texture2D> m_singleFrameTextures;

};
Texture2D LoadTextureAndLog(const std::string& path);

#endif //RAYLIBSTARTER_ASSESTMANAGERGRAPHICS_H
