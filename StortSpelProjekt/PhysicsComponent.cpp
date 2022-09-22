#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::~PhysicsComponent()
{
	this->worldPtr->destroyRigidBody(this->rigidBody);

	//Now we need to destroy the correct shape, and to get the shape we get the name and destroy accordingly
	reactphysics3d::CollisionShapeName shapeType = this->shape->getName();
	switch (shapeType)
	{
	case reactphysics3d::CollisionShapeName::BOX:
		reactphysics3d::BoxShape* a = dynamic_cast<reactphysics3d::BoxShape*>(this->shape);
		this->com->destroyBoxShape(a);
		break;
	case reactphysics3d::CollisionShapeName::SPHERE:
		reactphysics3d::SphereShape* b = dynamic_cast<reactphysics3d::SphereShape*>(this->shape);
		this->com->destroySphereShape(b);
		break;
	case reactphysics3d::CollisionShapeName::CONVEX_MESH:
		reactphysics3d::ConvexMeshShape* c = dynamic_cast<reactphysics3d::ConvexMeshShape*>(this->shape);
		this->com->destroyConvexMeshShape(c);
		break;
	case reactphysics3d::CollisionShapeName::CAPSULE:
		reactphysics3d::ConvexMeshShape* d = dynamic_cast<reactphysics3d::ConvexMeshShape*>(this->shape);
		this->com->destroyConvexMeshShape(d);
		break;
	default:
		std::cout << "Error: Non-Applicable shape was returned";
		break;
	}
}
