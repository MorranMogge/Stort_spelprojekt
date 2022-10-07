#pragma once
#include <reactphysics3d\reactphysics3d.h>
#include <SimpleMath.h>

class GameObject;

class PhysicsComponent
{
private:
	GameObject* parent;

	reactphysics3d::PhysicsCommon* comPtr;	//Used to destroy and handle certain components
	reactphysics3d::PhysicsWorld* worldPtr; //Used to create the physics object in the correct world

	reactphysics3d::CollisionShape* shape;	//Can be capsule, box, sphere and other shapes
	reactphysics3d::RigidBody* rigidBody;	//Used to give body correct physics calculations
	reactphysics3d::Collider* collider;		//Used to make collisions happen

	void createRigidBody(const reactphysics3d::Transform& transform = reactphysics3d::Transform::identity());
	void setShape(const reactphysics3d::CollisionShapeName& shapeType = reactphysics3d::CollisionShapeName::BOX);
	void addCollider(const reactphysics3d::Transform& transform = reactphysics3d::Transform::identity());
	void deallocate();

public:
	PhysicsComponent();
	~PhysicsComponent();
	
	//Important to do this function first, since it allows us to access the common and world
	void initiateComponent(reactphysics3d::PhysicsCommon* com, reactphysics3d::PhysicsWorld* world, reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX);

	//Change properties of the physics component
	void setType(const reactphysics3d::BodyType& physicsType = reactphysics3d::BodyType::DYNAMIC);
	void setMass(const float& mass = 1.0f);

	void setLinearDampning(const float& factor = 0.0f);
	void setAngularDampning(const float& factor = 0.0f);

	//When sleeping, the physics object will not update within the world
	void setIsAllowedToSleep(const bool& allowedToSleep = true);
	void setIsSleeping(const bool& sleep = false);

	//Apply force/and or change positions
	void applyForceToCenter(const reactphysics3d::Vector3& force);
	void applyForceToPoint(const reactphysics3d::Vector3& force, const reactphysics3d::Vector3& point);
	void applyWorldTorque(const reactphysics3d::Vector3& force);
	void applyLocalTorque(const reactphysics3d::Vector3& force);
	void setPosition(const reactphysics3d::Vector3& position);

	//Get information
	reactphysics3d::Vector3 getPosition()const;
	reactphysics3d::Quaternion getRotation()const;
	reactphysics3d::CollisionShapeName getTypeName()const;
	reactphysics3d::Collider* getCollider()const;
	DirectX::SimpleMath::Vector3 getPosV3()const;

	float getMass()const;
	float getLinearDampning()const;
	float getAngularDampning()const;

	void setParent(GameObject* parent);
	void resetPhysicsObject(); //This might be used when transfering to a different world
};