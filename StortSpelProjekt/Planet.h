#pragma once
#include "GameObject.h"
#include "GravityField.h"
#include "PhysicsWorld.h"

enum PlanetShape
{
	NONDISCLOSEDSHAPE,
	SPHERE,
	BOX
};

class Planet
{
private:
	Mesh* mesh;
	GravityField* gravField;

	DirectX::SimpleMath::Vector3 position;	//Center point of the planet
	DirectX::SimpleMath::Vector3 rotation;	//Rotating Planets
	DirectX::SimpleMath::Vector3 scale;		//Since we may explore other shapes than spheres
	float gravityFactor;

	float rotSpeed;

	PlanetShape planetShape;
	PhysicsComponent* planetCollisionBox;

public:
	Planet(Mesh* useMesh, const float& gravityFactor = 1.0f, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f), const DirectX::SimpleMath::Vector3& scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	~Planet();

	void setPlanetShape(PhysicsWorld* physWorld, const PlanetShape& shape = PlanetShape::SPHERE);
	float getFieldFactor()const;
	GravityField* getClosestField(const Planet& otherPlanet, const DirectX::SimpleMath::Vector3& position)const;
	GravityField* getGravityField()const;

	void drawPlanet();
};