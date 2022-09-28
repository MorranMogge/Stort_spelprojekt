#pragma once
#include <reactphysics3d\reactphysics3d.h>

class PhysicsComponent
{
private:
	reactphysics3d::PhysicsCommon* comPtr;	//Used to destroy and handle certain components
	reactphysics3d::PhysicsWorld* worldPtr; //Used to create the physics object in the correct world

	reactphysics3d::CollisionShape* shape;	//Can be capsule, box, sphere and other shapes
	reactphysics3d::RigidBody* rigidBody;	//Used to give body correct physics calculations
	reactphysics3d::Collider* collider;		//Used to make collisions happen

	void createRigidBody(reactphysics3d::Transform transform = reactphysics3d::Transform::identity());
	void setShape(reactphysics3d::CollisionShapeName shapeType = reactphysics3d::CollisionShapeName::BOX);
	void addCollider(reactphysics3d::Transform transform = reactphysics3d::Transform::identity());
	void deallocate();

public:
	PhysicsComponent();
	~PhysicsComponent();
	
	//Important to do this function first, since it allows us to access the common and world
	void initiateComponent(reactphysics3d::PhysicsCommon* com, reactphysics3d::PhysicsWorld* world);

	//Change properties of the physics component
	void setType(reactphysics3d::BodyType physicsType = reactphysics3d::BodyType::DYNAMIC);
	void setMass(float mass = 1.0f);

	void setLinearDampning(float factor = 0.0f);
	void setAngularDampning(float factor = 0.0f);

	//When sleeping, the physics object will not update within the world
	void setIsAllowedToSleep(bool allowedToSleep = true);
	void setIsSleeping(bool sleep = false);

	//Apply force/and or change positions
	void applyForceToCenter(const reactphysics3d::Vector3& force);
	void applyForceToPoint(const reactphysics3d::Vector3& force, const reactphysics3d::Vector3& point);
	void setPosition(const reactphysics3d::Vector3& position);

	//Get information
	reactphysics3d::Vector3 getPosition()const;
	reactphysics3d::Quaternion getRotation()const;
	reactphysics3d::CollisionShapeName getTypeName()const;

	float getMass()const;
	float getLinearDampning()const;
	float getAngularDampning()const;

	void resetPhysicsObject(); //This might be used when transfering to a different world
};