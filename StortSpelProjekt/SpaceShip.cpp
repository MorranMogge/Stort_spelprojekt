#include "stdafx.h"
#include "SpaceShip.h"
#include "HudUI.h"

using namespace DirectX;

SpaceShip::SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, Mesh* zoneMesh, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject(useMesh, pos, DirectX::XMFLOAT3(0,0,0), id, field, scale), compToComplete(nrofComp), currentComponents(0), team(team), animate(false), counter(0.0f)
{
	compAddedSfx.load(L"../Sounds/random.wav");
	engineTakeOff.load(L"../Sounds/shipTakeOff.wav");
	engineTakeOff.setVolume(0.5f);

	using namespace DirectX::SimpleMath;

	//Set rotation to gravity field
	this->setRot(this->getRotOrientedToGrav());

	//Icon initiation
	float constant = 60.0f;
	std::vector<std::string> filenames{ "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 test(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	this->rocketStatusQuad = new BilboardObject(filenames, test);
	this->rocketStatusQuad->setOffset(constant);

	//Particles
	this->particles = new ParticleEmitter(pos, this->getRotOrientedToGrav(), 26, DirectX::XMFLOAT2(2, 5), 2);
	this->particles2 = new ParticleEmitter(pos, this->getRotOrientedToGrav(), 26, DirectX::XMFLOAT2(2, 5), 2);

	//CapZone
	float constant2 = 40.0f;
	DirectX::XMFLOAT3 offset2(upDir.x * constant2, upDir.y * constant2, upDir.z * constant2);
	zone = new CaptureZone(zoneMesh, offset2, this->getRotXM(),field, DirectX::XMFLOAT3(8, 8, 8));

	//Team switch
	switch (team)
	{
	case 0:
		HudUI::red = this;
		mesh->matKey[0] = "spaceshipTexture1.jpg";
		break;

	case 1:
		HudUI::blue = this;
		mesh->matKey[0] = "spaceshipTexture2.jpg";
		break;
	}
}

SpaceShip::SpaceShip(const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, Mesh* zoneMesh, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject("../Meshes/rocket", pos, DirectX::XMFLOAT3(0, 0, 0), id, field, scale), compToComplete(nrofComp), currentComponents(0), team(team), animate(false), counter(0.0f)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	//Set rotation to gravity field
	this->setRot(this->getRotOrientedToGrav());

	//Icon initiation
	float constant = 60.f;
	std::vector<std::string> filenames{ "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 test(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	this->rocketStatusQuad = new BilboardObject(filenames, test);
	this->rocketStatusQuad->setOffset(constant);

	//Particles
	this->particles = new ParticleEmitter(pos, this->getRotOrientedToGrav(), 26, DirectX::XMFLOAT2(2, 5), 2);
	this->particles2 = new ParticleEmitter(pos, this->getRotOrientedToGrav(), 26, DirectX::XMFLOAT2(2, 5), 2);

	//CapZone
	float constant2 = 40.0f;
	DirectX::XMFLOAT3 offset2(upDir.x * constant2, upDir.y * constant2, upDir.z * constant2);
	zone = new CaptureZone(zoneMesh, offset2, this->getRotXM(), field, DirectX::XMFLOAT3(8, 8, 8));

	//Team switch
	switch (team)
	{
	case 0:
		HudUI::red = this;
		mesh->matKey[0] = "spaceshipTexture1.jpg"; 
		break;

	case 1:
		HudUI::blue = this;
		mesh->matKey[0] = "spaceshipTexture2.jpg";
		break;

	}
}

SpaceShip::~SpaceShip()
{
	//Delete components?
	//for (int i = 0; i < this->components.size(); i++)
	//{
	//	delete this->components.at(i);
	//}
	delete this->zone;
	delete this->particles;
	delete this->particles2;
	delete this->rocketStatusQuad;
}

int SpaceShip::getTeam() const
{
	return this->team;
}

int SpaceShip::getNrOfComponents() const
{
	return this->currentComponents;
}

bool SpaceShip::detectedComponent(GameObject* objectToCheck)
{
	bool didDetect = false;

	if (this->withinRadious(objectToCheck, 8.0f))
	{
		didDetect = true;
	}
	else
	{
		if (this->scale.y != 2.f) this->setScale(DirectX::XMFLOAT3(2, 2, 2));
	}
	return didDetect;
}

bool SpaceShip::detectedComponent(Component* componentToCheck)
{
	bool didDetect = false;

	if (this->withinRadious(componentToCheck, 8.0f))
	{
		didDetect = true;
	}
	else
	{
		if (this->scale.y != 2.f) this->setScale(DirectX::XMFLOAT3(2, 2, 2));
	}
	return didDetect;
}

void SpaceShip::addComponent()
{
	this->currentComponents++;
	if (this->currentComponents >= 4) { engineTakeOff.stop(); engineTakeOff.play(); }
	else
	{
		compAddedSfx.stop();
		compAddedSfx.play();
	}
}

void SpaceShip::update()
{
}

void SpaceShip::drawQuad()
{
	if (!(this->currentComponents >= this->compToComplete))
	{
		rocketStatusQuad->bindAndDraw(this->currentComponents, 0);//Changes texture depending on components
	}
}

bool SpaceShip::getCompletion() const
{
	return this->currentComponents >= this->compToComplete;
}

void SpaceShip::drawParticles()
{

	//Update particle movement
	if (this->particles != nullptr)
	{
		DirectX::XMFLOAT3 rot = this->getRotOrientedToGrav();
		this->particles->setPosition(this->position);
		this->particles->setRotation(this->getUpDirection());
		this->particles->updateBuffer();
	}


	if (this->particles != nullptr)
	{
		this->particles->BindAndDraw(0);
	}

	if (animate)
	{
		particles2->BindAndDraw(1);
	}
}

void SpaceShip::drawFresnel()
{
	switch (this->team)
	{
	case 0:
		this->zone->setColor(DirectX::XMFLOAT3(1, 0, 0));
		break;

	case 1:
		this->zone->setColor(DirectX::XMFLOAT3(0, 0.7f, 1.0f));
		break;
	}
	this->zone->drawFresnel();
}

bool SpaceShip::isFinished()
{
	bool complete = false;
	if (this->currentComponents >= this->compToComplete)
	{
		complete = true;
	}
	return complete;
}

void SpaceShip::draw()
{
	//Team switch
	switch (this->team)
	{
	case 0:
		mesh->matKey[0] = "spaceshipTexture1.jpg";
		break;

	case 1:
		mesh->matKey[0] = "spaceshipTexture2.jpg";
		break;
	}
	this->mesh->UpdateCB(position, rotation, scale);
	this->mesh->DrawWithMat();
}


void SpaceShip::setAnimate(const bool& onOff)
{
	this->animate = onOff;
	this->counter = 0;
	this->timer.resetStartTime();
}

void SpaceShip::animateOnPickup()
{
	if (animate)
	{
		float animationDuration = 0.3f;

		this->counter += this->timer.getDt();
		auto scale = this->getScale();
		float constant = 0.5;

		if (this->counter > (animationDuration / 2))
		{
			this->setScale({ scale.x + (animationDuration / 2) - this->counter,scale.y + (animationDuration / 2) - this->counter,scale.z + (animationDuration / 2) - this->counter });
		}
		else
		{
			this->setScale({ scale.x + this->counter + constant,scale.y + this->counter + constant ,scale.z + this->counter + constant });
		}

		this->timer.resetStartTime();

		if (this->counter > animationDuration)
		{
			this->counter = 0;
			this->animate = false;
		}
	}
}
void SpaceShip::move(const DirectX::XMFLOAT3& grav, const float& deltaTime)
{
	upVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
	this->position += upVector * 18.f * deltaTime;
}