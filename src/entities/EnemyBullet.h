#pragma once
#include "core/Vector2.h"
#include "render/Renderer.h"

class EnemyBullet {
public:
    Vector2 pos;
    Vector2 dir;
    float speed = 500.f;
    float radius = 4.f;
    float life = 5.f;
    int damage = 1;

    EnemyBullet(const Vector2& p, const Vector2& d, int dmg)
        : pos(p), dir(normalize(d)), damage(dmg) {}

    // Trail für Nachzieheffekt
    std::vector<Vector2> trail;
    int maxTrailLength = 5;

    int headSize = 12; // Größe des großen Kopfes
    int trailSize = 4; // Größe der Trail-Teile

    bool update(float dt);
    void render(Renderer& r);
};



