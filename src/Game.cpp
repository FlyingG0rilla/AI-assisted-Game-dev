
#include "Game.h"
#include "render/Renderer.h"
#include <iostream>


Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("WaveGame",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight  , 0);

    if (!renderer.init(window, "assets/font.ttf", 24)) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return; // oder exit(1);
    }


 initTextures();

    for (int i = 0; i < numStars; ++i) {
        Star s;
        s.pos = { float(rand() % windowWidth), float(rand() % windowHeight) };
        s.speed = 20.f + static_cast<float>(rand() % 40); // zufällige Geschwindigkeit
        s.size = 1 + rand() % 3; // zufällige Größe
        stars.push_back(s);
    }

}

void Game::initTextures() {
    //gegner Texturen
    textures["Runner"]  = renderer.loadTexture("assets/enemies/runner.png");
    textures["Shooter"] = renderer.loadTexture("assets/enemies/shooter.png");
    textures["SpikyWalker"]  = renderer.loadTexture("assets/enemies/spikyWalker.png");

    //Item texturen
    textures["Ammo"]  = renderer.loadTexture("assets/ammo.png");
    textures["Health"]  = renderer.loadTexture("assets/health.png");


    // Spieler-Textur
    player.texIdle  = renderer.loadTexture("assets/player/idle.png");
    player.texLeft  = renderer.loadTexture("assets/player/left.png");
    player.texRight = renderer.loadTexture("assets/player/right.png");

    SDL_SetTextureBlendMode(player.texIdle,  SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(player.texLeft,  SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(player.texRight, SDL_BLENDMODE_BLEND);


    //Menü texturen
    texWASD  = renderer.loadTexture("assets/ui/wasd.png");
    texMouse = renderer.loadTexture("assets/ui/mouse.png");

    //Hintergrund
    bgTexture = renderer.loadTexture("assets/ground4.png");
}

Game::~Game() {
    SDL_DestroyWindow(window);
    SDL_Quit();

    for (auto& [key, tex] : textures) {
        if (tex) SDL_DestroyTexture(tex);
    }

    if (texWASD)  SDL_DestroyTexture(texWASD);
    if (texMouse) SDL_DestroyTexture(texMouse);


}

void Game::run()
{
    Uint32 last = SDL_GetTicks();


    while (running)
    {
        Uint32 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;

        processInput();

        renderer.updateShake(dt);

        switch(state) {
            case GameState::TITLE: updateTitle(); break;
            case GameState::PLAYING: updatePlaying(dt); break;
            case GameState::GAMEOVER: updateGameOver(); break;
        }


        switch(state) {
            case GameState::TITLE: renderTitle(); break;
            case GameState::PLAYING: renderPlaying(); break;
            case GameState::GAMEOVER: renderGameOver(); break;
        }
    }
}


void Game::processInput()
{
    input.update(running);
}

void Game::updateTitle() {
    if (input.space) {
        state = GameState::PLAYING;
        player = Player();

        initTextures();



        enemies.clear();
        bullets.clear();
        enemyBullets.clear();
        score = 0;
        damageTimer = 0.f;
        shootTimer = 0.f;
        waveManager.wave = 1;
        player.health = player.maxHealth;
    }

    float dt = 1.0f / 60.0f; //Frames
    for (auto& s : stars) {
        s.pos.y += s.speed * dt;
        if (s.pos.y > windowHeight) {
            s.pos.y = 0.f;
            s.pos.x = float(rand() % windowWidth);
        }
    }


}

void Game::updatePlaying(float dt){

    // Spieler update
    player.update(dt, input, windowWidth, windowHeight);

    // WaveManager update
    waveManager.update(dt, enemies, windowWidth, windowHeight, score, &enemyBullets, items, textures);

    // Update Gegner-Bullets
    for (auto& b : enemyBullets) b.update(dt);
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                       [](const EnemyBullet& b){ return b.life <= 0.f; }),
        enemyBullets.end());


    // Damage-Cooldown runterzählen
    if (damageTimer > 0.f)
        damageTimer -= dt;

    // Gegner updaten
    for (auto& e : enemies)
        e->update(dt, player);

    // Gegner außerhalb des Spielfeldes markieren
    for (auto& e : enemies)
    {
        if (e->pos.x + e->radius < 0 || e->pos.x - e->radius > windowWidth ||
            e->pos.y + e->radius < 0 || e->pos.y - e->radius > windowHeight)
        {
            e->isDead = true;
        }
    }

    // Spieler <-> Gegner Kollision
    if (damageTimer <= 0.f)
    {
        for (auto& e : enemies)
        {
            Vector2 diff = e->pos - player.pos;
            float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);
            float collisionDist = e->radius + 12.f; // Spieler-Radius

            if (dist < collisionDist)
            {
                player.takeDamage(e->damage);
                renderer.startShake(15.f, 0.3f); // Stärke 5 Pixel, Dauer 0.3s
                shakeTime = shakeDuration;
                damageTimer = damageCooldown;
                std::cout << "Player hit! Health=" << player.health << std::endl;
                break;
            }
        }
    }

    // Spieler <-> Gegner Projektiel Kollision
    for (auto& b : enemyBullets)
    {
        Vector2 diff = b.pos - player.pos;
        float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);
        float collisionDist = b.radius + 12.f; // Spieler-Radius

        if (dist < collisionDist) {
            player.takeDamage(b.damage);
            renderer.startShake(15.f, 0.3f); // Stärke 5 Pixel, Dauer 0.3s
            shakeTime = shakeDuration;
            b.life = 0.f;
            std::cout << "Player hit by EnemyBullet! Health=" << player.health << std::endl;
        }
    }

    // Items updaten (Lebenszeit)
    for (auto& item : items)
        item->update(dt);

    // Items einsammeln prüfen
    for (auto& item : items) {
        Vector2 diff = item->pos - player.pos;
        float dist = sqrt(diff.x*diff.x + diff.y*diff.y);
        float playerradius = 12.f;
        if (dist < item->radius + playerradius) {
            item->applyEffect(player);
        }
    }

    // Items entfernen, die tot sind
    items.erase(
        std::remove_if(items.begin(), items.end(),
                       [](const std::unique_ptr<Item>& item){ return item->isDead; }),
        items.end()
    );;

    // Spieler-Shoot-Cooldown
    shootTimer -= dt;
    if (shootTimer < 0.f) shootTimer = 0.f;

    // Spieler schießt
    if (input.shoot && shootTimer == 0.f)
    {
        Vector2 mouse{ (float)input.mouseX, (float)input.mouseY };
        Vector2 dir = normalize(mouse - player.pos);
        bullets.emplace_back(player.pos, dir);
        shootTimer = shootCooldown;
    }

    //  Spieler-Bullets bewegen
    for (auto& b : bullets)
    {
        b.update(dt);
    }

    // Spieler-Bullets außerhalb des Spielfeldes entfernen
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [&](const Bullet& b)
            {
                return b.pos.x < 0 || b.pos.x > windowWidth ||
                       b.pos.y < 0 || b.pos.y > windowHeight;
            }),
        bullets.end()
    );

    //  Kollision Spieler-Bullets ↔ Gegner
    for (auto& b : bullets)
    {
        for (auto& e : enemies)
        {
            float dist = length(e->pos - b.pos);
            if (dist < b.radius + e->radius)
            {

                e->takeDamage(1);

                b.life = 0.f;
                break;
            }
        }
    }

    // Spieler-Bullets endgültig löschen
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b){ return b.life <= 0.f; }),
        bullets.end()
    );

    // Gegner endgültig entfernen & Punkte vergeben
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [&](const std::unique_ptr<Enemy>& e)
            {
                if (e->isDead)
                {
                    score += e->scoreValue;
                    spawnExplosion(e->pos); // ← Explosion erzeugen
                    return true;
                }
                return false;
            }),
        enemies.end()
    );


    // Partikel bewegen
    for (auto& p : particles) {
        p.pos.x += p.vel.x * dt;
        p.pos.y += p.vel.y * dt;
        p.life -= dt;
    }

    // tote Partikel entfernen
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle& p){ return p.life <= 0.f; }),
        particles.end()
    );


    // GameOver prüfen
    if (player.health <= 0)
    {
        state = GameState::GAMEOVER;
    }

}

void Game::updateGameOver() {
    if (input.space) {
        state = GameState::PLAYING;
        player = Player();

        initTextures();



        enemies.clear();
        bullets.clear();
        enemyBullets.clear();
        items.clear();
        score = 0;
        damageTimer = 0.f;
        shootTimer = 0.f;
        waveManager.wave = 1;
        player.health = player.maxHealth;
    }

    float dt = 1.0f / 60.0f;
    for (auto& s : stars) {
        s.pos.y += s.speed * dt;
        if (s.pos.y > windowHeight) {
            s.pos.y = 0.f;
            s.pos.x = float(rand() % windowWidth);
        }
    }
}



void Game::spawnExplosion(Vector2 pos, int count) {
    for (int i = 0; i < count; ++i) {
        float angle = float(rand()) / RAND_MAX * 6.28318f; // 0..2π
        float speed = float(rand() % 50 + 50); // Pixel pro Sekunde

        Particle p;
        p.pos = pos;
        p.vel = { cos(angle) * speed, sin(angle) * speed };
        p.life = float(rand() % 50) / 100.0f + 0.5f; // 0.5 - 1 Sekunde
        p.color = { 255, 200, 0, 255 }; // orange-gelb

        particles.push_back(p);
    }
}


void Game::renderTitle() {

    renderer.clear();

    for (auto& s : stars) {
        renderer.drawRect(s.pos, s.size, {255, 255, 255, 255}); // weißer Stern
    }


    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;


    renderer.drawBigText(
        "Lunar Arena",
        centerX - 180,
        centerY - 220,
        {255, 255, 255, 255}
    );


    Vector2 wasdPos  = { centerX - 200.f, centerY - 20.f };
    Vector2 mousePos = { centerX + 200.f, centerY - 20.f };

    if (texWASD)
        renderer.drawTexture(texWASD, wasdPos, 256, 256, 0);

    if (texMouse)
        renderer.drawTexture(texMouse, mousePos, 236, 236, 0);


    renderer.drawText("Move",  wasdPos.x - 20,  wasdPos.y + 100, {200,200,200,255});
    renderer.drawText("Shoot", mousePos.x - 30, mousePos.y + 100, {200,200,200,255});


    if ((SDL_GetTicks() / 500) % 2 == 0)
    {
        renderer.drawText(
            "Press Space to Start",
            centerX - 100,
            centerY + 160,
            {255, 255, 255, 255}
        );
    }





    renderer.present();

}

void Game::renderPlaying()
{
    renderer.clear();

    // Hintergrund
    if(bgTexture)
    {
        if (bgTexture)
            renderer.drawFullscreenTexture(bgTexture, windowWidth, windowHeight);
    }


    Vector2 hudStartPos = {20.f, 20.f};
    float iconSpacing = 40.f;
    int iconSize = 32;
    // Lebensherzen rendern
    for (int i = 0; i < player.health; ++i) {
        Vector2 pos = hudStartPos + Vector2{ i * iconSpacing, 0 };
        if (textures["Health"])
            renderer.drawTexture(textures["Health"], pos, iconSize, iconSize, 0.0f);
    }

    // Ammo
    Vector2 dmgStartPos = hudStartPos + Vector2{ 0, iconSpacing };
    for (int i = 0; i < player.damage; ++i) {
        Vector2 pos = dmgStartPos + Vector2{ i * iconSpacing, 0 };
        if (textures["Ammo"])
            renderer.drawTexture(textures["Ammo"], pos, iconSize, iconSize, 0.0f);
    }

    for (auto& item : items)
    {
        if (!item->isDead && item->texture)
        {
            renderer.drawTexture(item->texture, item->pos, item->texWidth, item->texHeight, 0.0);
        }
    }



    player.render(renderer, input.mouseX, input.mouseY);
    for (auto& e : enemies)
        e->render(renderer);


    for (auto& e : enemies)
        e->render(renderer);

    for (auto& b : bullets)
        b.render(renderer);

    for (auto& b : enemyBullets)
        b.render(renderer);

    for (auto& p : particles) {
        renderer.drawRect(p.pos, 4, p.color); // 4x4 Pixel Blöcke
    }

    if (renderer.font) {
        std::string scoreText = "Score: " + std::to_string(score);
        int textWidth = 0, textHeight = 0;
        TTF_SizeText(renderer.font, scoreText.c_str(), &textWidth, &textHeight);

        int margin = 10;
        int x = windowWidth - textWidth - margin;
        int y = margin;

        renderer.drawText(scoreText, x, y, {255, 255, 255, 255}); // Weißer Text
    }
    renderer.present();
}

void Game::renderGameOver() {
    renderer.clear();

    for (auto& s : stars)
        renderer.drawRect(s.pos, s.size, {255,255,255,255});

    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    renderer.drawBigText(
        "GAME OVER",
        centerX - 200,
        centerY - 200,
        {255, 80, 80, 255}
    );

    std::string scoreText = " Score: " + std::to_string(score);

    renderer.drawBigText(
        scoreText,
        centerX - 160,
        centerY - 60,
        {255, 255, 255, 255}
    );

    if ((SDL_GetTicks() / 500) % 2 == 0)
    {
        renderer.drawText(
            "Press Space to try again",
            centerX - 150,
            centerY + 140,
            {200,200,200,255}
        );
    }

    renderer.present();
}

