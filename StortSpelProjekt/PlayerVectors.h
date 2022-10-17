#pragma once
#include <d3d11.h>
#include "Player.h"

class PlayerVectors
{
private:
	Player* player;
	ID3D11Buffer* vBuffer;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	Vertex vectors[6];

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