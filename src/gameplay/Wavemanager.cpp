#include "WaveManager.h"
#include "entities/Runner.h"
#include "entities/Shooter.h"
#include "entities/SpikyWalker.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "entities/AmmoItem.h"
#include "entities/HealthItem.h"
#include "entities/Item.h"

void WaveManager::update(float dt,
                         std::vector<std::unique_ptr<Enemy>>& enemies,
                         int windowWidth, int windowHeight,
                         int score,
                         std::vector<EnemyBullet>* enemyBullets,
                         std::vector<std::unique_ptr<Item>>& items,
                         std::unordered_map<std::string, SDL_Texture*>& textures)
{
    if (!enemies.empty())
        return;

    spawnTimer += dt;
    if (spawnTimer < spawnInterval)
        return;

    spawnTimer = 0.f;


    Vector2 spawnPos;
    spawnPos.x = rand() % windowWidth;
    spawnPos.y = rand() % windowHeight;

    int r = rand() % 10;
    if (r < 5)
        items.push_back(std::make_unique<HealthItem>(spawnPos, textures["Health"], 1, 10.f));

    else if (r < 10)
        items.push_back(std::make_unique<AmmoItem>(spawnPos, textures["Ammo"], 1, 10.f));


    // Gegneranzahl pro Welle
    int baseEnemies = 2 + (wave - 1) * 3;
    int spawnedShooters = 0;
    int spawnedSpiky = 0;
    int maxShooters = baseEnemies / 2;
    int maxSpiky = baseEnemies / 3;

    for (int i = 0; i < baseEnemies; ++i)
    {
        int type;
        int r = rand() % 100;

        // Wahrscheinlichkeiten: Runner > SpikyWalker > Shooter
        if (r < 55)
        {
            type = 0; // Runner
        }
        else if (r < 85 && spawnedSpiky < maxSpiky)
        {
            type = 2; // SpikyWalker
            spawnedSpiky++;
        }
        else if (spawnedShooters < maxShooters)
        {
            type = 1; // Shooter
            spawnedShooters++;
        }
        else
        {
            type = 0;
        }

        Vector2 spawnPos;

        // Spawnlogik
        if (type == 0)
        {
            int margin = 10;
            int side = rand() % 4;
            switch(side) //Runner
            {
                case 0: spawnPos = { float(rand() % windowWidth), float(margin) }; break;   // oben
                case 1: spawnPos = { float(rand() % windowWidth), float(windowHeight - margin) }; break; // unten
                case 2: spawnPos = { float(margin), float(rand() % windowHeight) }; break; // links
                case 3: spawnPos = { float(windowWidth - margin), float(rand() % windowHeight) }; break; // rechts
            }

            SDL_Texture* runnerTex = textures["Runner"];
            enemies.push_back(std::make_unique<Runner>(spawnPos, runnerTex));
        }
        else if (type == 1) // Shooter
        {
            int margin = 100;
            int side = rand() % 4;
            switch(side)
            {
                case 0: spawnPos = { float(margin + rand() % (windowWidth - 2*margin)), float(margin) }; break;
                case 1: spawnPos = { float(margin + rand() % (windowWidth - 2*margin)), float(windowHeight - margin) }; break;
                case 2: spawnPos = { float(margin), float(margin + rand() % (windowHeight - 2*margin)) }; break;
                case 3: spawnPos = { float(windowWidth - margin), float(margin + rand() % (windowHeight - 2*margin)) }; break;
            }
            float cooldown = 2.f;
            SDL_Texture* shooterTex = textures["Shooter"];
            enemies.push_back(std::make_unique<Shooter>(spawnPos, cooldown, enemyBullets, shooterTex));
        }
        else if (type == 2) // SpikyWalker
        {
            int margin = 10;
            int side = rand() % 4;
            switch(side)
            {
                case 0: spawnPos = { float(rand() % windowWidth), float(margin) }; break;   // oben
                case 1: spawnPos = { float(rand() % windowWidth), float(windowHeight - margin) }; break; // unten
                case 2: spawnPos = { float(margin), float(rand() % windowHeight) }; break; // links
                case 3: spawnPos = { float(windowWidth - margin), float(rand() % windowHeight) }; break; // rechts
            }
            Vector2 center{ windowWidth/2.f, windowHeight/2.f };
            SDL_Texture* spikyTex = textures["SpikyWalker"];
            enemies.push_back(std::make_unique<SpikyWalker>(spawnPos, center, spikyTex));
        }
    }

    wave++;
    spawnInterval = std::max(0.5f, spawnInterval - 0.05f);
}
