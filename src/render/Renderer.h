#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "core/Vector2.h"

struct CameraShake {
    float time = 0.f;
    float strength = 0.f;
};

class Renderer {
public:
    // Konstruktor / Destruktor
    Renderer() = default;
    ~Renderer();

    // Initialisierung
    bool init(SDL_Window* window, const std::string& fontPath, int fontSize);

    // Render-Zyklus
    void clear();
    void present();

    // Zeichenfunktionen
    void drawText(const std::string& text, int x, int y, SDL_Color color);
    void drawBigText(const std::string& text, int x, int y, SDL_Color color);
    void drawRect(Vector2 pos, int size, SDL_Color color);
    void drawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, SDL_Color color);

    // Textur-Funktionen
    SDL_Texture* loadTexture(const std::string& path);
    void drawTexture(SDL_Texture* tex, Vector2 pos, int width, int height, double angle);
    void drawTiledTexture(SDL_Texture* tex, int tileW, int tileH, int windowWidth, int windowHeight, float offsetX = 0.f, float offsetY = 0.f);
    void drawFullscreenTexture(SDL_Texture* tex, int windowWidth, int windowHeight);

    SDL_Renderer *getSDLRenderer();

    // Kamera-Shake
    void startShake(float strength, float duration);
    void updateShake(float dt);

    // Getter
    SDL_Renderer* getRawRenderer() const { return renderer; }

    // Öffentliche Membervariablen
    TTF_Font* font = nullptr;
    TTF_Font* bigFont = nullptr;
    Vector2 cameraOffset{0.f, 0.f};

private:
    SDL_Renderer* renderer = nullptr;
    CameraShake shake;
    Vector2 shakeOffset{0.f, 0.f};
};



