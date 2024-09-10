//
// Created by lrfri on 19.06.2024.
//

#include "robot.h"
#include "../maincharacter.h"

void robot::update() {


}

void robot::draw() {

    DrawTexture(robotFront, position.x - 20, position.y - 15, WHITE);


}



robot::robot(gameplay *scene) {

}

robot::~robot() {}

