#pragma once
#ifndef WORLD_H
#define WORLD_H


#include <XVec.h>
#include <WorldPlayer.h>
#include <WorldBall.h>
#include <SFML\Graphics.hpp>

struct PlayerInfo
{
	float degrees;
	int ID;
	sf::Vector2f WallRight;
	sf::Vector2f WallLeft;
	sf::Vector2f PaddleRight;
	sf::Vector2f PaddleLeft;
	sf::Color PlayerColor;
};

struct BallInfo
{
	sf::Vector2f Position;
	sf::Color BallColor;
	float radius;
};

class World
{
public:
	World();
	~World();

	sf::Int32 runTime;

	std::vector<WorldPlayer> players;
	std::vector<WorldBall> balls;

	void Update(sf::Int32 dt);

	void CollisionCheck();

	float PlayerCount();
	int BallCount();

	std::vector<PlayerInfo> GetPlayerInfo();
	std::vector<BallInfo> GetBallInfo();

	void AddPlayer(int curTime);
	void AddBall(int curTime);

	bool UpdatePlayer(WorldPlayer player);

	float PlayerPos(int index, int rTime);
	std::vector<float> PlayersPos(int rTime);
	std::vector<sf::Vector2f> BallPos(int rTime);

	void Collision(WorldBall& A, WorldBall& B);
	void Collision(WorldBall& A, WorldPlayer& B);
	void Collision(WorldBall& A);
};

#endif
