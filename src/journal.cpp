//
// Created by lrfri on 14.05.2024.
//
#include "journal.h"
#include "raylib.h"
#include "gameplay.h"
#include "mainmenu.h"

void journal::update() {
    journalnavigation();
    flippingpages();
}

scene *journal::evaluateSceneChange() {
    if (IsKeyPressed(KEY_T)) {
        return gameplayInstance;
    } else if (IsKeyPressed(KEY_M)) {
        return new mainmenu();
    } else if (closejournal) {
        closejournal = false;  // Reset the bool
        return gameplayInstance;
    } else {
        return this;
    }
}

void journal::draw() {
    DrawTexture(journalbackground, 5, 5, WHITE);

    switch(page){
        case 0:
            drawpage0();
            break;
        case 1:
            drawpage1();
            break;
        case 2:
            drawpage2();
            break;
        case 3:
            drawpage3();
    }
}

void journal::drawpage0() {
    switch (cursor) { //shows which box is selected by giving it a white outline
        case 1:
            DrawRectangleRec(arrowforward_marked, WHITE);
            break;
        case 2:
            DrawRectangleRec(hitbox_close_marked, WHITE);
            break;
        default:
            break;
    }
    DrawRectangleRec(arrowforward, GRAY);
    DrawRectangleRec(hitbox_close, GRAY);

    DrawTexture(closeButton, 685, 395, WHITE);

    DrawText("DAY 1", dayposleft,20,25,BLACK);
    DrawText(day1text, textposleft, 70, 20, BLACK);
    DrawText("DAY 2", dayposright, 20, 25, BLACK);
    DrawText(day2text, textposright, 70, 20, BLACK);
}

void journal::drawpage1() {
    switch (cursor) { //shows which box is selected by giving it a white outline
        case 0:
            DrawRectangleRec(arrowback_marked, WHITE);
            break;
        case 1:
            DrawRectangleRec(arrowforward_marked, WHITE);
            break;
        case 2:
            DrawRectangleRec(hitbox_close_marked, WHITE);
            break;
        default:
            break;
    }
    DrawRectangleRec(arrowforward, GRAY);
    DrawRectangleRec(hitbox_close, GRAY);
    DrawRectangleRec(arrowback,GRAY);

    DrawTexture(closeButton, 685, 395, WHITE);

    DrawText("DAY 3", dayposleft,20,25,BLACK);
    DrawText(day3text, textposleft, 70, 20, BLACK);
    DrawText("some image, maybe teddy enemy?", 32*12,250,20, BLACK);
    //DrawTexture(texture, 32*17, 100, WHITE);
}

void journal::drawpage2() {
    switch (cursor) { //shows which box is selected by giving it a white outline
        case 0:
            DrawRectangleRec(arrowback_marked, WHITE);
            break;
        case 1:
            DrawRectangleRec(arrowforward_marked, WHITE);
            break;
        case 2:
            DrawRectangleRec(hitbox_close_marked, WHITE);
            break;
        default:
            break;
    }
    DrawRectangleRec(arrowforward, GRAY);
    DrawRectangleRec(hitbox_close, GRAY);
    DrawRectangleRec(arrowback,GRAY);

    DrawTexture(closeButton, 685, 395, WHITE);
    DrawText("DAY 4",dayposleft , 20, 25, BLACK);
    DrawText(day4text1, textposleft, 70, 20, BLACK);
    DrawText(day4text2, textposright, 70, 20, BLACK);
}
void journal::drawpage3() {
    switch (cursor) { //shows which box is selected by giving it a white outline
        case 0:
            DrawRectangleRec(arrowback_marked, WHITE);
            break;
        case 2:
            DrawRectangleRec(hitbox_close_marked, WHITE);
            break;
        default:
            break;
    }

    DrawRectangleRec(hitbox_close, GRAY);
    DrawRectangleRec(arrowback,GRAY);

    DrawTexture(closeButton, 685, 395, WHITE);
    DrawText("DAY 5", dayposleft,20,25,BLACK);
    DrawText(day5text, textposleft, 70, 20, BLACK);
}



void journal::drawDebug() {

}

void journal::journalnavigation() {
//navigating the journal by using arrow keys

switch(page){
    case 0:
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            if (cursor == 2) {
                cursor = 2; //ends with last box, does not come back on the other side of the screen
            } else {
                cursor++;
            }
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            if (cursor == 1) {
                cursor = 1; //same here: you can't go more the side if you're already on the last box
            } else {
                cursor--;
            }
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            cursor = 2;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            cursor = 1;
        }
        break;
    case 1:
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            if (cursor == 2) {
                cursor = 2; //ends with last box, does not come back on the other side of the screen
            } else {
                cursor++;
            }
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            if (cursor == 0) {
                cursor = 0; //same here: you can't go more the side if you're already on the last box
            } else {
                cursor--;
            }
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            cursor = 2;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            cursor = 1;
        }
        break;
    case 2:
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            if (cursor == 2) {
                cursor = 2; //ends with last box, does not come back on the other side of the screen
            } else {
                cursor++;
            }
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            if (cursor == 0) {
                cursor = 0; //same here: you can't go more the side if you're already on the last box
            } else {
                cursor--;
            }
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            cursor = 2;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            cursor = 1;
        }
        break;
    case 3:
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            if (cursor == 2) {
                cursor = 2; //ends with last box, does not come back on the other side of the screen
            } else {
                cursor=2;
            }
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            if (cursor == 0) {
                cursor = 0; //same here: you can't go more the side if you're already on the last box
            } else {
                cursor=0;
            }
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            cursor = 2;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            cursor = 0;
        }
        break;
}
}

void journal::flippingpages() {

    if (IsKeyPressed(KEY_ENTER)) {
        switch (cursor) {
            case 0:  //arrow to the left
                if (page > 0) {
                    page--;
                }
                break;
            case 1:  //arrow to the right
                if (page < 3) {
                    page++;
                }
                break;
            case 2:  // Close button
                closejournal = true;
                break;
        }
    }
}

