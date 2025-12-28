#pragma once
#include <SDL.h>
#include "core/Vector2.h"
#include "entities/Player.h"

class Item {
public:
    Vector2 pos;
    float radius = 12.f;   // Pickup Radius
    float lifeTime = 10.f; // Lebenszeit, danach verschwinden
    SDL_Texture* texture = nullptr;
    int texWidth = 32;
    int texHeight = 32;

    bool isDead = false;

    virtual ~Item() = default;
    virtual void applyEffect(Player& player) = 0;

    void update(float dt) {
        lifeTime -= dt;
        if (lifeTime <= 0.f) isDead = true;
    }
};


