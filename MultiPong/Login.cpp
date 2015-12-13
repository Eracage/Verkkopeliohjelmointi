#include "Login.h"
#include "util.h"



Login::Login(sf::Font & font, bool & LoginSuccessful, sf::RenderWindow & Window)
	: active(LoginSuccessful),
	field(0),
	window(Window)
{
	std::vector<sf::String> loginInfo; // ip, port, name

	//loginInfo.push_back(sf::String());
	//loginInfo.back() = "IP        :";
	//sizes.push_back(loginInfo.back().getSize());
	//loginInfo.push_back(sf::String());
	//loginInfo.back() = "PORT    :";
	//sizes.push_back(loginInfo.back().getSize());
	loginInfo.push_back(sf::String());
	loginInfo.back() = "NAME   :";
	sizes.push_back(loginInfo.back().getSize());

	for each (sf::String t in loginInfo)
	{
		texts.push_back(sf::Text(t,font,24));
		texts.back().setColor(sf::Color::Red);
	}

	window.setView(window.getDefaultView());
	for (int i = 0; i < texts.size(); i++)
	{
		texts[i].setPosition(window.getSize() / 2.0f + sf::Vector2f(0, 30) * (i - 1));
		texts[i].setOrigin(texts[i].getLocalBounds().width / 2, texts[i].getLocalBounds().height / 2);
	}

	box.setSize(sf::Vector2f(300, 200));
	box.setOrigin(box.getSize() / 2);
	box.setPosition(window.getSize() / 2);
	box.setFillColor(sf::Color(0, 0, 0, 191));
}


Login::~Login()
{
}

void Login::Update()
{
	texts[0].rotate(0.1f);
}

void Login::Draw()
{
	window.setView(window.getDefaultView());

	if (active)
	{
		window.draw(box);
	}

	for (int i = 0; i < texts.size(); i++)
	{
		window.draw(texts[i]);
	}
}

void Login::Input(sf::Uint32 c)
{
	if (!active)
		return;
	if (texts[field].getString().getSize() > sizes[field])
	{
		if (c == 8)
		{
			texts[field].setString(texts[field].getString().substring(0, texts[field].getString().getSize() - 1));
		}
		else if (c == 10 || c == 13)
		{
			if (Check())
				field++;
		}
	}
	if (c == 9)
	{
		field = ++field % texts.size();
	}
	else if (c == 27)
		window.close();
	else if (32 <= c && texts[field].getString().getSize() <= 30)
		texts[field].setString(texts[field].getString() + c);

	window.setView(window.getDefaultView());
	for (int i = 0; i < texts.size(); i++)
	{
		if (active)
		{
			texts[i].setPosition(window.getSize() / 2.0f + sf::Vector2f(0, 30) * (i - 1));
			texts[i].setOrigin(texts[i].getLocalBounds().width / 2, texts[i].getLocalBounds().height / 2);
		}
		else
		{
			static int z = 0;
			texts[i].setCharacterSize(16);
			texts[i].setOrigin(0, 0);
			texts[i].setPosition(5, 5 + 30 * i);
		}
	}
}

bool Login::Check()
{
	//switch (LOGININFO(field))
	//{
	//case LOGININFO::IP:
	//	if (!IPCheck)
	//	{
	//		return false;
	//	}
	//	break;
	//case LOGININFO::PORT:
	//	if (!PORTCheck)
	//	{
	//		return false;
	//	}
	//	break;
	//case LOGININFO::NAME:
	//	if (!NAMECheck)
	//	{
	//		return false;
	//	}
	//	break;

	//default:
	//	break;
	//}

	for (int i = 0; i < texts.size(); i++)
	{
		if (texts[i].getString().getSize() <= sizes[i])
		{
			return false;
		}
	}

	// TODO: check all fields
	//for (int i = 0; i < texts.size(); i++)
	//{
	//	if (texts[i].getString().getSize() <= sizes[i])
	//	{
	//		switch (LOGININFO(i))
	//		{
	//		case LOGININFO::IP:
	//			if (!IPCheck)
	//			{
	//				return false;
	//			}
	//			break;
	//		case LOGININFO::PORT:
	//			if (!PORTCheck)
	//			{
	//				return false;
	//			}
	//			break;
	//		case LOGININFO::NAME:
	//			if (!NAMECheck)
	//			{
	//				return false;
	//			}
	//			break;

	//		default:
	//			break;
	//		}
	//	}
	//}

	return active = false;
}

//bool Login::IPCheck()
//{
//	std::regex ip("\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0 - 5] | 2[0 - 4][0 - 9] | [01] ? [0 - 9][0 - 9] ? )\b");
//
//	std::regex_search(texts[(int)LOGININFO::IP].getString()., );
//
//	return false;
//}
//
//bool Login::PORTCheck()
//{
//	return false;
//}
//
//bool Login::NAMECheck()
//{
//	return false;
//}
