//
// Created by sadeh on 08.09.2024.
//

#ifndef RAYLIBSTARTER_HEALTHMANAGER_H
#define RAYLIBSTARTER_HEALTHMANAGER_H


class HealthManager {
public:
    HealthManager(int maxHealth);

    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const;
    float getHealthPercentage() const;
    int getCurrentHealth() const;
    int getMaxHealth() const;

private:
    int maxHealth;
    int currentHealth;
};


#endif //RAYLIBSTARTER_HEALTHMANAGER_H
