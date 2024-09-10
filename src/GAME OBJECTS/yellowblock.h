//
// Created by lrfri on 26.08.2024.
//

#ifndef RAYLIBSTARTER_YELLOWBLOCK_H
#define RAYLIBSTARTER_YELLOWBLOCK_H

#include "movableblocks.h"
#include "../assestmanagergraphics.h"

class yellowblock :public movableblocks {
public:
    yellowblock(gameplay* scene, Vector2 position, bool isSwitch = false);

    void draw() override;

    void toggle() override;

    bool canWalkThrough() const;

private:
    static const int frame_count = 8;
    static const float animation_speed;

    void updateAnimation(float deltaTime) override;

    Texture2D yellowblock_turning_on;
    Texture2D yellowblock_turning_off;
    Texture2D yellowblock_texture;
    Texture2D yellowswitch_on;
    Texture2D yellowswitch_off;
    Texture2D switchTexture;
};


#endif //RAYLIBSTARTER_YELLOWBLOCK_H
