//
// Created by sadeh on 01.07.2024.
//

#ifndef RAYLIBSTARTER_ENUMS_H
#define RAYLIBSTARTER_ENUMS_H



enum Direction {
    Down = 0,
    Up = 1,
    Right = 2,
    Left = 3,
};

enum class AnimationState {
    IDLE,               // General idle state for body animation
    WALK,               // General walk state for body animation
    ATTACK_NORMAL,      // General normal attack state for body animations
    ATTACK_RANGED,      // General ranged attack state for body animations

    DASH,               // Soul-specific dash attack
    DUST,               // Soul-specific dust attack
    SWITCH,             // Sou- and robot-specific switch animation

    // Effects that are drawn on top of the body

    NORMAL_EFFECT,      // special effect for normal robot attacks
    RANGED_EFFECT,      // special effect for ranged robot attacks
    BOMB_EFFECT,        // special effect for Teddy bomb attack

    //ranged attacks that don't have to be drawn on top of the body animation

    FLYINGSCREWS,        // special effect (throws screws) for robot ranged attack
    SPIDERTOOTH,         // Special effect (shoots tooth) for tackle spider
    LIT,                 // Special effect for bombs (bomb is lit)
    EXPLOTION           // Special effect for bombs (bomb explodes)

};

enum class ControlType {
    Path,
    Random
};

enum class ControlRandom {
    RandomStart,
    RandomWalk,
    RandomStop
};


//enum AnimationFacingDirection {BACK,FRONT,LEFT,RIGHT};

#endif //RAYLIBSTARTER_ENUMS_H