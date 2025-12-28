#pragma once
#include <vector>
#include <SDL.h>
#include "core/Vector2.h"
#include "core/Input.h"
#include "render/AnimationController.h"
#include "render/Renderer.h"

class Player {
public:
    Vector2 pos{500.f,500.f};
    Vector2 velocity{0.f, 0.f};
    float speed = 200.f;

    int health = 5;
    int maxHealth = 5;
    int damage = 1;
    int maxDamage = 5;

    // Textures
    //SDL_Texture* idleTexture = nullptr;
   // std::vector<SDL_Texture*> walkTextures;



    Player() = default;

    void update(float dt, const Input& in, int windowWidth, int windowHeight);
    void render(Renderer& r, int mouseX, int mouseY);

    void setTexture(SDL_Texture* tex, int frameW, int frameH);

    void takeDamage(int dmg);

    void setSpriteSheet(SDL_Texture *tex);

    void increaseHealth();
    void increaseDamage();


    // Texturen
    SDL_Texture* texIdle  = nullptr;
    SDL_Texture* texLeft  = nullptr;
    SDL_Texture* texRight = nullptr;


    //SDL_Texture* spriteSheetPlayer;

private:
    AnimationController animator;
    /*
    // Animationen
    AnimationController::Animation idle {0, 1, 0.2f};
    AnimationController::Animation walk {0, 4, 0.12f};





    int frameWidth = 64;
    int frameHeight = 64;

    int currentRow = 0; // 0=linker Fuß, 1=rechter Fuß

    SDL_Rect getSrcRect(int frame, int row) const;
    */

    enum class AnimState {
        Idle,
        StepLeft,
        StepRight
    };

    AnimState animState = AnimState::Idle;
    float animTimer = 0.f;
    float stepInterval = 0.18f; // Zeit pro Schritt


    int renderSize = 90; // Standardgröße, kann jeder Unterklasse überschreiben
};






