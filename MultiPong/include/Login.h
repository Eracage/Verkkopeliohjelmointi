#pragma once
#ifndef LOGIN_H
#define LOGIN_H


#include <vector>
#include <SFML/Graphics.hpp>

enum class LOGININFO
{
	IP,
	PORT,
	NAME
};

class Login
{
public:
	Login(sf::Font & font, bool & LoginSuccessful, sf::RenderWindow & Window);
	~Login();

	void Update();
	void Draw();

	void Input(sf::Uint32 c);

	bool Check();

	sf::RectangleShape box;

	std::vector<sf::Text> texts;
	std::vector<int> sizes;

	int field;

	bool& active;
	sf::RenderWindow & window;

private:
	bool IPCheck();
	bool PORTCheck();
	bool NAMECheck();
};

#endif
