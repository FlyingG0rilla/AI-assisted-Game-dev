
#pragma once
#include "core/Vector2.h"
#include "render/Renderer.h"

class Bullet {
public:
    Vector2 pos;
    Vector2 dir;
    float speed = 500.f;
    float radius = 6.f;
    float life = 2.f;

    // Trail für Nachzieheffekt
    std::vector<Vector2> trail;
    int maxTrailLength = 5;

    int headSize = 12; // Größe des großen Kopfes
    int trailSize = 4; // Größe der Trail-Teile

    Bullet(Vector2 p, Vector2 d) : pos(p), dir(d) {}

    bool update(float dt);
    void render(Renderer& r);
};
