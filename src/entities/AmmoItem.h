#pragma once
#include "Item.h"

class AmmoItem : public Item {
public:
    int ammoAmount = 1;

    AmmoItem(Vector2 pos_, SDL_Texture* tex = nullptr, int ammo = 1, float life = 10.f) {
        pos = pos_;
        ammoAmount = ammo;
        lifeTime = life;
        texture = tex;
    }

    void applyEffect(Player& player) override {
        player.damage += ammoAmount;
        isDead = true;
    }
};

