
#pragma once
#include <cmath>

struct Vector2 {
    float x = 0;
    float y = 0;


    Vector2 operator+(const Vector2& o) const { return {x + o.x, y + o.y}; }
    Vector2 operator-(const Vector2& o) const { return {x - o.x, y - o.y}; }
    Vector2 operator*(float s) const { return {x * s, y * s}; }
};


inline float length(const Vector2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}


inline Vector2 normalize(const Vector2& v) {
    float l = length(v);
    return l > 0 ? Vector2{v.x / l, v.y / l} : Vector2{};
}