
#include "Input.h"


void Input::update(bool& running)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;



        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            bool down = e.type == SDL_KEYDOWN;
            if (e.key.keysym.sym == SDLK_w) w = down;
            if (e.key.keysym.sym == SDLK_s) s = down;
            if (e.key.keysym.sym == SDLK_a) a = down;
            if (e.key.keysym.sym == SDLK_d) d = down;
            if (e.key.keysym.sym == SDLK_SPACE) space = down;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) shoot = true;
        if (e.type == SDL_MOUSEBUTTONUP) shoot = false;
    }
    SDL_GetMouseState(&mouseX, &mouseY);
}