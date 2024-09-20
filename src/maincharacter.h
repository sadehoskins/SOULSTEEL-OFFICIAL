//
// Created by lrfri on 20.05.2024.
//

#ifndef RAYLIBSTARTER_MAINCHARACTER_H
#define RAYLIBSTARTER_MAINCHARACTER_H

#include "raylib.h"
#include <vector>
#include "raymath.h"
#include "maincharactermodus.h"
#include <iostream>
#include "assestmanagergraphics.h"
#include "gameplay.h"
#include "Utils.h"
#include "ENEMIES/Enemy.h"
#include "GAME OBJECTS/gameobjects.h"
#include "GAME OBJECTS/bombs.h"
#include "Enums.h"
#include "HealthManager.h"
//


struct ActivatedFirebowls;

class gameplay;
class Enemy;
class bombs;

class maincharacter {

public:

    AnimationState currentState = AnimationState::IDLE;
    Direction currentDirection = Direction::Down;
    Direction intendedDirection = Direction::Down;
    Direction lookingdirection = Direction::Down;

    void update();
    void draw();
    void maincharacterwalking();
    void drawrobot();
    void drawsoul();
    void souldash();
    void souldust();
    void collisionwall();
    void collisionenemies();
    void collisionbars();
    void collisionabyss();
    void startDusting();
    void updateDustAnimation(float deltaTime);
    void drawDustAnimation();
    bool canSwitchToRobot() const;
    void performMeleeAttack();

    //*NEWCODE*
    //HealthManager healthManager;

    // Abyss Message Timer
    float abyssMessageTimer = 0.0f;
    static constexpr float ABYSS_MESSAGE_DURATION = 3.0f;

    //recovery timer
    static float recoveryTimer;
    float recoveryPeriod;
    static constexpr float IMMUNITY_DURATION = 1.0f;

    //health & Damage
    int health = 10;
    static int attackPower;

    //code for idle animation
    static const int FRAME_COUNT = 8;
    static const float FRAME_DURATION;
    static const float DASH_ANIMATION_SPEED;
    static constexpr float DASH_DISTANCE = 150.0f;  // Adjust this value to change dash distance
    static constexpr int DASH_FRAME_COUNT = 8;      // Number of frames in dash animation
    float dashProgress = 0.0f;
    Vector2 dashStartPosition;
    Vector2 dashEndPosition;

    Rectangle frameRec;
    int currentFrame;
    float frameCounter;


    void switchCharacter() {
        currentState = AnimationState::SWITCH;
        // Rest of the switching logic
    }

    void updateAnimation(float deltaTime);
    void updateDashAnimation(float deltaTime);
    bool isDashing() const {return currentState == AnimationState::DASH;}

    int getHealth(const maincharacter& maincharacter);
    void calculateDamage(maincharacter& maincharacter, int damage);

    //attack
    void setAttackPower(int attack);
    void attack(Enemy* target);


    //movement
    int stepsize =4;
    //int stepsizesouldash =5;
    bool souldashactivated=false;
    gameplay* _scene;


    // possessed Status
    bool isCharacterPossessed() const;
    void setPossessionStatus(bool possessed);

    //Character map interaction stuff
    maincharacter(gameplay *scene);
    maincharactermodus currentmodus =soulmodus;
    Vector2 position = {12*32, 6*32};
    float size = 12;
    //stones
    void interactWithStone();
    void collisionstone();



    //Collision
    Rectangle getCollisionRectangle() const;

    //souldust
    /*
    Texture2D souldust = assestmanagergraphics::getTexture("item/souldust");
    bool souldustactivated1 =false;
    bool souldustactivated2 =false;
     */
    bool felldown=false;

    //bombs
    void throwBomb();
    float lastBombThrowTime;
    static const float bomb_cooldown;
    float bombthrowing_range=48;

    //static const int MAX_HEALTH = 10;
    //void takeDamage(int amount);
    //void heal(int amount);
    //bool isAlive() const;
    //float getHealthPercentage() const;

protected:
    bool checkCollision(const Wall& wall);

private:
    Texture2D getCurrentTexture();
    //Texture2D characterSoulTexture = assestmanagergraphics::getTexture("characters/soul/Idle_Animation_front");
    Texture2D characterRobotTexture = assestmanagergraphics::getTexture("characters/robot/Character_-_Robot_-_Idle_Front_-_animated");
    Vector2 lastSafePosition;
    void updateLastSafePosition();
    bool souldustcanbeused() const;
    bool isPossessed;
    bombs* activeBomb;
    bool isDusting;
    float dustAnimationTimer;
    Vector2 dustPosition;
    std::string currentDustAnimation;
    static constexpr float DUST_ANIMATION_DURATION = 3.0f; // Adjust as needed
    static constexpr int DUST_FRAME_COUNT = 8; // Adjust based on your sprite sheet

    //animation
    AnimationState currentAnimationState;
    //AnimationFacingDirection facingDirection;

    //switch
    bool isSwitching = false;
    float switchAnimationTimer = 0.0f;
    static constexpr float SWITCH_ANIMATION_DURATION = 1.0f; // Adjust as needed
    static constexpr int SWITCH_FRAME_COUNT = 8;

    //immunity

    float immunityTimer = 0.0f;

    //melee attack
    bool isAttacking;
    float attackTimer;
    static const float ATTACK_DURATION;
    static const float ATTACK_COOLDOWN;
    float lastAttackTime;

};


#endif //RAYLIBSTARTER_MAINCHARACTER_H
