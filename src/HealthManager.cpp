//
// Created by sadeh on 08.09.2024.
//

#include "HealthManager.h"
#include <algorithm>

HealthManager::HealthManager(int maxHealth) : maxHealth(maxHealth), currentHealth(maxHealth) {}

void HealthManager::takeDamage(int amount) {
    currentHealth = std::max(0, currentHealth - amount);
}

void HealthManager::heal(int amount) {
    currentHealth = std::min(maxHealth, currentHealth + amount);
}

bool HealthManager::isAlive() const {
    return currentHealth > 0;
}

float HealthManager::getHealthPercentage() const {
    return static_cast<float>(currentHealth) / maxHealth;
}

int HealthManager::getCurrentHealth() const {
    return currentHealth;
}

int HealthManager::getMaxHealth() const {
    return maxHealth;
}