#include "AnimationController.h"

void AnimationController::Play(const Animation& anim) {
    if (current != &anim) {
        current = &anim;
        frame = 0;
        timer = 0.f;
    }
}

void AnimationController::Update(float dt) {
    if (!current) return;

    timer += dt;
    if (timer >= current->frameTime) {
        timer -= current->frameTime;
        frame = (frame + 1) % current->frameCount;
    }
}

int AnimationController::GetFrame() const {
    return current ? current->startFrame + frame : 0;
}
