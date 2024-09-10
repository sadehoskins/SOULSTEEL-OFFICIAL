//
// Created by lrfri on 14.05.2024.
//
#include "journal.h"
#include "raylib.h"
#include "gameplay.h"
#include "mainmenu.h"

void journal::update() {
    //navigating the journal by using arrow keys
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
}

scene *journal::evaluateSceneChange() {
    if (IsKeyPressed(KEY_T)) {
        return gameplayInstance;
    }else if (IsKeyPressed(KEY_M)) {
        return new mainmenu();
    } else if (IsKeyPressed(KEY_ENTER)) {
        switch (cursor) {
            case 0:
                //no journal content yet
                break;
            case 1:
                //no journal content yet
                break;
            case 2:
                return gameplayInstance;
        }
    } else {
        return this;
    }
}

void journal::draw() {
    DrawTexture(journalbackground, 5, 5, WHITE);

    switch (cursor) { //shows which box is selected by giving it a white outline
        case 0:
            DrawRectangleRec(journalpage1_marked, WHITE);
            break;
        case 1:
            DrawRectangleRec(journalpage2_marked, WHITE);
            break;
        case 2:
            DrawRectangleRec(hitbox_close_marked, WHITE);
            break;
        default:
            break;
    }


    DrawRectangleRec(journalpage1, GRAY);
    DrawRectangleRec(journalpage2, GRAY);
    DrawRectangleRec(hitbox_close, GRAY);

    DrawTexture(closeButton, 685, 395, WHITE);

    DrawText("Journal content TBD", 45, 20, 23, BLACK);
    DrawText("Journal content TBD", 395, 20, 23, BLACK);
    DrawText("Eventually here\nwill be a journal\nwith different pages.", 60, 200, 20, BLACK);
    DrawText("At this time\nnothing happens on\nthese two pages.", 410, 200, 20, BLACK);

}

void journal::drawDebug() {

}
