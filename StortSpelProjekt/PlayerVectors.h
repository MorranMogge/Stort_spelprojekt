#pragma once
#include <d3d11.h>
#include "Player.h"

#define NROFVERTICES 8

class PlayerVectors
{
private:
	Player* player;
	ID3D11Buffer* vBuffer;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	Vertex vectors[NROFVERTICES];

	UINT stride;
	UINT offset;	

	bool setUpShaders();
	bool setUpVertexBuffer();
	void updateVertexBuffer();
public:
	PlayerVectors();
	~PlayerVectors();
	void setPlayer(Player* player);
	void drawLines();
};