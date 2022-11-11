#pragma once
#include "PhysicsWorld.h"
#include "Player.h"
#include "Planet.h"

class Asteroid
{
private:
	Mesh* asteroidMesh;

	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	float speed;
	bool inactive;

	void explode(std::vector<Planet*>& planets, std::vector<GameObject *>& objects);

public:
	Asteroid(Mesh* mesh);
	~Asteroid();

	bool isActive()const;
	void spawnAsteroid(DirectX::XMFLOAT3 spawnPos, DirectX::XMFLOAT3 direction, float speed);
	void moveAsteroid(const float& dt, std::vector<Planet*>& planets, std::vector<GameObject*>& objects);
	void draw();
};