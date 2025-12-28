#include "SpikyWalker.h"
#include <cmath>

SpikyWalker::SpikyWalker(const Vector2& startPos, const Vector2& targetPos, SDL_Texture* tx) {
    pos = startPos;
    target = targetPos;
    speed = 60.f;
    radius = 16.f;
    damage = 0;
    health = 1;
    scoreValue = 2;
    isDead = false;

    // Richtung zur Mitte berechnen
    dir = normalize(targetPos - pos);


    texture = tx;
    if (texture)
        SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);
    else
        texWidth = texHeight = 20; // Fallback, falls Texture nul
}

void SpikyWalker::update(float dt, const Player& player) {

    pos = pos + dir * speed * dt;
    lookAngle = atan2(dir.y, dir.x) * 180.0 / M_PI - 90.0;
    updateDamageTimer(dt);
}

void SpikyWalker::render(Renderer& r) {

    if (!isVisible()) return;
    if (texture)
        r.drawTexture(texture, pos, renderSize, renderSize, lookAngle);
    else
        r.drawRect(pos, 2*radius, {255,0,0,255});
}
