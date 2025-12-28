#pragma once
#include "Item.h"

class HealthItem : public Item {
public:
    int healthAmount = 1;

    HealthItem(Vector2 pos_, SDL_Texture* tex = nullptr, int health = 1, float life = 10.f) {
        pos = pos_;
        healthAmount = health;
        lifeTime = life;
        texture = tex;
    }

    void applyEffect(Player& player) override {
        player.health += healthAmount;
        if (player.health > player.maxHealth) player.health = player.maxHealth;
        isDead = true;
    }
};
