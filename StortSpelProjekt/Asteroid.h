#pragma once
#include "PhysicsWorld.h"
#include "Player.h"
#include "Planet.h"

class Asteroid
{
private:
	PhysicsComponent* physComp;
	Mesh* asteroidMesh;

	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	float speed;

	void explode(std::vector<Planet*>& planets, std::vector<GameObject *>& objects);
public:
	Asteroid(Mesh* mesh, PhysicsWorld& physWorld, DirectX::XMFLOAT3 spawnPos, DirectX::XMFLOAT3 travelDirection, float speed);
	~Asteroid();

	void update(std::vector<Planet*>& planets, std::vector<GameObject*>& objects);
	void draw();
};