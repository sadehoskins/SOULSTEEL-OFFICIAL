//
// Created by lrfri on 21.08.2024.
//

#include "gamechoicescreen.h"

void gamechoicescreen::update() {
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        if (cursor == 1) {
            cursor = 1;
        } else {
            cursor++;
        }
    }

    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        if (cursor == 0) {
            cursor = 0;
        } else {
            cursor--;
        }
    }
}

scene *gamechoicescreen::evaluateSceneChange() {
    if (IsKeyPressed(KEY_ENTER)) {
        switch (cursor) {
            case 0:  // New Game
                std::cout << "Starting new game\n";
                if (gameplayInstance) {
                    delete gameplayInstance;
                }
                gameplayInstance = new gameplay();
                return gameplayInstance;
            case 1:  // Continue
                std::cout << "Continuing existing game\n";
                return gameplayInstance;
        }
    }
    if(IsKeyPressed(KEY_M)){
        return new mainmenu();
    }else{return this;}
}

void gamechoicescreen::draw() {
    DrawTexture(background,0,0,WHITE);

    DrawTexture(logo_soulsteel, 120, 20, WHITE);
    DrawTexture(logo_team, 320, 390, WHITE);

    int button_height=240;
    switch (cursor) { //shows which box is selected by giving it a white outline
        case 0:
            DrawTexture(new_selected, 140,button_height, WHITE);
            DrawTexture(continue_texture, 485, button_height, WHITE);
            break;
        case 1:
            DrawTexture(new_texture, 140,button_height, WHITE);
            DrawTexture(continue_selected, 485, button_height, WHITE);
            break;
        default:
            break;
    }
    //draws the buttons
    //DrawRectangleRec(hitbox_newgame, GRAY);
    //DrawRectangleRec(hitbox_continue, GRAY);

    //draws the text on the buttons
    //DrawText("NEW GAME",140,360,30,BLACK);
    //DrawText("CONTINUE",485,360,30,BLACK);
}

void gamechoicescreen::drawDebug() {

}
