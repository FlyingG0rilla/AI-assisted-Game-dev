
#include "Renderer.h"
#include <iostream>

#include "Renderer.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

Renderer::~Renderer() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (bigFont) {
        TTF_CloseFont(bigFont);
        bigFont = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
}

bool Renderer::init(SDL_Window* window, const std::string& fontPath, int fontSize) {
    if (!window) {
        std::cerr << "[Renderer] Error: window is null!" << std::endl;
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[Renderer] SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }


    // Pixel-Art: kein Blur beim Skalieren
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // Nearest Neighbor


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "[Renderer] SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "[Renderer] TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("assets/arial.ttf", 24);
    if (!font) {
        std::cerr << "[Renderer] Failed to load font: " << TTF_GetError() << std::endl;
    }

    bigFont = TTF_OpenFont("assets/arial.ttf", 64);
    if (!bigFont) {
        std::cerr << "[Renderer] Failed to load big font: " << TTF_GetError() << std::endl;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        return false;
    }

    std::cout << "Renderer initialized successfully!" << std::endl;
    return true;
}

void Renderer::clear() {
    if (!renderer) return;
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    if (!renderer) return;
    SDL_RenderPresent(renderer);
}


void Renderer::startShake(float strength, float duration)
{
    shake.strength = strength;
    shake.time = duration;
}

void Renderer::updateShake(float dt)
{
    if (shake.time > 0.f)
    {
        shake.time -= dt;

        float angle = float(rand()) / RAND_MAX * 6.28318f; // 0..2π
        float power = shake.strength * (shake.time);

        shakeOffset.x = cos(angle) * power;
        shakeOffset.y = sin(angle) * power;
    }
    else
    {
        shakeOffset = {0.f, 0.f};
    }
}


void Renderer::drawRect(Vector2 pos, int size, SDL_Color color) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect r{
        static_cast<int>(pos.x ) - size / 2,
        static_cast<int>(pos.y ) - size / 2,
        size,
        size
    };
    SDL_RenderFillRect(renderer, &r);
}

void Renderer::drawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, SDL_Color color) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, static_cast<int>(p1.x + shakeOffset.x), static_cast<int>(p1.y + shakeOffset.y),
                                 static_cast<int>(p2.x + shakeOffset.x), static_cast<int>(p2.y + shakeOffset.y));
    SDL_RenderDrawLine(renderer, static_cast<int>(p2.x + shakeOffset.x), static_cast<int>(p2.y + shakeOffset.y),
                                 static_cast<int>(p3.x + shakeOffset.x), static_cast<int>(p3.y + shakeOffset.y));
    SDL_RenderDrawLine(renderer, static_cast<int>(p3.x + shakeOffset.x), static_cast<int>(p3.y + shakeOffset.y),
                                 static_cast<int>(p1.x + shakeOffset.x), static_cast<int>(p1.y + shakeOffset.y));
}

void Renderer::drawText(const std::string& text, int x, int y, SDL_Color color) {
    if (!renderer || !font) return;

    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surf) return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }

    SDL_Rect dst{ static_cast<int>(x + shakeOffset.x), static_cast<int>(y + shakeOffset.y), surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, nullptr, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void Renderer::drawBigText(const std::string& text, int x, int y, SDL_Color color)
{
    if (!renderer || !bigFont) return;

    SDL_Surface* surf = TTF_RenderText_Blended(bigFont, text.c_str(), color);
    if (!surf) return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }

    SDL_Rect dst{ static_cast<int>(x + shakeOffset.x), static_cast<int>(y + shakeOffset.y), surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, nullptr, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

SDL_Texture* Renderer::loadTexture(const std::string& path) {
    if (!renderer) return nullptr;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "[Renderer] IMG_Load failed for " << path << ": " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex) {
        std::cerr << "[Renderer] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    return tex;
}

void Renderer::drawTexture(SDL_Texture* tex, Vector2 pos, int width, int height, double angle)
{
    if (!renderer || !tex) return;

    SDL_Rect dst;
    dst.w = width;
    dst.h = height;
    dst.x = static_cast<int>(pos.x + shakeOffset.x - width / 2);
    dst.y = static_cast<int>(pos.y + shakeOffset.y - height / 2);

    SDL_Point center = { width / 2, height / 2 };

    SDL_RenderCopyEx(
        renderer,
        tex,
        nullptr,
        &dst,
        angle,
        &center,
        SDL_FLIP_NONE
    );
}


void Renderer::drawTiledTexture(SDL_Texture* tex, int tileW, int tileH, int windowWidth, int windowHeight, float offsetX, float offsetY)
{
    if (!renderer || !tex) return;

    for (int x = -tileW; x < windowWidth; x += tileW)
    {
        for (int y = -tileH; y < windowHeight; y += tileH)
        {
            SDL_Rect dst{ static_cast<int>(x + offsetX), static_cast<int>(y + offsetY), tileW, tileH };
            SDL_RenderCopy(renderer, tex, nullptr, &dst);
        }
    }
}

void Renderer::drawFullscreenTexture(SDL_Texture* tex, int windowWidth, int windowHeight)
{
    if (!renderer || !tex) return;

    SDL_Rect dst{ 0, 0, windowWidth, windowHeight };
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
}

SDL_Renderer* Renderer::getSDLRenderer() {
    return renderer;
}





