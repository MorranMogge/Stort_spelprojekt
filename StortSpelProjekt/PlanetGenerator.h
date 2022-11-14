#pragma once
#include "State.h"
#include "Planet.h"
#include "BasicRenderer.h"
#include "ImGuiHelper.h"

enum PlanetType
{
	UV,
	ICO,
	QUAD
};

struct Triangle
{
	Vertex vertices[3];
	Triangle(const DirectX::XMFLOAT3& posOne, const DirectX::XMFLOAT3& posTwo, const DirectX::XMFLOAT3& posThree)
	{
		vertices[0].position = posOne;
		vertices[1].position = posTwo;
		vertices[2].position = posThree;
	}
};

//const double GoldenRatio = 1.61803398875;
//const float GoldenRatio = 1.f/(1.f + sqrt(5.f))*0.5f;
//const float GoldenRatio = (1.f + sqrt(5.f)) * 0.5f;

const float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
const float a = 1.0f;
const float GoldenRatio = 1.0f / phi;


class PlanetGenerator : public State
{
private:
	//D3D11 stuff
	ID3D11Buffer* triangleBuffer;
	ID3D11Buffer* lineBuffer;
	ID3D11PixelShader* pShader;
	DirectX::XMFLOAT3 cameraPosition;

	int horizontal;
	int vertical;
	UINT stride;
	UINT offset;
	DirectX::XMFLOAT4X4 worldMatrix;
	ID3D11Buffer* worldMatrixBuffer;
	Camera camera;

	PlanetType typeOfSphere;
	Planet* planet;
	std::vector<Mesh*> meshes;

	BasicRenderer renderer;
	ImGuiHelper imGuiHelper;

	std::vector<Vertex> vertices;
	std::vector<Vertex> lines;

	bool setVertexBuffers();
	bool setWorldMatrix();
	void createInitialSphere();
	void createQuadSphere();
	void createUVSphere();
	void createIcoSphere();

public:
	PlanetGenerator();
	virtual  ~PlanetGenerator() override;

	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
