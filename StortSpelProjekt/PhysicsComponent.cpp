#include "PhysicsComponent.h"
#include "GameObject.h"

void PhysicsComponent::createRigidBody(const reactphysics3d::Transform& transform)
{
	this->rigidBody = this->worldPtr->createRigidBody(transform);
}

void PhysicsComponent::deallocate()
{
	this->worldPtr->destroyRigidBody(this->rigidBody);
	this->worldPtr->destroyCollisionBody(this->collisionBody);

	//Now we need to destroy the correct shape, and to get the shape we get the name and destroy accordingly
	reactphysics3d::CollisionShapeName shapeType = this->shape->getName();
	reactphysics3d::BoxShape* a = dynamic_cast<reactphysics3d::BoxShape*>(this->shape);
	reactphysics3d::SphereShape* b = dynamic_cast<reactphysics3d::SphereShape*>(this->shape);
	reactphysics3d::ConvexMeshShape* c = dynamic_cast<reactphysics3d::ConvexMeshShape*>(this->shape);
	reactphysics3d::ConvexMeshShape* d = dynamic_cast<reactphysics3d::ConvexMeshShape*>(this->shape);
	switch (shapeType)
	{
	case reactphysics3d::CollisionShapeName::BOX:
		this->comPtr->destroyBoxShape(a);
		break;
	case reactphysics3d::CollisionShapeName::SPHERE:
		this->comPtr->destroySphereShape(b);
		break;
	case reactphysics3d::CollisionShapeName::CONVEX_MESH:
		this->comPtr->destroyConvexMeshShape(c);
		break;
	case reactphysics3d::CollisionShapeName::CAPSULE:
		this->comPtr->destroyConvexMeshShape(d);
		break;
	default:
		std::cout << "Error: Non-Applicable shape was returned";
		break;
	}
}

PhysicsComponent::PhysicsComponent()
	:comPtr(nullptr), worldPtr(nullptr), parent(nullptr),
	rigidBody(nullptr), shape(nullptr), collider(nullptr)
{
}

PhysicsComponent::~PhysicsComponent()
{
	//We do not need to deallocate anything since destroying the world will destroy everything,
	//however, if you want to destory the object but not the world we can use this
	if (this->rigidBody != nullptr && this->shape != nullptr && this->collider != nullptr) this->deallocate();
}

void PhysicsComponent::initiateComponent(reactphysics3d::PhysicsCommon* com, reactphysics3d::PhysicsWorld* world, const reactphysics3d::CollisionShapeName& shape)
{
	this->comPtr = com;
	this->worldPtr = world;

	//Creates basic physics component
	this->createRigidBody();
	this->setShape(shape);
	this->addCollider();

	//Gives some values	
	this->setType();
	this->setMass();
	this->rigidBody->enableGravity(false);

	this->collisionBody = this->worldPtr->createCollisionBody(reactphysics3d::Transform::identity());
	this->collisionBody->addCollider(this->shape, reactphysics3d::Transform::identity());
}

void PhysicsComponent::setType(const reactphysics3d::BodyType& physicsType)
{
	this->rigidBody->setType(physicsType);
}

void PhysicsComponent::setShape(const reactphysics3d::CollisionShapeName& shapeType)
{
	reactphysics3d::Transform defaultTransform = reactphysics3d::Transform::identity();
	switch (shapeType)
	{
	case reactphysics3d::CollisionShapeName::BOX:
		this->shape = this->comPtr->createBoxShape(reactphysics3d::Vector3(4*0.35f, 4 * 0.35f, 4 * 0.35f));
		break;
	case reactphysics3d::CollisionShapeName::SPHERE:
		this->shape = this->comPtr->createSphereShape(4*0.5f);
		break;
	/*case reactphysics3d::CollisionShapeName::CONVEX_MESH:
		this->comPtr->destroyConvexMeshShape(c);
		break;
	case reactphysics3d::CollisionShapeName::CAPSULE:
		this->comPtr->destroyConvexMeshShape(d);
		break;*/
	default:
		std::cout << "Error: Non-Applicable shape was created";
		break;
	}
}

void PhysicsComponent::addCollider(const reactphysics3d::Transform& transform)
{
	this->collider = this->rigidBody->addCollider(this->shape, transform);
}

void PhysicsComponent::setMass(const float& mass)
{
	this->rigidBody->setMass(mass);
}

void PhysicsComponent::setLinearDampning(const float& factor)
{
	this->rigidBody->setLinearDamping(factor);
}

void PhysicsComponent::setAngularDampning(const float& factor)
{
	this->rigidBody->setAngularDamping(factor);
}

void PhysicsComponent::setIsAllowedToSleep(const bool& allowedToSleep)
{
	this->rigidBody->setIsAllowedToSleep(allowedToSleep);
}

void PhysicsComponent::setIsSleeping(const bool& sleep)
{
	this->rigidBody->setIsSleeping(sleep);
}

void PhysicsComponent::applyForceToCenter(const reactphysics3d::Vector3& force)
{
	this->rigidBody->applyWorldForceAtCenterOfMass(force);
}

void PhysicsComponent::applyForceToPoint(const reactphysics3d::Vector3& force, const reactphysics3d::Vector3& point)
{
	this->rigidBody->applyWorldForceAtLocalPosition(force, point);
}

void PhysicsComponent::applyWorldTorque(const reactphysics3d::Vector3& force)
{
	this->rigidBody->applyWorldTorque(force);
}

void PhysicsComponent::applyLocalTorque(const reactphysics3d::Vector3& force)
{
	this->rigidBody->applyLocalTorque(force);
}

void PhysicsComponent::setPosition(const reactphysics3d::Vector3& position)
{
	this->rigidBody->setTransform(reactphysics3d::Transform(position, this->rigidBody->getTransform().getOrientation()));
}

reactphysics3d::BodyType PhysicsComponent::getType() const
{
	return this->rigidBody->getType();
}

reactphysics3d::Vector3 PhysicsComponent::getPosition() const
{
	return this->rigidBody->getTransform().getPosition();
}

reactphysics3d::Quaternion PhysicsComponent::getRotation() const
{
	return this->rigidBody->getTransform().getOrientation();
}

reactphysics3d::CollisionShapeName PhysicsComponent::getTypeName() const
{
	return this->shape->getName();
}

reactphysics3d::Collider* PhysicsComponent::getCollider() const
{
	return this->collider;
}

reactphysics3d::CollisionBody* PhysicsComponent::getCollisionBody()
{
	this->collisionBody->setTransform(this->rigidBody->getTransform());
	return this->collisionBody;
}

DirectX::SimpleMath::Vector3 PhysicsComponent::getPosV3() const
{
	reactphysics3d::Vector3 temp = this->getPosition();
	return {temp.x, temp.y, temp.z};
}

bool PhysicsComponent::testPointInside(const reactphysics3d::Vector3& point)
{
	this->collisionBody->setTransform(this->rigidBody->getTransform());
	return this->collisionBody->testPointInside(point);
}

bool PhysicsComponent::testBodiesOverlap(PhysicsComponent* other)
{
	this->collisionBody->setTransform(this->rigidBody->getTransform());
	return this->collisionBody->testAABBOverlap(other->getCollisionBody()->getAABB());
}

float PhysicsComponent::getMass() const
{
	return this->rigidBody->getMass();
}

float PhysicsComponent::getLinearDampning() const
{
	return this->rigidBody->getLinearDamping();
}

float PhysicsComponent::getAngularDampning() const
{
	return this->getAngularDampning();
}

void PhysicsComponent::setParent(GameObject* parent)
{
	this->parent = parent;
}

void PhysicsComponent::resetPhysicsObject()
{
	this->deallocate();	//After this we can re-define a new physics component
}
