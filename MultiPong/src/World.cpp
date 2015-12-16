#include "World.h"
#include "util.h"
#include <random>
#include <iostream>


World::World()
{
	runTime = 0;
}


World::~World()
{
}

void World::Update(sf::Int32 dt)
{
	runTime += dt;

	auto ballPos = BallPos(runTime);
	for (size_t i = 0; i < balls.size(); i++)
	{
		balls[i].pos[0] = ballPos[i];
		balls[i].time = runTime;
	}

	CollisionCheck();
}

void World::CollisionCheck()
{
	auto p = GetPlayerInfo();
	auto b = GetBallInfo();

	for (size_t i = 0; i < b.size(); i++)
	{
		for (size_t j = i; j < b.size(); j++)
		{

		}

		for (size_t j = 0; j < p.size(); j++)
		{

		}
		if (v2fMagnitude(b[i].Position) > 200)
		{
			balls[i].pos[1] = -balls[i].pos[1];
		}
	}

	//for (size_t i = 0; i < balls.size(); i++)
	//{
	//	for (size_t j = i + 1; j < balls.size(); j++)
	//	{
	//		Collision(balls[i], balls[j]);
	//	}

	//	for (size_t j = 0; j < players.size(); j++)
	//	{
	//		Collision(balls[i], players[j]);
	//	}

	//	Collision(balls[i]);
	//}
}

float World::PlayerCount()
{
	return players.size();
}

int World::BallCount()
{
	return balls.size();
}

std::vector<PlayerInfo> World::GetPlayerInfo()
{
	std::vector<PlayerInfo> retVal;

	int playersAlive = 0;
	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i].leaveTime == 0)
		{
			playersAlive++;
		}
	}

	if (playersAlive > 1)
	{
		float degrees = 360;

		float perPlayer = degrees / playersAlive;

		float playerWall = 1.0f / playersAlive;

		float playerArea = 1.0f - playerWall;

		float hlfp = 0.125f;

		PlayerInfo info;

		int size = players.size();
		int deadPlayers = players.size() - playersAlive;
		int deadCount = 0;

		auto pPos = PlayersPos(runTime);

		for (int i = 0; i < size; i++)
		{
			if (players[i].leaveTime != 0)
			{
				deadCount++;
				continue;
			}
			const int C = i - deadCount;

			PlayerInfo info;
			info.WallRight = CirclePos(perPlayer * C - perPlayer * playerArea / 2) * 200.0f;
			info.WallLeft = CirclePos(perPlayer * C + perPlayer * playerArea / 2) * 200.0f;

			info.PaddleRight = info.WallRight +
				(info.WallLeft - info.WallRight) *
				(pPos[i] * (1 - hlfp * 2.0f));

			info.PaddleLeft = info.WallRight +
				(info.WallLeft - info.WallRight) *
				(pPos[i] * (1 - hlfp * 2.0f) + hlfp * 2.0f);

			info.PlayerColor = sf::Color(players[C].color);
			retVal.push_back(info);
		}
	}

	return retVal;
}

std::vector<BallInfo> World::GetBallInfo()
{
	std::vector<BallInfo> retVal;

	auto ballPos = BallPos(runTime);

	for (size_t i = 0; i < balls.size(); i++)
	{
		BallInfo info;

		info.Position = ballPos[i];
		info.BallColor = sf::Color(balls[i].color);
		info.radius = 50.0f / (5.0f + balls.size());

		retVal.push_back(info);
	}

	return retVal;
}

void World::AddPlayer(int curTime)
{
	WorldPlayer p;
	p.color = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255).toInteger();
	p.ID = players.size();
	p.joinTime = curTime;
	p.time = curTime;
	players.push_back(p);

	if (std::sqrt(players.size() + 0.1f) > balls.size() + 1.0f)
		AddBall(curTime);
}

void World::AddBall(int curTime)
{
	WorldBall b;
	b.color = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255).toInteger();
	b.pos[0] = sf::Vector2f(0, 0);
	b.pos[1] = CirclePos(rand());
	b.pos[2] = sf::Vector2f(0, 0);
	b.time = curTime;
	balls.push_back(b);
}

bool World::UpdatePlayer(WorldPlayer player)
{
	if (player.ID > players.size())
		return false;

	players[player.ID] = player;
	return true;
}

float World::PlayerPos(int index, int rTime)
{
	float dt = rTime - players[index].time;

	float pos =
		players[index].pos[0]
		+ players[index].pos[1] * dt;
		//+ players[index].pos[2] * 0.5f * dt;
	pos = clamp(pos, 0.0f, 1.0f);

	return pos;
}

std::vector<float> World::PlayersPos(int rTime)
{
	std::vector<float> retVal;
	retVal.reserve(players.size());
	for (size_t i = 0; i < players.size(); i++)
	{
		retVal.push_back(PlayerPos(i,rTime));
	}
	return retVal;
}

std::vector<sf::Vector2f> World::BallPos(int rTime)
{
	std::vector<sf::Vector2f> retVal;
	retVal.reserve(balls.size());
	for (size_t i = 0; i < balls.size(); i++)
	{
		float dt = rTime - balls[i].time;
		sf::Vector2f pos =
			balls[i].pos[0]
			+ balls[i].pos[1] * dt;
			//+ balls[i].pos[2] * 0.5f * dt;
		retVal.push_back(pos);
	}
	return retVal;
}

void World::Collision(WorldBall & A, WorldBall & B)
{
}

void World::Collision(WorldBall & A, WorldPlayer & B)
{
}

void World::Collision(WorldBall & A)
{
}
