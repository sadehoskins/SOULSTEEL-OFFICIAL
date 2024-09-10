
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "raylib.h"
#include <string>
#include <vector>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    bool loadSounds(const std::vector<std::string>& filenames);
    void playSound(int index);
    void stopSound(int index);
    int getNumLoadedSounds() const;

    bool loadBackgroundMusic(const std::string& filename);
    void playBackgroundMusic();
    void updateBackgroundMusic();
    void stopBackgroundMusic();

private:
    std::vector<Sound> loadedSounds;
    Music backgroundMusic;
    bool isBackgroundMusicLoaded;
};

#endif // SOUNDMANAGER_H
