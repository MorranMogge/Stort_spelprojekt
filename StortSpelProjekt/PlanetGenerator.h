#pragma once
#include "State.h"
#include "Planet.h"
#include "BasicRenderer.h"
#include "ImGuiHelper.h"

struct PlanetImGuiInfo
{
	int currentSubdivisions = 0;
	int randomizedFactor = 0;
	bool renderTriangles = true;
	bool renderLines = true;
};

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

const float GoldenRatio = 1.0f / ((1.0f + sqrt(5.0f)) * 0.5f);

class PlanetGenerator : public State
{
private:
	//D3D11 stuff
	ID3D11Buffer* worldMatrixBuffer;
	ID3D11Buffer* triangleBuffer;
	ID3D11Buffer* lineBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11PixelShader* pShader;
	ID3D11VertexShader* vShader;
	DirectX::XMFLOAT4X4 worldMatrix;

	PlanetImGuiInfo planetImGuiStruct;

	//Used for sphere generation
	std::vector<Vertex*> vertices;
	std::vector<Vertex> lines;
	std::vector<Vertex*> newVertices;
	std::vector<DWORD> indices;
	PlanetType typeOfSphere;
	int lastSubdivisions;
	int horizontal;
	int vertical;


	//Used for rendering
	BasicRenderer renderer;
	ImGuiHelper imGuiHelper;
	UINT offset;
	UINT stride;
	Camera camera;
	DirectX::XMFLOAT3 cameraPosition;
	std::vector<std::vector<Triangle>> sphereMeshes;
	std::vector<std::vector<Triangle*>> triangleAdresses;

	std::vector<Mesh*> meshes;

	bool setVertexBuffers();
	bool setWorldMatrix();
	bool setIndexBuffer();
	void recreateVertexBuffers();
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
