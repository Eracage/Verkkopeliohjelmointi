#pragma once

#include <SFML/Graphics.hpp>



class Paddle
{
public:
	Paddle(sf::Font & font, float length = 20, float width = 5, sf::String name = "");

	~Paddle();

	void SetWidht(float width);

	void SetColor(sf::Color color);

	void SetPoints(sf::Vector2f start, sf::Vector2f end);

	void Draw(sf::RenderWindow& window);

	sf::CircleShape circles[2];
	sf::RectangleShape rectangle;
	sf::Text name;

	float Length;
	float Width;
};

