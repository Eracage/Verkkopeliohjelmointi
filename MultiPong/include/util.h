#pragma once
#ifndef UTIL_H
#define UTIL_H


#include <SFML/Graphics.hpp>

template <class T>
sf::Vector2f operator/(sf::Vector2<T> a, float b)
{
	return sf::Vector2f(a.x / b, a.y / b);
}

//template <class T>
//sf::Vector2f operator*(sf::Vector2<T> a, float b)
//{
//	return sf::Vector2f(a.x * b, a.y * b);
//}

template <class T>
float v2fMagnitude(sf::Vector2<T> a)
{
	return std::sqrt(a.x * a.x + a.y * a.y);
}

template <class T>
T clamp(T val, T min, T max)
{
	return std::max(min, std::min(val, max));
}

static sf::Vector2f CirclePos(float degrees)
{
	float rad = degrees * 3.14159265f / 180.0f;
	float x = std::cos(rad);
	float y = std::sin(rad);
	return sf::Vector2f(x, y);
}

#endif
