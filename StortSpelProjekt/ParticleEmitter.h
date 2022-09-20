#pragma once
#include <wrl.h>
#include <vector>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include "GPU.h"
#include "ConstantBufferNew.h"



class ParticleEmitter
{
private:
	
	Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> PT_UAV;			//UAV for vertex shader
	Microsoft::WRL::ComPtr<ID3D11Buffer> PT_vertexBuffer;				//Vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> emitterPosBuffer;				//Position buffer
	
	
	std::vector<particleStruct> PT_Data;						//Particle Data (pos, delta time, lifetime)
	DirectX::XMFLOAT2 minMaxLifetime;							//Min and max lifetime of particles
	DirectX::XMFLOAT3 Position;									//Emitter Position
	int nrOfParticles;									//Nr of points in buffer
	bool active;										//Particle emitter state 
	//int maxHeight = 10;									//Respawn height (gör på compute shader?)

public:
	ParticleEmitter(DirectX::XMFLOAT3 Pos, int nrOfPT, DirectX::XMFLOAT2 minMaxLifetime, int randRange = 10);
	ID3D11Buffer* getVTXBuffer();
	ID3D11Buffer* getPosBuffer();
	void BindAndDraw(ID3D11Buffer*& timeBuffer);
	ID3D11UnorderedAccessView* getUAV();
	int getNrOfParticles();
	void updateBuffer(ID3D11DeviceContext* immediateContext);		//Updates position & active state
	DirectX::XMFLOAT3 getPosition();
	void setPosition(DirectX::XMFLOAT3 Pos);
	void setActive(bool onOrOff);
	bool isActive();
};