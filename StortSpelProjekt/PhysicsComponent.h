#pragma once
#include <reactphysics3d\reactphysics3d.h>

class PhysicsComponent
{
private:
	reactphysics3d::PhysicsWorld* worldPtr; //Used to create the physics object in the correct world
	
	reactphysics3d::CollisionShape* shape;	//Can be capsule, box, sphere and other shapes
	reactphysics3d::RigidBody* rigidBody;	//Used to give body correct physics calculations
	reactphysics3d::Collider* collider;		//Used to make collisions happen

public:
	PhysicsComponent();
	~PhysicsComponent();


};