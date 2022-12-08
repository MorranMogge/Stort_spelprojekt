#pragma once
#include "GameObject.h"
#include "GravityField.h"
#include "PhysicsComponent.h"
#include "ConstantBufferNew.h"

#define PLANETGRAVSIZE 10.f

class PhysicsWorld;

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

	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* normalMap;

	GravityField* gravField;
	Mesh* atmosphere;
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;	//buffer for atmosphere color
	float atmoScale;
	DirectX::SimpleMath::Vector3 position;	//Center point of the planet
	DirectX::SimpleMath::Vector3 rotation;	//Rotating Planets
	DirectX::SimpleMath::Vector3 scale;		//Since we may explore other shapes than spheres
	float gravityFactor;

	float rotDegrees;	//O-359 degrees
	DirectX::SimpleMath::Vector3 rotSpeed;
	float velocity;
	DirectX::SimpleMath::Vector3 originPoint;

	PlanetShape planetShape;
	PhysicsComponent* planetCollisionBox;

public:
	Planet(Mesh* useMesh, const DirectX::SimpleMath::Vector3& scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f), const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f), const float& gravityFactor = 4.f*9.82f, Mesh * atmoMesh = nullptr, const float& atmoScale = 10, const DirectX::SimpleMath::Vector3 & atmoColor = DirectX::XMFLOAT3(0.0f, 0.55f, 0.75f), const float& atmoDensity = 3.8f);
	~Planet();

	//Sets the shape of the planet, can be used to create box shaped planets
	void setPlanetShape(PhysicsWorld* physWorld, const PlanetShape& shape = PlanetShape::SPHERE);
	void setVelocity(const float& speed);
	void setRotationSpeed(const DirectX::SimpleMath::Vector3& rotSpeed);
	void setRotation(const DirectX::SimpleMath::Vector3& rotation);

	float getFieldFactor()const;
	DirectX::SimpleMath::Vector3 getPlanetPosition()const;
	void setPlanetPosition(const DirectX::SimpleMath::Vector3& pos);
	void setPlanetScale(const DirectX::SimpleMath::Vector3& s);
	
	void movePlanet(float offset);
	//Returns the gravity field of the planet
	GravityField* getGravityField();
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
	DirectX::SimpleMath::Vector3 getRotation() const;
	//Rotats in sphere path around point
	void rotateAroundPoint(const DirectX::XMFLOAT3& point);
	void rotatePlanet();
	void rotateMoon(const DirectX::XMFLOAT3& point, const float& speed);
	
	//set color of atmosphere
	void setColor(const DirectX::SimpleMath::Vector3& color);
	//Draws the planet using regular pipeline stages
	void drawPlanet(bool tesselation = false);
	//Draws atmosphere using inverse fresnel stage
	void drawAtmosphere();

	void setPosition(const DirectX::SimpleMath::Vector3& position);
	void setScale(const DirectX::SimpleMath::Vector3& position);
	void setSrv(ID3D11ShaderResourceView* srv);
	void setNormalMap(ID3D11ShaderResourceView* normalMap);
	void drawObjectWithNormalMap();
};