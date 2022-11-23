#pragma once
#include "PhysicsWorld.h"
#include "Player.h"
#include "Planet.h"
#include "Time.h"

class Asteroid
{
private:
	Mesh* asteroidMesh;

	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	float speed;
	bool inactive;
	float currentTime;
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;
	Mesh* explosionMesh;
	DirectX::XMFLOAT3 explodePosition;

	void explode(std::vector<Planet*>& planets, std::vector<GameObject *>& objects);

public:
	Asteroid(Mesh* mesh);
	~Asteroid();

	bool isActive()const;
	void spawnAsteroid(const DirectX::XMFLOAT3& spawnPos, const DirectX::XMFLOAT3& direction, const float& speed);
	void moveAsteroid(const float& dt, std::vector<Planet*>& planets, std::vector<GameObject*>& objects);
	void draw();
	void drawFresnel();
	//void drawParticles();
};