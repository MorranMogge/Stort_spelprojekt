#pragma once
#include "stdafx.h"
#include "Vertex.h"

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
	void setShape(const reactphysics3d::CollisionShapeName& shapeType = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	void addCollider(const reactphysics3d::Transform& transform = reactphysics3d::Transform::identity());
	void deallocate();

public:
	PhysicsComponent();
	~PhysicsComponent();
	
	//Important to do this function first, since it allows us to access the common and world

	//Initialises the variables: rigidbody, shape and collider
	void initiateComponent(reactphysics3d::PhysicsCommon* com, reactphysics3d::PhysicsWorld* world, const reactphysics3d::CollisionShapeName& shape = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));

	//Change properties of the physics component

	//Set the Physcics Type of the component
	void setType(const reactphysics3d::BodyType& physicsType = reactphysics3d::BodyType::DYNAMIC);
	//Set the mass of the component
	void setMass(const float& mass = 1.0f);
	//Set the scale of the Sphere
	void setScale(const float& scale = 0.5f);
	//Set the scale of the Box
	void setScale(const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.f,1.f,1.f));
	//Set the scale of the Box
	void setScale(const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3(1.f, 1.f, 1.f));
	
	//Set the shape to Box
	void setBoxShape(const DirectX::XMFLOAT3& dimensions = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	//Set the shape to Sphere
	void setSphereShape(const float& radius = 0.5f);
	//Set the shape to Convex Mesh
	void setConvexMeshShape(const std::vector<Vertex>& vertices); //Implement this later

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
	void setRotation(const reactphysics3d::Quaternion& rotation);
	void setRotation(const DirectX::SimpleMath::Quaternion& rotation);
	void setTransform(const reactphysics3d::Transform& transform);
	void resetForce()const;
	void resetTorque()const;

	//Get information
	reactphysics3d::BodyType getType()const;
	reactphysics3d::Vector3 getPosition()const;
	reactphysics3d::Quaternion getRotation()const;
	reactphysics3d::CollisionShapeName getTypeName()const;
	reactphysics3d::Collider* getCollider()const;
	reactphysics3d::RigidBody* getRigidBody()const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	bool raycast(const reactphysics3d::Ray& ray, reactphysics3d::RaycastInfo& rayInfo)const;

	bool testPointInside(const reactphysics3d::Vector3& point)const;	//Can be used instead of "testBodiesOverlap()", check point instead of AABB
	bool testBodiesOverlap(PhysicsComponent* other)const;				//Better collision detection

	float getMass()const;
	float getLinearDampning()const;
	float getAngularDampning()const;
	GameObject* getParent()const;

	void setParent(GameObject* parent);
	void resetPhysicsObject(); //This might be used when transfering to a different world
};