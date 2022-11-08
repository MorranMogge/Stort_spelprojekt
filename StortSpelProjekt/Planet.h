#pragma once
#include "GameObject.h"
#include "GravityField.h"
#include "PhysicsWorld.h"

#define PLANETGRAVSIZE 10.f

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
	Planet(Mesh* useMesh, const DirectX::SimpleMath::Vector3& scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f), const float& gravityFactor = 1.0f);
	~Planet();

	//Sets the shape of the planet, can be used to create box shaped planets
	void setPlanetShape(PhysicsWorld* physWorld, const PlanetShape& shape = PlanetShape::SPHERE);

	float getFieldFactor()const;
	DirectX::SimpleMath::Vector3 getPlanetPosition()const;

	void movePlanet(float offset);
	//Returns the gravity field of the planet
	GravityField* getGravityField()const;
	//Returns the closest gravity field out of two planets
	GravityField* getClosestField(std::vector<Planet*>& planets, const DirectX::SimpleMath::Vector3& position)const;
	//Returns the closest gravity field factor out of two planets
	DirectX::SimpleMath::Vector3 getClosestFieldFactor(std::vector<Planet*>& planets, const DirectX::SimpleMath::Vector3& position)const;
	//Uses two planets and calculates the resulting gravity 
	DirectX::SimpleMath::Vector3 getBothGravFactor(const Planet& otherPlanet, const DirectX::SimpleMath::Vector3& position)const;
	//Uses all the planets and calculates resulting gravity
	DirectX::SimpleMath::Vector3 getAllGravFactor(const std::vector<Planet*>& planets, const DirectX::SimpleMath::Vector3& position)const;
	//Calculates whether or not the player is within the gravity field of the planet
	bool insideGravityField(const DirectX::SimpleMath::Vector3& position)const;
	PhysicsComponent* getPlanetCollider()const;
	float getSize(int index = 0)const;

	//Draws the planet using regular pipeline stages
	void drawPlanet();
};