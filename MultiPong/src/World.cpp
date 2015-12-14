#include "World.h"
#include "util.h"
#include <random>


World::World()
{
}


World::~World()
{
}

void World::Update(int dt)
{
	auto playerPos = PlayerPos(dt);
	for (int i = 0; i < players.size(); i++)
	{
		players[i].pos[0] = playerPos[i];
	}

	auto ballPos = BallPos(dt);
	for (int i = 0; i < balls.size(); i++)
	{
		balls[i].pos[0] = ballPos[i];
	}

	CollisionCheck();
}

void World::CollisionCheck()
{
	for (int i = 0; i < balls.size(); i++)
	{
		for (int j = i + 1; j < balls.size(); j++)
		{
			Collision(balls[i], balls[j]);
		}

		for (int j = 0; j < players.size(); j++)
		{
			Collision(balls[i], players[j]);
		}

		Collision(balls[i]);
	}
}

float World::PlayerCount()
{
	return 0;
}

int World::BallCount()
{
	return 0;
}

void World::AddPlayer(int curTime)
{
	WorldPlayer p;
	p.color = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255).toInteger();
	p.ID = players.size();
	p.joinTime = curTime;
	p.time = curTime;
	p.leaveTime = 0;
	p.pos[0] = 0.5f;
	p.pos[1] = 0.0f;
	p.pos[2] = 0.0f;
	players.push_back(p);

	if (std::sqrt(players.size()) > balls.size() + 1)
		AddBall(curTime);
}

void World::AddBall(int curTime)
{
	WorldBall b;
	b.color = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255).toInteger();
	b.pos[0] = sf::Vector2f(0, 0);
	b.pos[1] = sf::Vector2f(1, 0);
	b.pos[2] = sf::Vector2f(0, 0);
	b.time = curTime;
	balls.push_back(b);
}

bool World::UpdatePlayer(WorldPlayer player)
{
	if (player.ID > players.size())
		return false;

	players[player.ID] = player;
}

std::vector<float> World::PlayerPos(int rTime)
{
	std::vector<float> retVal(players.size());
	for (int i = 0; i < players.size(); i++)
	{
		float dt = rTime - players[i].time;
		float pos =
			players[i].pos[0]
			+ players[i].pos[1] * dt;
			//+ players[i].pos[2] * 0.5f * dt;
		retVal.push_back(pos);
	}
	return retVal;
}

std::vector<sf::Vector2f> World::BallPos(int rTime)
{
	std::vector<sf::Vector2f> retVal(balls.size());
	for (int i = 0; i < balls.size(); i++)
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
