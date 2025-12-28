
#include "Runner.h"


Runner::Runner(Vector2 startPos, SDL_Texture* tx)
{
    pos = startPos;
    scoreValue = 5;
    speed = 120.f;
    radius = 16;
    damage = 1;
    health = 3;

    texture = tx;

    if (texture)
        SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);
    else
        texWidth = texHeight = 20; // Fallback, falls Texture nul
}


void Runner::update(float dt, const Player& player)
{
    pos = pos + normalize(player.pos - pos) * 120 * dt;
    Vector2 dir = normalize(player.pos - pos);
    lookAngle = atan2(dir.y, dir.x) * 180.0 / M_PI - 90.0;

    // Damage Timer runterzählen
    updateDamageTimer(dt);
}


void Runner::render(Renderer& r)
{
    if (!isVisible()) return; // Wenn damageTimer > 0, nicht rendern

    if (texture)
        r.drawTexture(texture, pos, renderSize, renderSize, lookAngle);
    else
        r.drawRect(pos, 2*radius, {255,0,0,255});

}
