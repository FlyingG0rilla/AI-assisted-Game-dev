#pragma once

class AnimationController {
public:
    struct Animation {
        int startFrame;
        int frameCount;
        float frameTime;
    };

    void Play(const Animation& anim);
    void Update(float dt);
    int GetFrame() const;

private:
    const Animation* current = nullptr;
    int frame = 0;
    float timer = 0.f;
};
