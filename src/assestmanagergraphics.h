//
// Created by sweet on 29.05.2024.
//

#ifndef RAYLIBSTARTER_ASSESTMANAGERGRAPHICS_H
#define RAYLIBSTARTER_ASSESTMANAGERGRAPHICS_H


#include <map>
#include "raylib.h"
#include <string>

class assestmanagergraphics {
public:
    static void init();
    static Texture2D getTexture(const std::string &name);
    static Texture2D getCharacterTexture(const std::string& characterName, const std::string& animationName);

private:
    static void loadCharacterAnimations(const std::string& characterName);
    static bool m_texturesLoaded;
    static std::map<std::string, Texture2D> m_textures;
    static std::map<std::string, std::map<std::string, Texture2D>> m_characterAnimations;
};


#endif //RAYLIBSTARTER_ASSESTMANAGERGRAPHICS_H
