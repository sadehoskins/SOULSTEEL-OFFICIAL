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

    DrawText("DAY 1", 32*4,20,25,BLACK);
    DrawText(day1text, 40, 70, 20, BLACK);
    DrawText("DAY 2", 32*12+16+32*3, 20, 25, BLACK);
    DrawText(day2text, 32*13, 70, 20, BLACK);
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

    DrawText("DAY 3", 32*4,20,25,BLACK);
    DrawText("The strange occurrences\ncontinue. A janitor\nreported seeing a living\ntoy, and the power\nin the factory keeps going\nout intermittently. It's almost\nas if the factory is\ncursed, or someone is\ntampering with the\ngenerator. I'm starting to\nget worried...", 40, 70, 20, BLACK);
}

void journal::drawpage2() {
    switch (cursor) { //shows which box is selected by giving it a white outline
        case 0:
            DrawRectangleRec(arrowback_marked, WHITE);
            break;
        case 1:
            DrawRectangleRec(arrowforward_marked, WHITE);
            break;
        default:
            break;
    }

    DrawRectangleRec(hitbox_close, GRAY);
    DrawRectangleRec(arrowback,GRAY);

    DrawTexture(closeButton, 685, 395, WHITE);

    DrawText("DAY 5", 32*4,20,25,BLACK);
    DrawText("The strange occurrences\ncontinue. A janitor\nreported seeing a living\ntoy, and the power\nin the factory keeps going\nout intermittently. It's almost\nas if the factory is\ncursed, or someone is\ntampering with the\ngenerator. I'm starting to\nget worried...", 40, 70, 20, BLACK);
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
            cursor=2;
        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            cursor=0;
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
                if (page < 2) {
                    page++;
                }
                break;
            case 2:  // Close button
                closejournal = true;
                break;
        }
    }
}

