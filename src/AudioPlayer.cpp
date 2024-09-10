#include "AudioPlayer.h"

SoundManager::SoundManager() : isBackgroundMusicLoaded(false) {}

SoundManager::~SoundManager() {
    for (auto& sound : loadedSounds) {
        UnloadSound(sound);
    }
    if (isBackgroundMusicLoaded) {
        UnloadMusicStream(backgroundMusic);
    }
}

bool SoundManager::loadSounds(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames) {
        Sound sound = LoadSound(filename.c_str());
        if (sound.stream.buffer == nullptr) {
            // If loading fails, clear all loaded sounds
            for (auto& loadedSound : loadedSounds) {
                UnloadSound(loadedSound);
            }
            return false;
        }
        loadedSounds.push_back(sound);
    }
    return true;
}

void SoundManager::playSound(int index) {
    if (index >= 0 && index < loadedSounds.size()) {
        PlaySound(loadedSounds[index]);
    }
}

void SoundManager::stopSound(int index) {
    if (index >= 0 && index < loadedSounds.size()) {
        StopSound(loadedSounds[index]);
    }
}

int SoundManager::getNumLoadedSounds() const {
    return loadedSounds.size();
}

bool SoundManager::loadBackgroundMusic(const std::string& filename) {
    backgroundMusic = LoadMusicStream(filename.c_str());
    if (backgroundMusic.stream.buffer == nullptr) {
        return false;
    }
    isBackgroundMusicLoaded = true;
    return true;
}

void SoundManager::playBackgroundMusic() {
    if (isBackgroundMusicLoaded) {
        PlayMusicStream(backgroundMusic);
    }
}

void SoundManager::updateBackgroundMusic() {
    if (isBackgroundMusicLoaded) {
        UpdateMusicStream(backgroundMusic);
    }
}

void SoundManager::stopBackgroundMusic() {
    if (isBackgroundMusicLoaded) {
        StopMusicStream(backgroundMusic);
    }
}
