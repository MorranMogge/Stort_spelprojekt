#include "stdafx.h"
#include "Asteroid.h"
#include "DirectXMathHelper.h"

void Asteroid::explode(std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	explodePosition = this->position;
	this->explosionMesh->scale = DirectX::XMFLOAT3(27, 27, 27);
	currentTime = 0.5f;

	for (int j = 0; j < objects.size(); j++)
	{
		DirectX::XMFLOAT3 vecToObject = this->position;
		subtractionXMFLOAT3(vecToObject, objects[j]->getPos());
		if (objects[j]->getPhysComp()->getType() == reactphysics3d::BodyType::STATIC) continue;
		if (getLength(vecToObject) <= 25.f)
		{
			objects[j]->getPhysComp()->setType(reactphysics3d::BodyType::DYNAMIC);
			DirectX::XMFLOAT3 explosionRange = getSubtractionXMFLOAT3(this->position, objects[j]->getPos());

			explosionRange = objects[j]->getPosV3() - this->position;
			float factor = 1.f / getLength(explosionRange);
			float newForce;
			//newNormalizeXMFLOAT3(explosionRange);
			Player* hitPlayer = dynamic_cast<Player*>(objects[j]);
			if (hitPlayer != nullptr)
			{
				newForce = this->scale.x * 2500 * factor;
				scalarMultiplicationXMFLOAT3(newForce, explosionRange);
				hitPlayer->hitByBat(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			}
			//Add force to object
			else
			{
				newForce = this->scale.x * 5000 * factor;
				scalarMultiplicationXMFLOAT3(newForce, explosionRange);
				objects[j]->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			}
		}
	}
	this->inactive = true;
}

Asteroid::Asteroid(Mesh* mesh)
	:currentTime(0.5)
{
	this->asteroidMesh = mesh;
	this->scale = DirectX::XMFLOAT3(8, 8, 8);
	this->inactive = false;
	this->direction = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	this->position = DirectX::XMFLOAT3(6969.f, 6969.f, 6969.f);
	this->speed = 0.f;
	this->explosionMesh = mesh;
	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(1, 0.25, 0, 0.5);
	this->colorBuffer.applyData();
}

Asteroid::~Asteroid()
{
}

bool Asteroid::isActive() const
{
	return !inactive;
}

void Asteroid::spawnAsteroid(const DirectX::XMFLOAT3& spawnPos, const DirectX::XMFLOAT3& direction, const float& speed)
{
	this->position = spawnPos;
	this->direction = direction;
	this->speed = speed;
	this->inactive = false;
}

void Asteroid::moveAsteroid(const float& dt, std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	if (this->inactive) return;
	additionXMFLOAT3(this->position, getScalarMultiplicationXMFLOAT3(speed*dt, direction));
	for (int i = 0; i < planets.size(); i++)
	{
		DirectX::XMFLOAT3 vecToPlanet = this->position;
		subtractionXMFLOAT3(vecToPlanet, planets[i]->getPlanetPosition());
		if ((getLength(vecToPlanet) - (this->scale.x + planets[i]->getSize())) <= 0)
		{ 
			this->explode(planets, objects); 
			return; 
		}
	}
}

void Asteroid::draw()
{
	if (this->inactive) return;
	this->asteroidMesh->position = this->position;
	this->asteroidMesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	this->asteroidMesh->scale = this->scale;

	this->asteroidMesh->UpdateCB(this->position, DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f)), this->scale);
	this->asteroidMesh->DrawWithMat();
}

void Asteroid::drawFresnel()
{
	float maxtime = 0.5f;
	if (inactive)
	{

		if (explosionMesh->scale.x > 1)
		{
			currentTime -= Time::DeltaTimeInSeconds();

			if (currentTime < 0.0f)
			{
				currentTime = maxtime;
			}
			float timeScale = currentTime / maxtime;
			float meshScale = 27 * timeScale;

			this->explosionMesh->scale = DirectX::XMFLOAT3(meshScale, meshScale, meshScale);
			this->explosionMesh->UpdateCB(explodePosition, DirectX::XMMatrixIdentity(), this->explosionMesh->scale);
			GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
			this->explosionMesh->DrawWithMat();
		}
	}
}