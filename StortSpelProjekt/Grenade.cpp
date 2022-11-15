#include "stdafx.h"
#include "Grenade.h"
#include "DirectXMathHelper.h"
#include "Player.h"
#include "PhysicsComponent.h"

Grenade::Grenade(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(useMesh, pos, rot, id, onlineId, GRENADE, field), destructionIsImminent(false), timeToExplode(5.f), currentTime(0.0f)
{
	sfx.load(L"../Sounds/explosion.wav");
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 2);

	//Item Icon
	float constant = 4.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_boom.png", "icon_boom2.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);

	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(1 ,0.25 ,0 , 0.5);
	this->colorBuffer.applyData();

	explosionMesh = (new Mesh("../Meshes/Sphere"));
}

Grenade::Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(objectPath, pos, rot, id, onlineId, GRENADE, field), destructionIsImminent(false), timeToExplode(5.f), currentTime(0.0f)
{
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 36, DirectX::XMFLOAT2(2, 5), 4);

	//Item Icon
	float constant = 4.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_boom.png", "icon_boom2.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);

	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(1, 0.25, 0, 0.5);
	this->colorBuffer.applyData();

	explosionMesh = (new Mesh("../Meshes/Sphere"));
}

Grenade::~Grenade()
{
	delete explosionMesh;
}

void Grenade::explode()
{
	sfx.stop();
	sfx.play();
	std::cout << "THE GRENADE EXPLODED\n";
	int iterations = (int)gameObjects.size();
	for (int i = 0; i < iterations; i++)
	{
		if (gameObjects[i] == this) continue;
		if (this->withinRadious(gameObjects[i], 25))
		{
			gameObjects[i]->getPhysComp()->setType(reactphysics3d::BodyType::DYNAMIC);
			DirectX::XMFLOAT3 explosionRange = getSubtractionXMFLOAT3(this->position, gameObjects[i]->getPos());

			explosionRange = gameObjects[i]->getPosV3() - this->position;
			float factor = 1.f / getLength(explosionRange);
			float newForce;
			//newNormalizeXMFLOAT3(explosionRange);
			Player* hitPlayer = dynamic_cast<Player*>(gameObjects[i]);
			if (hitPlayer != nullptr)
			{
				newForce = this->physComp->getMass() * 2500 * factor;
				scalarMultiplicationXMFLOAT3(newForce, explosionRange);
				hitPlayer->hitByBat(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			}
			//Add force to object
			else
			{
				newForce = this->physComp->getMass() * 5000 * factor;
				scalarMultiplicationXMFLOAT3(newForce, explosionRange);
				gameObjects[i]->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			}
		}
	}
	this->explosionMesh->scale = DirectX::XMFLOAT3(27, 27, 27);
	this->destructionIsImminent = false;
}

void Grenade::updateExplosionCheck()
{
	if (destructionIsImminent && this->timer.getTimePassed(this->timeToExplode)) this->explode();
}

void Grenade::setGameObjects(const std::vector<GameObject*>& gameObjects)
{
	this->gameObjects = gameObjects;
}

void Grenade::drawIcon()
{
	if (destructionIsImminent)
	{
		this->itemIcon->bindAndDraw(1, 0);
	}
	else
	{
		if (this->itemIcon != nullptr && !pickedUp)
		{
			if (withinPlayerReach)
			{
				this->itemIcon->bindAndDraw(1, 0);
			}
			else
			{
				this->itemIcon->bindAndDraw(0, 0);
			}
		}
	}
}

void Grenade::drawParticles()
{
	if (destructionIsImminent)
	{
		this->particles->BindAndDraw(0);



		currentTime += this->timer2.getDt();
		static int frame = 0;
		if (currentTime >= 1)
		{
			currentTime -= 1;
			frame = 15;
		}

		this->setScale(DirectX::XMFLOAT3(1.0f + (currentTime/2), 1.0f+ (currentTime / 2), 1.0f+ (currentTime / 2)));


		if (frame != 0)
		{
			frame--;
			this->mesh->matKey[0] = "Red.png";
		}
		else
		{
			this->mesh->matKey[0] = "Default";			

		}
		timer2.resetStartTime();
	}
	else
	{
		this->setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		if (this->particles != nullptr && pickedUp)
		{
			if (tStruct.getDt() < 1)
			{
				this->particles->BindAndDraw(0);
			}
		}
	}
}

void Grenade::drawFresnel()
{	
	DirectX::XMFLOAT3 test = this->explosionMesh->scale;
	if (test.x > 1)
	{
		GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
		this->explosionMesh->DrawWithMat();
		this->explosionMesh->scale = DirectX::XMFLOAT3(this->explosionMesh->scale.x - (currentTime / 4) , this->explosionMesh->scale.y - (currentTime / 4), this->explosionMesh->scale.z - (currentTime / 4));
		this->explosionMesh->UpdateCB(position, rotation, this->explosionMesh->scale);
	}
}

void Grenade::useItem()
{
	if (!this->destructionIsImminent)
	{
		
		this->destructionIsImminent = true;
		timer.resetStartTime();
		timer2.resetStartTime();
	}
}
