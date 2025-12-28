

#include "Player.h"
#include <cmath>
#include <iostream>



void Player::update(float dt, const Input& in, int windowWidth, int windowHeight)
{

    // Bewegungsrichtung bestimmen
    Vector2 dir{0.f, 0.f};
    if (in.w) dir.y -= 1.f;
    if (in.s) dir.y += 1.f;
    if (in.a) dir.x -= 1.f;
    if (in.d) dir.x += 1.f;

    bool moving = (dir.x != 0 || dir.y != 0);

    if (!moving) {
        animState = AnimState::Idle;
        animTimer = 0.f;
    }
    else {
        animTimer += dt;

        if (animTimer >= stepInterval) {
            animTimer = 0.f;

            if (animState == AnimState::Idle || animState == AnimState::StepRight)
                animState = AnimState::StepLeft;
            else
                animState = AnimState::StepRight;
        }
    };

    // Bewegung
    if (dir.x != 0.f || dir.y != 0.f)
        pos = pos + normalize(dir) * speed * dt;

    // Fensterbegrenzung
    float radius = 12.f;
    if (pos.x < radius) pos.x = radius;
    if (pos.y < radius) pos.y = radius;
    if (pos.x > windowWidth - radius) pos.x = windowWidth - radius;
    if (pos.y > windowHeight - radius) pos.y = windowHeight - radius;


}

void Player::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;
    std::cout << "Player takes " << dmg << " damage, health=" << health << std::endl;
}




void Player::increaseHealth()
{
    if (health < maxHealth) health++;
    if (health > maxHealth) health = maxHealth;
}

void Player::increaseDamage()
{
    if (damage < maxDamage) damage++;
    if (damage > maxDamage) damage = maxDamage;
}

void Player::render(Renderer& r, int mouseX, int mouseY)
{

    SDL_Texture* tex = texIdle;

    switch (animState) {
        case AnimState::Idle:      tex = texIdle;  break;
        case AnimState::StepLeft:  tex = texLeft;  break;
        case AnimState::StepRight: tex = texRight; break;
    }

    // Richtung zur Maus
    Vector2 toMouse = Vector2{ float(mouseX), float(mouseY) } - pos;

    // atan2 → Grad
    float angleDeg =
        std::atan2(toMouse.y, toMouse.x) * 180.f / M_PI
        - 90.f;

    ;

    r.drawTexture(
        tex,
        pos,
        renderSize,
        renderSize,
        angleDeg
    );



    /*
    }else if (!spriteSheetPlayer){

        // 3. Pfeil Richtung Maus
        Vector2 dir = normalize(Vector2{float(mouseX), float(mouseY)} - pos);
        float size = 16.f;
        float offset = 20.f;
        Vector2 center = pos + dir * offset;
        Vector2 perp{-dir.y, dir.x};

        Vector2 tip = center + dir * size;
        Vector2 left = center + perp * (size / 3.f);
        Vector2 right = center - perp * (size / 3.f);

        r.drawTriangle(left, right, tip, {255,255,255,255});
    }
    */
}
