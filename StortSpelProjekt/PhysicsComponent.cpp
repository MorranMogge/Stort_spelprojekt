#include "PhysicsComponent.h"

void PhysicsComponent::createRigidBody(reactphysics3d::Transform transform)
{
	this->rigidBody = this->worldPtr->createRigidBody(transform);
}

void PhysicsComponent::deallocate()
{
	this->worldPtr->destroyRigidBody(this->rigidBody);

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
	:comPtr(nullptr), worldPtr(nullptr), 
	rigidBody(nullptr), shape(nullptr), collider(nullptr)
{
}

PhysicsComponent::~PhysicsComponent()
{
	//We do not need to deallocate anything since destroying the world will destroy everything,
	//however, if you want to destory the object but not the world we can use this
	if (this->rigidBody != nullptr && this->shape != nullptr && this->collider != nullptr) this->deallocate();
}

void PhysicsComponent::initiateComponent(reactphysics3d::PhysicsCommon* com, reactphysics3d::PhysicsWorld* world)
{
	this->comPtr = com;
	this->worldPtr = world;

	//Creates basic physics component
	this->createRigidBody();
	this->setShape();
	this->addCollider();

	//Gives some values	
	this->setType();
	this->setMass();
	this->rigidBody->enableGravity(false);
}

void PhysicsComponent::setType(reactphysics3d::BodyType physicsType)
{
	this->rigidBody->setType(physicsType);
}

void PhysicsComponent::setShape(reactphysics3d::CollisionShapeName shapeType)
{
	reactphysics3d::Transform defaultTransform = reactphysics3d::Transform::identity();
	switch (shapeType)
	{
	case reactphysics3d::CollisionShapeName::BOX:
		this->shape = this->comPtr->createBoxShape(reactphysics3d::Vector3(0.5f, 0.5f, 0.5f));
		break;
	case reactphysics3d::CollisionShapeName::SPHERE:
		this->shape = this->comPtr->createSphereShape(0.5f);
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

void PhysicsComponent::addCollider(reactphysics3d::Transform transform)
{
	this->collider = this->rigidBody->addCollider(this->shape, transform);
}

void PhysicsComponent::setMass(float mass)
{
	this->rigidBody->setMass(mass);
}

void PhysicsComponent::setLinearDampning(float factor)
{
	this->rigidBody->setLinearDamping(factor);
}

void PhysicsComponent::setAngularDampning(float factor)
{
	this->rigidBody->setAngularDamping(factor);
}

void PhysicsComponent::setIsAllowedToSleep(bool allowedToSleep)
{
	this->rigidBody->setIsAllowedToSleep(allowedToSleep);
}

void PhysicsComponent::setIsSleeping(bool sleep)
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

void PhysicsComponent::setPosition(const reactphysics3d::Vector3& position)
{
	this->rigidBody->setTransform(reactphysics3d::Transform(position, this->rigidBody->getTransform().getOrientation()));
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

void PhysicsComponent::resetPhysicsObject()
{
	this->deallocate();	//After this we can re-define a new physics component
}
