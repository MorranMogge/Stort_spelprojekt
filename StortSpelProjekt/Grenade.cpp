#include "stdafx.h"
#include "Grenade.h"
#include "DirectXMathHelper.h"
#include "Player.h"
#include "PhysicsComponent.h"
#include "Time.h"
#include "GUISprite.h"


Grenade::Grenade(Mesh* useMesh, Mesh* expMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(useMesh, pos, rot, id, onlineId, GRENADE, field), destructionIsImminent(false), exploded(false), timeToExplode(5.f), currentTime(0.0f), explodePosition(0,0,0), redMesh(expMesh)
{
	//Sfx
	counter = 1.0f;

	sfx.load(L"../Sounds/explosion.wav");
	explosion.load(L"../Sounds/explodeGrenade.wav");

	//Color
	this->color = DirectX::Colors::Yellow.v;

	//Set up Fresnel buffer
	fresnelBuffer.Initialize(GPU::device, GPU::immediateContext);
	fresnelBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 1);
	fresnelBuffer.applyData();

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 2);
	this->particles->setColor(color);

	//Item Icon
	float constant = 4.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_boom.png", "icon_boom2.png" , "Ekey.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);

	//Set up Explosion color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(1 ,0.25 ,0 , 0.5);
	this->colorBuffer.applyData();

	//Set meshes & materials
	this->redMesh->matKey[0] = "Red.png";
	this->mesh->matKey[0] = "olive.jpg";
	explosionMesh = (new Mesh("../Meshes/Sphere"));
	explosionMesh->scale = DirectX::XMFLOAT3( 0.1f,0.1f,0.1f);


	//explosionMesh = explodeMesh; ändra tillbaka?

}

Grenade::Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(objectPath, pos, rot, id, onlineId, GRENADE, field), destructionIsImminent(false), exploded(false), timeToExplode(5.f), currentTime(0.0f), explodePosition(0, 0, 0)
{
	//Sfx
	counter = 1.0f;
	sfx.load(L"../Sounds/explosion.wav");
	explosion.load(L"../Sounds/explodeGrenade.wav");

	//Color
	this->color = DirectX::Colors::Yellow.v;

	//Set up Fresnel buffer
	fresnelBuffer.Initialize(GPU::device, GPU::immediateContext);
	fresnelBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 1);
	fresnelBuffer.applyData();

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 2);
	this->particles->setColor(color);

	//Item Icon
	float constant = 4.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_boom.png", "icon_boom2.png" , "Ekey.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);

	//Set up Explosion color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(1, 0.25, 0, 0.5);
	this->colorBuffer.applyData();
	this->mesh->matKey[0] = "olive.jpg";
	explosionMesh = (new Mesh("../Meshes/Sphere"));
}

Grenade::~Grenade()
{
	delete this->explosionMesh;
}

void Grenade::explode()
{
	explosion.stop();
	explosion.play();
	std::cout << "THE GRENADE EXPLODED\n";
	exploded = true;
	explodePosition = this->position;
	currentTime = 0;
	int iterations = (int)gameObjects.size();
	for (int i = 0; i < iterations; i++)
	{
		if (gameObjects[i] == this || gameObjects[i]->getPhysComp()->getType() == reactphysics3d::BodyType::STATIC) continue;
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
	counter = 1.0f;
	this->explosionMesh->scale = DirectX::XMFLOAT3(27, 27, 27);
	this->destructionIsImminent = false;
}

void Grenade::updateExplosionCheck()
{
	if (destructionIsImminent && this->timer.getTimePassed(this->timeToExplode)) this->explode();
	else if (destructionIsImminent && this->timer.getTimePassed(counter)) {counter++; sfx.stop(); sfx.play(); }
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
				this->itemIcon->bindAndDraw(2, 0);
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
		currentTime += Time::DeltaTimeInSeconds();
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
			this->redMesh->scale = this->scale;
			this->mesh->matKey[0] = "Red.png";
			drawRed = true;
		}
		else
		{
			this->mesh->matKey[0] = "olive.jpg";
			drawRed = false;
		}
	}
	else
	{
		this->mesh->matKey[0] = "olive.jpg";
		drawRed = false;
		this->setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		if (this->explosionMesh->scale.x >= 1)
		{
			this->particles->BindAndDraw(2);
			this->particles->setSize(3.0f);
		}
		else if (this->particles != nullptr && pickedUp)
		{
			if (tStruct.getDt() < 1)
			{
				this->particles->setColor(1, 1, 1);
				this->particles->setSize(3.0f);
				this->particles->BindAndDraw(0);
			}
		}
		else if (!this->pickedUp && !destructionIsImminent)
		{
			this->particles->setColor(color);
			this->particles->setSize(1.0f);
			this->particles->BindAndDraw(4);
		}
		/*else*/ 

	}

}


void Grenade::drawFresnel()
{	
	DirectX::XMFLOAT3 test = this->explosionMesh->scale;
	float delta = Time::DeltaTimeInSeconds();
	if (test.x > 1)
	{
		GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
		this->explosionMesh->DrawWithMat();
		this->explosionMesh->scale = DirectX::XMFLOAT3(this->explosionMesh->scale.x - (delta * 50), this->explosionMesh->scale.y - (delta * 50), this->explosionMesh->scale.z - (delta * 50));
		this->explosionMesh->UpdateCB(explodePosition, rotation, this->explosionMesh->scale);
	}

	if (!this->pickedUp && !destructionIsImminent)
	{
		float constant = 0.2f;
		GPU::immediateContext->PSSetConstantBuffers(2, 1, this->fresnelBuffer.getReferenceOf());
		DirectX::XMFLOAT3 test = this->scale;
		this->scale = DirectX::XMFLOAT3(test.x + constant, test.y + constant, test.z + constant);
		this->draw();
		this->scale = test;
	}
}

void Grenade::useItem(const Player* playerHoldingItem)
{
	if (!this->destructionIsImminent)
	{
		this->destructionIsImminent = true;
		timer.resetStartTime();
	}
}

bool Grenade::getExploded() const
{
	return this->exploded;
}

void Grenade::setExploded(const bool& onOff)
{
	this->exploded = onOff;
}

void Grenade::draw()
{
	if (drawRed)
	{
		this->redMesh->UpdateCB(position, rotation, scale);
		this->redMesh->DrawWithMat();
	}
	else
	{
		this->mesh->UpdateCB(position, rotation, scale);
		this->mesh->DrawWithMat();
	}
}
