#pragma once
#include <wrl/client.h>
#include "stdafx.h"

class BilboardObject
{
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>> bilboardTX;
	std::vector <Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> bilboardTXView;
	Microsoft::WRL::ComPtr <ID3D11Buffer> vertexBuffer;
	DirectX::XMFLOAT3 position;

public:
	BilboardObject(const std::vector<std::string>& textureNames, const DirectX::XMFLOAT3& position);
	void setPosition(const DirectX::XMFLOAT3& position);
	DirectX::XMFLOAT3 getPosition();
	int getNrOfTextures();
	void PSbindSrv(const int& textureIndex, const int& slot);
	void VSbindVTXBuffer();
	void bindAndDraw(const int& textureIndex, const int& slot);

private:
	void updateBuffer();
};