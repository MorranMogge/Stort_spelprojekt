#pragma once
#include <wrl.h>
#include <vector>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include "GPU.h"
#include "ConstantBufferNew.h"
#include "TimeStruct.h"


class ParticleEmitter
{
private:
	
	Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> PT_UAV;			//UAV for vertex shader
	Microsoft::WRL::ComPtr<ID3D11Buffer> PT_vertexBuffer;				//Vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> emitterPosBuffer;				//Position buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> timeBuffer;					//Time buffer, contains delta time
	

	Microsoft::WRL::ComPtr <ID3D11BlendState> blendState;				//Blendstate for PT_Pixel shader
	TimeStruct tStruct;
	std::vector<particleStruct> PT_Data;								//Particle Data (pos, delta time, lifetime)//Add speed?
	DirectX::XMFLOAT2 minMaxLifetime;									//Min and max lifetime of particles
	DirectX::XMFLOAT3 Position;											//Emitter Position
	DirectX::XMFLOAT3 Rotation;											//Emitter Rotation
	int nrOfParticles;													//Nr of points in buffer
	bool active;														//Particle emitter state
	bool renderPassComplete;											//Particles need to render one time after switched to inactive

	void updateTimeBuffer(float delta);

public:
	ParticleEmitter(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Rot, int nrOfPT, DirectX::XMFLOAT2 minMaxLifetime, int randRange = 10);
	void BindAndDraw();
	void unbind();
	void updateBuffer();												//Updates position, rotation & state (emitter on or off)

	ID3D11Buffer* getVTXBuffer() const;
	ID3D11Buffer* getPosBuffer() const;
	ID3D11UnorderedAccessView* getUAV() const;
	int getNrOfParticles() const;
	DirectX::XMFLOAT3 getPosition() const;
	DirectX::XMFLOAT3 getRotation() const;

	void setPosition(DirectX::XMFLOAT3 Pos);
	void setRotation(DirectX::XMFLOAT3 Rot);
	void setActive(bool onOrOff);
	void setPassComplete(bool onOrOff);
	bool isActive();
	bool isPassComplete();
};