
#pragma once
#include <vector>

#include "core/Vector2.h"
#include "render/Renderer.h"
#include "entities/Player.h"


class Enemy {
public:
    Vector2 pos;
    float speed;
    float radius;
    int damage;
    int scoreValue;
    bool isDead = false;
    int health;


    float damageTimer = 0.f;
    void takeDamage(int amount) {
        health -= amount;
        damageTimer = 0.05f;
        if (health <= 0) isDead = true;
    }

    void updateDamageTimer(float dt) {
        if (damageTimer > 0.f) damageTimer -= dt;
    }

    bool isVisible() const { return damageTimer <= 0.f; }

    std::vector<SDL_Texture*> textures;
    SDL_Texture* texture = nullptr;
    int texWidth = 0;
    int texHeight = 0;

    virtual ~Enemy() = default;
    virtual void update(float dt, const Player& player) = 0;
    virtual void render(Renderer& r) = 0;

    int renderSize = 64; // Standardgröße, kann jeder Unterklasse überschreiben
};