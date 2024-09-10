//
// Created by lrfri on 19.06.2024.
//

#ifndef RAYLIBSTARTER_ROBOT_H
#define RAYLIBSTARTER_ROBOT_H

#include "gameobjects.h"
#include "../assestmanagergraphics.h"
#include "../maincharactermodus.h"

class robot {
private:
    Texture2D robotFront = assestmanagergraphics::getTexture("placeholder/standby-robot");
public:
    void update();
    void draw();


    float size=12;
    float stepsize=4;

    //looking direction
    enum lookingdirection{
        north, south, east, west
    };
    lookingdirection lookingdirection =south;

    Vector2 position;
    robot(gameplay*scene);


    gameplay* _scene;

    ~robot();
};


#endif //RAYLIBSTARTER_ROBOT_H
