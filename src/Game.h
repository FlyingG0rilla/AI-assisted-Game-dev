#pragma once

#include <SDL.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>

#include "core/Input.h"
#include "entities/Bullet.h"
#include "entities/Enemy.h"
#include "entities/Player.h"
#include "entities/Item.h"
#include "gameplay/Wavemanager.h"
#include "render/Renderer.h"

struct Star {
    Vector2 pos;
    float speed;
    int size;
};

struct Particle {
    Vector2 pos;
    Vector2 vel;   // Geschwindigkeit
    float life;    // verbleibende Lebenszeit
    SDL_Color color;
};

enum class GameState {
    TITLE,
    PLAYING,
    GAMEOVER
};

class Game {
public:
    // Konstruktor / Destruktor
    Game();

    void initTextures();

    ~Game();

    // Hauptloop
    void run();

private:
    // Input
    void processInput();

    // Update-Methoden
    void updateTitle();
    void updatePlaying(float dt);
    void updateGameOver();

    // Render-Methoden
    void renderTitle();
    void renderPlaying();
    void renderGameOver();

    // Hilfsmethoden
    void loadTextures();
    void spawnExplosion(Vector2 pos, int count = 20);

    // Fenster
    int windowWidth = 1020;
    int windowHeight = 720;

    // Spielstatus
    bool running = true;
    GameState state = GameState::TITLE;

    // Spieler / Gegner
    float shootCooldown = 0.3f;
    float shootTimer = 0.f;
    float damageCooldown = 0.5f;
    float damageTimer = 0.f;
    int score = 0;

    SDL_Window* window = nullptr;
    Input input;
    Renderer renderer;
    Player player;
    WaveManager waveManager;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Bullet> bullets;
    std::vector<EnemyBullet> enemyBullets;
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::vector<std::unique_ptr<Item>> items;

    // Partikel & Sterne
    std::vector<Particle> particles;
    std::vector<Star> stars;
    int numStars = 100;

    // UI Texturen
    SDL_Texture* texWASD = nullptr;
    SDL_Texture* texMouse = nullptr;


    // Hintergrund
    SDL_Texture* bgTexture = nullptr;
    int tileWidth = 512;
    int tileHeight = 512;
    float bgOffsetY = 0.01f;

    // Screen Shake
    float shakeTime = 0.f;
    float shakeDuration = 0.2f;
    float shakeStrength = 8.f;
    Vector2 shakeOffset{0, 0};
};
