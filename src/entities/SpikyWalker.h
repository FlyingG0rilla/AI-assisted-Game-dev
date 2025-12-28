#pragma once
#include "Enemy.h"
#include "core/Vector2.h"
#include "Player.h"
#include "render/Renderer.h"

class SpikyWalker : public Enemy {
public:


    Vector2 target;

    SpikyWalker(const Vector2& startPos, const Vector2& targetPos, SDL_Texture* tx);

    void update(float dt, const Player& player) override;
    void render(Renderer& r) override;

private:
    Vector2 dir;
    float rotation = 0.f;
    float rotationSpeed = 180.f;
    SDL_Texture* texture = nullptr;
    float lookAngle = 0.f;

};





