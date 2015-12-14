#pragma once
#ifndef WORLD_H
#define WORLD_H


#include <XVec.h>
#include <WorldPlayer.h>
#include <WorldBall.h>

class World
{
public:
	World();
	~World();

	std::vector<WorldPlayer> players;
	std::vector<WorldBall> balls;

	void Update(int dt);

	void CollisionCheck();

	float PlayerCount();
	int BallCount();

	void AddPlayer(int curTime);
	void AddBall(int curTime);

	bool UpdatePlayer(WorldPlayer player);

	std::vector<float> PlayerPos(int rTime);
	std::vector<sf::Vector2f> BallPos(int rTime);

	void Collision(WorldBall& A, WorldBall& B);
	void Collision(WorldBall& A, WorldPlayer& B);
	void Collision(WorldBall& A);
};

#endif
