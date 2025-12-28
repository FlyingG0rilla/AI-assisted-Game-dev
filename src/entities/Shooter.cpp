#include "Shooter.h"
#include "core/Vector2.h"
#include "render/Renderer.h"
#include <iostream>
#include <cmath>
#include <cstdlib> // für rand()

Shooter::Shooter(const Vector2& startPos, float cooldown, std::vector<EnemyBullet>* enemyBullets, SDL_Texture* tx) {
    pos = startPos;
    this->startPos = startPos;
    speed = 0.f;
    radius = 16.f;
    damage = 1;
    scoreValue = 10;
    shootCooldown = cooldown;
    shootTimer = 0.f;
    bullets = enemyBullets;
    isDead = false;
    health = 3;

    texture = tx;
    if (texture)
        SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);
    else
        texWidth = texHeight = 20;

    // Zufällige Pendelparameter, damit Shooter nicht synchron sind
    moveAmplitudeX = 20.f + static_cast<float>(rand() % 31); // 20-50px
    moveAmplitudeY = 10.f + static_cast<float>(rand() % 21); // 10-30px
    moveSpeedX = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f; // 1-3 rad/s
    moveSpeedY = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;
    moveOffsetX = static_cast<float>(rand()) / RAND_MAX * 2.f * 3.1415f; // zufällige Phase
    moveOffsetY = static_cast<float>(rand()) / RAND_MAX * 2.f * 3.1415f;
}

void Shooter::update(float dt, const Player& player) {
    if (isDead) return;

    // Pendelbewegung mit Sinus
    moveOffsetX += dt * moveSpeedX;
    moveOffsetY += dt * moveSpeedY;

    pos.x = startPos.x + moveAmplitudeX * std::sin(moveOffsetX);
    pos.y = startPos.y + moveAmplitudeY * std::sin(moveOffsetY);


    shootTimer -= dt;
    if (shootTimer < 0.f) shootTimer = 0.f;


    if (shootTimer == 0.f && bullets) {
        Vector2 dir = normalize(player.pos - pos);
        bullets->emplace_back(pos, dir, damage);
        shootTimer = shootCooldown;

        // Debug
        std::cout << "Shooter at (" << pos.x << "," << pos.y << ") shoots!" << std::endl;
    }

    lookAngle = atan2(player.pos.y - pos.y, player.pos.x - pos.x) * 180.0 / M_PI - 90.0;

    updateDamageTimer(dt);
}

void Shooter::render(Renderer& r) {
    if (!isVisible()) return;

    if (texture)
        r.drawTexture(texture, pos, renderSize, renderSize, lookAngle);
    else
        r.drawRect(pos, 2*radius, {255,0,0,255});
}


