#include "PhysicsWorld.h"

#include <time.h>
#include "PhysicsComponent.h"
#include "DirectXMathHelper.h"
#include "ServerGravField.h"

void PhysicsWorld::setUpBaseScenario()
{
	playerBox = new PhysicsComponent();
	playerBox->initiateComponent(&this->com, this->world);
	playerBox->setType(reactphysics3d::BodyType::KINEMATIC);

	world->setIsDebugRenderingEnabled(true);
}


PhysicsWorld::PhysicsWorld(std::string worldName)
{
	// Create the world settings 
	reactphysics3d::PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 10;
	settings.isSleepingEnabled = false;
	settings.worldName = worldName;
	settings.isSleepingEnabled = true;
	settings.gravity = reactphysics3d::Vector3(0, 0, 0);
	settings.defaultBounciness = 0.1f;
	settings.defaultFrictionCoefficient = 0.1f;
	settings.defaultTimeBeforeSleep = 1.f;

	// Create the physics world with your settings 
	world = com.createPhysicsWorld(settings);
	this->setUpBaseScenario();
}


PhysicsWorld::~PhysicsWorld()
{
	for (int i = 0; i < physObjects.size(); i++)
	{
		delete physObjects[i];
	}
	delete playerBox;
	if (world != nullptr) com.destroyPhysicsWorld(world);
}

void PhysicsWorld::setPlanets(std::vector<Planet*>& planets)
{
	this->planets = planets;
}

void PhysicsWorld::update(const float& dt)
{
	this->addForceToObjects(dt);
	world->update(dt);
}

void PhysicsWorld::addForceToObjects(const float& dt)
{
	float constant = 10.f;
	for (int i = 0; i < this->physObjects.size(); i++)
	{
		/*temp = this->physObjects[i]->getPosition();
		grav = normalizeXMFLOAT3(DirectX::XMFLOAT3(-temp.x, -temp.y, -temp.z));43
		this->physObjects[i]->applyForceToCenter(this->physObjects[i]->getMass() * reactphysics3d::Vector3(9820.f * grav.x * dt * constant, 9820.f * grav.y * dt * constant, 9820.f * grav.z * dt * constant));*/

		GravityField* field = planets[0]->getClosestField(planets, this->physObjects[i]->getPosV3());
		grav = field->calcGravFactor(this->physObjects[i]->getPosV3());
		this->physObjects[i]->applyForceToCenter(this->physObjects[i]->getMass() * reactphysics3d::Vector3(98.2f * grav.x * dt * constant, 98.2f * grav.y * dt * constant, 98.2f * grav.z * dt * constant));
		//if (this->physObjects[i]->getParent() != nullptr) this->physObjects[i]->getParent()->setGravityField(field);
	}
}

PhysicsComponent* PhysicsWorld::getPlayerBox() const
{
	return this->playerBox;
}

void PhysicsWorld::addBoxToWorld(DirectX::XMFLOAT3 dimensions, float mass, DirectX::XMFLOAT3 position)
{
	if (position.x == 0 && position.y == 10 && position.z == 0)
	{
		float x = 50.f - 2.f * (float)(rand() % 61);
		float y = 50.f - 2.f * (float)(rand() % 61);
		float z = 50.f - 2.f * (float)(rand() % 61);
		position = DirectX::XMFLOAT3(x, y, z);
	}
	UINT vectorSize = UINT(this->physObjects.size());
	physObjects.emplace_back(new PhysicsComponent());
	physObjects[vectorSize]->initiateComponent(&this->com, this->world);
	physObjects[vectorSize]->setPosition(reactphysics3d::Vector3(position.x, position.y, position.z));
	physObjects[vectorSize]->setLinearDampning(1.3f);
}

void PhysicsWorld::addPhysComponent(Component& pos, reactphysics3d::CollisionShapeName shape, const DirectX::XMFLOAT3& scale)
{
	PhysicsComponent* newComp = new PhysicsComponent();
	newComp->initiateComponent(&this->com, this->world, shape, scale);
	pos.setPhysCompServer(newComp);
	newComp->setPosition({ pos.getposition('x'), pos.getposition('y'), pos.getposition('z') });
	newComp->setLinearDampning(0.3f);
	physObjects.emplace_back(newComp);
}

void PhysicsWorld::addPhysComponent(PhysicsComponent* newComp, const DirectX::XMFLOAT3& pos, reactphysics3d::CollisionShapeName shape, const DirectX::XMFLOAT3& scale)
{
	newComp->initiateComponent(&this->com, this->world, shape, scale);
	newComp->setPosition({ pos.x, pos.y, pos.z });
	newComp->setLinearDampning(0.3f);
	physObjects.emplace_back(newComp);
}

