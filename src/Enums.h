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

enum class ControlType {
    Path,
    Random
};

enum class ControlRandom {
    RandomStart,
    RandomWalk,
    RandomStop
};

enum class AnimationState {
    IDLE,
    WALK,
    ATTACK,
    SPECIAL,
    DASH,
    DUST,
    SWITCH
};

//enum AnimationFacingDirection {BACK,FRONT,LEFT,RIGHT};

#endif //RAYLIBSTARTER_ENUMS_H