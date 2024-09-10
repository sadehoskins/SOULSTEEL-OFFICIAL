//
// Created by lrfri on 26.08.2024.
//

#ifndef RAYLIBSTARTER_BLUEBLOCK_H
#define RAYLIBSTARTER_BLUEBLOCK_H

#include "movableblocks.h"
#include "../assestmanagergraphics.h"

class blueblock :public movableblocks {
public:
    blueblock(gameplay* scene, Vector2 position, bool isSwitch = false);

    void draw() override;

    void toggle() override;

    bool canWalkThrough() const;


private:
    static const int frame_count = 8;
    static const float animation_speed;
    Texture2D blueblock_turning_on;
    Texture2D blueblock_turning_off;
    Texture2D blueblock_texture;
    Texture2D blueswitch_on;
    Texture2D blueswitch_off;
    Texture2D switchTexture;


    void updateAnimation(float deltaTime) override;
};


#endif //RAYLIBSTARTER_BLUEBLOCK_H
