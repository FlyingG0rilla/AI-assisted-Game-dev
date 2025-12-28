#include "EnemyBullet.h"

bool EnemyBullet::update(float dt) {
    pos = pos + dir * speed * dt;


    // Trail updaten
    trail.push_back(pos);
    if (trail.size() > maxTrailLength)
        trail.erase(trail.begin());

    life -= dt;
    return life > 0.f;
}

void EnemyBullet::render(Renderer& r) {
    //r.drawRect(pos, radius * 2, {255, 0, 255, 255}); // Lila Farbe

    SDL_Renderer* sdlRenderer = r.getSDLRenderer();
    if (!sdlRenderer) return;

    // Trail rendern (kleiner & transparenter)
    for (size_t i = 0; i < trail.size(); ++i) {
        float alpha = 255.f * (i + 1) / trail.size(); // hinten transparenter
        SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, static_cast<Uint8>(alpha));
        SDL_Rect t{ static_cast<int>(trail[i].x - trailSize / 2),
                    static_cast<int>(trail[i].y - trailSize / 2),
                    trailSize,
                    trailSize };
        SDL_RenderFillRect(sdlRenderer, &t);
    }

    // Kopf rendern (groß & voll)
    SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 255);
    SDL_Rect head{ static_cast<int>(pos.x - headSize / 2),
                   static_cast<int>(pos.y - headSize / 2),
                   headSize,
                   headSize };
    SDL_RenderFillRect(sdlRenderer, &head);
}
