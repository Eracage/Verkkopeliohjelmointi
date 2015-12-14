#include "Paddle.h"
#include "util.h"


sf::Color CInvert(sf::Color c)
{
	return sf::Color(255 - c.r, 255 - c.g, 255 - c.b, c.a);
}

Paddle::Paddle(sf::Font & font, float length, float width, sf::String namep)
	: Length(length),
	Width(width),
	name(namep,font)
{
	SetWidht(width);
	SetColor(sf::Color::White);
}

Paddle::~Paddle()
{
}

void Paddle::SetWidht(float width)
{
	Width = width;
	for (int i = 0; i < 2; i++)
	{
		circles[i].setPointCount(10 + sqrt(width));
		circles[i].setRadius(width);
		circles[i].setOrigin(width, width);
	}

	rectangle.setSize(sf::Vector2f(rectangle.getSize().x, width * 2));
	rectangle.setOrigin(rectangle.getSize() / 2);
	name.setCharacterSize(width * 2);
	name.setOrigin(name.getLocalBounds().width / 2, name.getLocalBounds().height / 1.5f);
}

void Paddle::SetColor(sf::Color color)
{
	for (int i = 0; i < 2; i++)
	{
		circles[i].setFillColor(color);
	}
	rectangle.setFillColor(color);

	name.setColor(CInvert(color));
}

void Paddle::SetPoints(sf::Vector2f start, sf::Vector2f end)
{
	circles[0].setPosition(start);
	circles[1].setPosition(end);

	sf::Vector2f vec = end - start;

	Length = v2fMagnitude(vec);
	rectangle.setSize(sf::Vector2f(Length, rectangle.getSize().y));
	rectangle.setOrigin(rectangle.getSize() / 2);

	const float rot = std::atan2(vec.y, vec.x) * 180 / 3.14159265;
	name.setRotation(rot);
	name.setPosition(start + vec / 2);
	rectangle.setRotation(rot);
	rectangle.setPosition(start + vec / 2);
}

void Paddle::Draw(sf::RenderWindow & window)
{
	for (int i = 0; i < 2; i++)
	{
		window.draw(circles[i]);
	}
	window.draw(rectangle);
	window.draw(name);
}
