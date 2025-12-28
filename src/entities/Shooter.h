#pragma once
#include "Enemy.h"
#include "core/Vector2.h"
#include "Player.h"
#include "EnemyBullet.h"
#include <vector>

class Renderer; // Forward Declaration

class Shooter : public Enemy {
public:
    float shootCooldown;                 // Sekunden zwischen Schüssen
    float shootTimer;                    // Timer für Schuss
    std::vector<EnemyBullet>* bullets;  // Zeiger auf Game::enemyBullets

    Shooter(const Vector2& startPos, float cooldown, std::vector<EnemyBullet>* enemyBullets, SDL_Texture* texture);

    void update(float dt, const Player& player) override;
    void render(Renderer& r) override;

private:
    float lookAngle = 0;


    Vector2 startPos;
    float moveAmplitudeX;
    float moveAmplitudeY;
    float moveSpeedX;
    float moveSpeedY;
    float moveOffsetX;
    float moveOffsetY;
};




