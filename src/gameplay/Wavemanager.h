#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "entities/Enemy.h"
#include "entities/EnemyBullet.h"
#include "entities/Item.h"

class WaveManager {
public:
    float spawnTimer = 0.f;
    float spawnInterval = 2.f;
    int wave = 1;

    void update(float dt,
                std::vector<std::unique_ptr<Enemy>>& enemies,
                int windowWidth, int windowHeight,
                int score,
                std::vector<EnemyBullet>* enemyBullets,
                std::vector<std::unique_ptr<Item>>& items,
                std::unordered_map<std::string, SDL_Texture*>& textures);
};

