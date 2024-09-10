//
// Created by lrfri on 22.08.2024.
//

#ifndef RAYLIBSTARTER_STONE_H
#define RAYLIBSTARTER_STONE_H

#include <cmath>
#include "raylib.h"
#include "../assestmanagergraphics.h"
#include "gameobjects.h"
class gameplay;



class Stone : public gameobjects {
public:
    Stone(gameplay* scene, Vector2 initialPosition);
    void update() override;
    void draw() override;
    bool tryMove(Vector2 direction);
    void setScene(gameplay* scene) { _scene = scene; }
    gameplay* getScene() const { return _scene; }

    Vector2 getTilePosition() const { return {std::floor(position.x / 32), std::floor(position.y / 32)}; }
private:
    Texture2D stoneTexture;
};
#endif //RAYLIBSTARTER_STONE_H
