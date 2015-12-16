#include <util.h>



sf::Vector2f CirclePos(float degrees)
{
	float rad = degrees * 3.14159265f / 180.0f;
	float x = std::cos(rad);
	float y = std::sin(rad);
	return sf::Vector2f(x, y);
}

float sfdot(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}
