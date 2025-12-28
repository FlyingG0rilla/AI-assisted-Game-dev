//
// Created by Dennis Schwab on 13.12.2025.
//
#pragma once
#include "Enemy.h"


class Runner : public Enemy {
public:
    Runner(Vector2 startPos, SDL_Texture* texture);


    float lookAngle;

    Runner();

    void update(float dt, const Player& player) override;
    void render(Renderer& r) override;
};



