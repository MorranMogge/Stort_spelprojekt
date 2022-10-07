#pragma once
#include <d3d11.h>
#include "Player.h"

class PlayerVectors
{
private:
	Player* player;
	ID3D11Buffer* vBuffer;
	Vertex vectors[6];

	UINT stride;
	UINT offset;	

	bool setUpVertexBuffer();
	void updateVertexBuffer();
public:
	PlayerVectors();
	~PlayerVectors();
	void setPlayer(Player* player);
	void drawLines();
};