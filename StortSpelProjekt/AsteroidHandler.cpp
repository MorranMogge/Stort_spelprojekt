#include "stdafx.h"
#include "AsteroidHandler.h"
#include "DirectXMathHelper.h"

AsteroidHandler::AsteroidHandler(const std::vector<Mesh*>& meshes, PhysicsWorld& physWorld)
	:activeAsteroids(0)
{
	timer.resetStartTime();
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		asteroids.emplace_back(new Asteroid(meshes[rand() % (int)meshes.size()], physWorld));
	}
}

AsteroidHandler::AsteroidHandler(Mesh* meshes, PhysicsWorld& physWorld)
	:activeAsteroids(0)
{
	timer.resetStartTime();
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		asteroids.emplace_back(new Asteroid(meshes, physWorld));
	}
}

AsteroidHandler::~AsteroidHandler()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		delete asteroids[i];
	}
}

bool AsteroidHandler::ifTimeToSpawnAsteroids()
{
	bool spawn = timer.getTimePassed(TIMEBEFOREASTEROIDS);
	if (spawn) timer.resetStartTime();
	return spawn;
}

void AsteroidHandler::spawnAsteroids(const Planet* planet, int nrOf)
{
	float x;
	float y;
	float z;

	DirectX::XMFLOAT3 spawnPos;
	DirectX::XMFLOAT3 dir;

	if (nrOf == 0) this->activeAsteroids = rand() % MAXASTEROIDS + 1;
	else this->activeAsteroids = nrOf;
	for (int i = 0; i < this->activeAsteroids; i++)
	{
		x = (float)(rand() % 101) + planet->getSize() * 2 * (1 - rand() % 3);
		y = (float)(rand() % 101) + planet->getSize() * 2 * (1 - rand() % 3);
		z = (float)(rand() % 101) + planet->getSize() * 2 * (1 - rand() % 3);
		spawnPos = DirectX::XMFLOAT3(x, y, z);

		dir = spawnPos;
		subtractionXMFLOAT3(dir, planet->getPlanetPosition());
		scalarMultiplicationXMFLOAT3(-1, dir);
		newNormalizeXMFLOAT3(dir);

		asteroids[i]->spawnAsteroid(spawnPos, dir, (float)(rand() % 41 + 10));
		//asteroids[i]->spawnAsteroid(,);
	}
}

void AsteroidHandler::updateAsteroids(const float& dt, std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	for (int i = 0; i < activeAsteroids; i++)
	{
		this->asteroids[i]->moveAsteroid(dt, planets, objects);
	}
	/*bool active = false;
	for (int i = 0; i < activeAsteroids; i++)
	{
		if (this->asteroids[i]->isActive()) active = true;
	}
	if (!active) this->spawnAsteroids(planets[0]);*/
}

void AsteroidHandler::drawAsteroids()
{
	for (int i = 0; i < this->activeAsteroids; i++)
	{
		asteroids[i]->draw();
	}
}
