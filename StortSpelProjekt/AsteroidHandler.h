#pragma once
#include "Asteroid.h"
#include "TimeStruct.h"

#define MAXASTEROIDS 10
#define TIMEBEFOREASTEROIDS 60

class AsteroidHandler
{
private:
	std::vector<Asteroid*> asteroids;
	std::vector<Mesh*> meshes;
	TimeStruct timer;
	int activeAsteroids;

public:
	AsteroidHandler(const std::vector<Mesh*>& meshes);
	AsteroidHandler(Mesh* meshes);
	~AsteroidHandler();

	bool ifTimeToSpawnAsteroids();
	void spawnAsteroids(const Planet* planet, int nrOf = 0);
	void updateAsteroids(const float& dt, std::vector<Planet*>& planets, std::vector<GameObject*>& objects);
	void drawAsteroids();
};