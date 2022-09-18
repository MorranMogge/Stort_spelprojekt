#pragma once
#define NOMINMAX
#include "State.h"
#include "GuiHandler.h"
#include "Mesh.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "PerspectiveCamera.h"
#include "ShaderResourceView.h"
#include "DepthStencilView.h"
#include "BlendState.h"
#include "UnorderedAccessView.h"
#include "DepthStencilState.h"
#include "ViewPort.h"
#include "RenderTargetView.h"




//Ny
#include "ModelManager.h"

class Menu : public State
{
private:

	GuiHandler ui;

#pragma region Meshes

	ModelManager* manager;
	std::vector<Mesh2*> testMeshes;
	ID3D11ShaderResourceView* m_textureSRV;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	std::vector<int> subMeshRanges;
	std::vector<ID3D11Buffer*> testIndexBuff;

	std::vector<Mesh> meshes_Static;
	std::vector<Mesh> meshes_Dynamic;

#pragma endregion

#pragma region Camera

	PerspectiveCamera mainCamera;

	MatrixS mainCamViewProjS;
	ConstantBuffer mainCamViewProjCB;

	VectorS mainCamPosS;
	ConstantBuffer mainCamPosCB;

#pragma endregion

#pragma region Light

	__declspec(align(16))
	struct Light
	{
		enum Type { Poin = 0, Spot = 1, Directional = 2 };
		DirectX::SimpleMath::Vector4 position = { 0.0f, 0.0f, 0.0f, 0.0f };
		DirectX::SimpleMath::Vector4 direction = { 0.0f, 0.0f, 0.0f, 0.0f };
		DirectX::SimpleMath::Vector4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

		DirectX::XMFLOAT4X4 lightCamViewProj;

		float range = 0.0f;
		float cone = 0.0f;

		bool enabled = false;
		int type = 0;

	};

	__declspec(align(16))
	struct LightS
	{
		Light lits[4]; // nr of lights
	};

	LightS lightS;
	ConstantBuffer lightCB;

	PerspectiveCamera lightCam;

	MatrixS lightCamViewProjS;
	ConstantBuffer lightCamViewProjCB;

#pragma endregion

#pragma region Shadow

	DepthStencilView shadowDepthStencilView[4]; // nr of lights
	ShaderResourceView shadowSRV;
	ViewPort shadowViewPort;

#pragma endregion

#pragma region Billboard

	ShaderResourceTexture billboardSRT;
	BlendState blendState;

#pragma endregion

#pragma region Particle

	std::vector<DirectX::SimpleMath::Vector3> particlePositions;

	Microsoft::WRL::ComPtr<ID3D11Buffer> particleBuffer;
	UnorderedAccessView particleUAV;

	float simulateTime = 0.0f;
	ConstantBuffer particleTimeCB;

#pragma endregion

#pragma region DirectX

	RenderTargetView rtv;

	ViewPort viewPort;

	DepthStencilView depthStencilView;
	DepthStencilState depthStencilState;

	RasterizerState rss_Solid;
	RasterizerState rss_Wireframe;

	SamplerState samplerState_Clamp;
	SamplerState samplerState_Wrap;
	SamplerState samplerState_Shadow;

#pragma endregion

#pragma region Settings

	bool show_WireFrame = false;

#pragma endregion

#pragma region Functions

	void UpdateConstanBuffer();

	void ShadowPass();
	void GeometryPass();

#pragma endregion

public:
	Menu();
	~Menu();

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
	virtual void DrawUI() override;
};

