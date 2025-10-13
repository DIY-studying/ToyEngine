#pragma once

#include "SFML/Graphics.hpp"

class Vec2
{
public:
    float x = 0;
    float y = 0;
public:
    inline static Vec2 Zero() { return Vec2(); };
    inline static Vec2 One() { return Vec2(1, 0); };
    static Vec2 angleToUint(float angle);
public:
    Vec2() = default;
    Vec2(float xin, float yin);
    Vec2(float xin);
    explicit Vec2(const sf::Vector2u&);
    explicit Vec2(const sf::Vector2i&);

    bool operator==(const Vec2& rhs) const;

    bool operator!=(const Vec2& rhs) const;

    Vec2 operator+(const Vec2& rhs) const;

    Vec2 operator-(const Vec2& rhs) const;

    Vec2 operator*(float val) const;

    Vec2 operator/(float val) const;

    Vec2& operator+=(const Vec2& rhs) ;

    Vec2& operator-=(const Vec2& rhs) ;

    Vec2& operator*=(float val) ;

    Vec2& operator/=(float val) ;

    [[nodiscard]] float dist(const Vec2& v) const;

    [[nodiscard]] float length() const;

    Vec2& normalize();

};

