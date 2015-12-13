#ifndef MULTIPONGUTIL_H
#define MULTIPONGUTIL_H

#include <SFML/Graphics.hpp>

template <class T>
sf::Vector2f operator/(sf::Vector2<T> a, float b)
{
	return sf::Vector2f(a.x / b, a.y / b);
}

template <class T>
sf::Vector2f operator*(sf::Vector2<T> a, float b)
{
	return sf::Vector2f(a.x * b, a.y * b);
}

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


#endif // !UTIL_H