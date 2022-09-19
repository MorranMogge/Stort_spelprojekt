#pragma once
#include <wrl.h>
#include <vector>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include "GPU.h"
#include "ConstantBufferNew.h"


using namespace DirectX;
using namespace std;
using namespace Microsoft;
using namespace WRL;

class ParticleEmitter
{
private:
	ComPtr <ID3D11UnorderedAccessView> PT_UAV;			//UAV for vertex shader
	ConstantBufferNew<particleStruct>  PT_vertexBuffer;	//Vertex buffer
	ComPtr<ID3D11Buffer> PT_posBuffer;					//Position buffer
	vector<particleStruct> PtData;						//Particle Data (pos, delta time, lifetime)

	
	XMFLOAT2 minMaxLifetime;							//Min and max lifetime of particles
	XMFLOAT3 Position;									//Emitter Position
	int nrOfParticles;									//Nr of points in buffer
	int maxHeight = 10;									//Respawn height (gör på compute shader?)
	bool active;										//Particle emitter state 

public:
	ParticleEmitter(ID3D11Device * device, XMFLOAT3 Pos, int nrOfPT, XMFLOAT2 minMaxLifetime, int randRange = 10);
	ID3D11Buffer* getVTXBuffer();
	ID3D11Buffer* getPosBuffer();
	void BindAndDraw(ID3D11Buffer*& timeBuffer);
	ID3D11UnorderedAccessView* getUAV();
	int getNrOfParticles();
	void updateBuffer(ID3D11DeviceContext* immediateContext);
	XMFLOAT3 getPosition();
	void setPosition(XMFLOAT3 Pos);
	void setActive(bool onOrOff);
	bool isActive();
};