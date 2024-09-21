﻿#include <cstdlib>
#include <algorithm>

#include "raylib.h"

#include "config.h"
#include "mainmenu.h"
#include "optionen.h"
#include "gameplay.h"
#include "pausescreen.h"
#include "journal.h"
#include "maincharacter.h"
#include "maincharactermodus.h"
#include "ingameoptions.h"
#include "assestmanagergraphics.h"
#include "scene.h"
#include "ENEMIES/Enemy.h"
#include "gamechoicescreen.h"
//#include "HealthManager.h"
#include "AudioPlayer.h"

int main() {
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h.in file
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(Game::ScreenWidth, Game::ScreenHeight, "Soul Steel");
    SetTargetFPS(60);
    assestmanagergraphics::init();
    //Jan
    InitAudioDevice();

#ifdef GAME_START_FULLSCREEN
    ToggleFullscreen();
#endif
//Jan
    SoundManager soundManager;

    //Loaded sounds and index numbers
    std::vector<std::string> filenames = {
            "assets/audio/sfx/RobottoSoultransition.wav",
            "assets/audio/sfx/Soultorobottransition.wav",
            "assets/audio/sfx/souldash.wav",
            "assets/audio/sfx/souldust.wav",
            "assets/audio/sfx/menuselectsound.wav",
            "assets/audio/sfx/Rockmove.wav",
            "assets/audio/sfx/placebomb.wav",
            "assets/audio/sfx/bombexplosion.wav",
            "assets/audio/sfx/RobotAttack.wav",
            "assets/audio/sfx/Soulsteel_Main_Theme.wav",

    };

    if (!soundManager.loadSounds(filenames)) {
        printf("sound load failed");
        return 1;
    }

    if (!soundManager.loadBackgroundMusic("assets/audio/sfx/Soulsteel_Factory_theme.mp3")) {
        printf("Failed to load Music.\n");
        return 1;
    }

    float volume = 0.5f;
    const float volumeChange = 0.1f;

    soundManager.setMusicVolume(volume);
    soundManager.playBackgroundMusic();


    // Your own initialization code here

    RenderTexture2D canvas = LoadRenderTexture(Game::ScreenWidth, Game::ScreenHeight);
    float renderScale{}; //those two are relevant to drawing and code-cleanliness
    Rectangle renderRec{};

    //set enums to the state they have when starting the game


    maincharactermodus modus =soulmodus;


    gameplay* gameplayInstance = nullptr;
    scene* currentScene =  new mainmenu();

    //std::cout << "Starting main loop\n";
//Jan
    bool spaceKeyPressedLastFrame = false;
    bool bombSoundPlayed = false;
    double bombSoundPlayTime = 0.0;

    //SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
    //ToggleFullscreen();

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_F)) { //Fullscreen logic.
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
                SetWindowSize(Game::ScreenWidth, Game::ScreenHeight);
            } else {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                ToggleFullscreen();
            }
        }


        //checks which screen is shown and calls the methods needed there
        currentScene->update();
        scene* newScene = currentScene->evaluateSceneChange();

        if (currentScene != newScene) {
            //std::cout << "Switching scenes\n";

            if (dynamic_cast<gameplay*>(newScene) != nullptr) {
                gameplayInstance = dynamic_cast<gameplay*>(newScene);
                if (currentScene != gameplayInstance) {
                    delete currentScene;
                }
                currentScene = gameplayInstance;
            } else if (dynamic_cast<gamechoicescreen*>(newScene) != nullptr) {
                if (gameplayInstance == nullptr) {
                    gameplayInstance = new gameplay();
                }
                delete currentScene;
                currentScene = new gamechoicescreen(gameplayInstance);
            } else if (dynamic_cast<gameplay*>(currentScene) != nullptr) {
                currentScene = newScene;
            } else {
                delete currentScene;
                currentScene = newScene;
            }
        }

        BeginDrawing();
        // You can draw on the screen between BeginDrawing() and EndDrawing()
        // For the letterbox we draw on canvas instead
        BeginTextureMode(canvas);
        { //Within this block is where we draw our app to the canvas.
            ClearBackground(BLACK);

            //checks which screen is shown and calls the methods needed there
           currentScene->draw();
        }
        EndTextureMode();
        //The following lines put the canvas in the middle of the window and have the negative as black
        ClearBackground(BLACK);
        renderScale = std::min(GetScreenHeight() /
                               (float) canvas.texture.height, //Calculates how big or small the canvas has to be rendered.
                               GetScreenWidth() / (float) canvas.texture.width);
        renderRec.width = canvas.texture.width * renderScale;
        renderRec.height = canvas.texture.height * renderScale;
        renderRec.x = (GetScreenWidth() - renderRec.width) / 2.0f;
        renderRec.y = (GetScreenHeight() - renderRec.height) / 2.0f;
        DrawTexturePro(canvas.texture, Rectangle{0, 0, (float) canvas.texture.width, (float) -canvas.texture.height},
                       renderRec,
                       {}, 0, WHITE);
        EndDrawing();


        //Jan
        if (!mainmenu::IsGameRunning) {
                if (IsKeyDown(KEY_A)) {
                    soundManager.playSound(4);
                }
                if (IsKeyDown(KEY_D)) {
                    soundManager.playSound(4);
                }
                if (IsKeyDown(KEY_LEFT)) {
                    soundManager.playSound(4);
                }
                if (IsKeyDown(KEY_RIGHT)) {
                    soundManager.playSound(4);
                }
        }
        if (mainmenu::IsGameRunning) {

            soundManager.updateBackgroundMusic();

            bool spaceKeyPressedThisFrame = IsKeyDown(KEY_SPACE);
            if (spaceKeyPressedThisFrame && !spaceKeyPressedLastFrame) {
                if (gameplayInstance) {
                    maincharactermodus modus = gameplayInstance->getCurrentModus();
                    if (modus == soulmodus) {
                        soundManager.playSound(0);
                    } else if (modus == robotmodus) {
                        soundManager.playSound(1);
                    }
                }
            }
            spaceKeyPressedLastFrame = spaceKeyPressedThisFrame;

            if (IsKeyDown(KEY_I)) {
                soundManager.playSound(2);
            }
            if (IsKeyDown(KEY_L)) {
                soundManager.playSound(3);
            }
            if (IsKeyPressed(KEY_EQUAL)) { // `+`
                volume += volumeChange;
                if (volume > 1.0f) volume = 1.0f; // Max volume
                soundManager.setMusicVolume(volume);
            }
            if (IsKeyPressed(KEY_MINUS)) { // `-`
                volume -= volumeChange;
                if (volume < 0.0f) volume = 0.0f; // Min volume
                soundManager.setMusicVolume(volume);
            }
            if (IsKeyPressed(KEY_U)) {
                if (gameplayInstance) {
                    maincharactermodus modus = gameplayInstance->getCurrentModus();
                    if (modus == robotmodus) {
                        soundManager.playSound(8);
                    }
                }
            }
            gameplayInstance->updateStones();  // stone update


            if (gameplayInstance->isStoneMoved) {
                soundManager.playSound(5);
                std::cout << "Stone moved, playing sound..." << std::endl;
                gameplayInstance->isStoneMoved = false;
            }

            if (IsKeyPressed(KEY_J)) {
                if (gameplayInstance) {
                    maincharactermodus modus = gameplayInstance->getCurrentModus();
                    if (modus == robotmodus) {
                        if (!bombSoundPlayed) {
                            soundManager.playSound(6);
                            bombSoundPlayTime = GetTime();
                            bombSoundPlayed = true;

                        }
                    }
                }
            }

// Check if 0.5 seconds have passed since the bomb sound was played
            if (bombSoundPlayed && (GetTime() - bombSoundPlayTime >= 1.5)) {
                soundManager.playSound(7);
                bombSoundPlayed = false;
            }

        }

    } // Main game loop end
    std::cout << "Exiting main loop\n";
    if (currentScene != gameplayInstance) {
        delete currentScene;
    }
    delete gameplayInstance;
    std::cout << "Cleanup complete\n";
    // De-initialization here
    // Close window and OpenGL context
    CloseWindow();

    //CloseAudioDevice();

    return EXIT_SUCCESS;
}


