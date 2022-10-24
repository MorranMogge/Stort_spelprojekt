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
	float offset = 0.0f;
	
public:
	BilboardObject(const std::vector<std::string>& textureNames, const DirectX::XMFLOAT3& position, const float& offset = 0.0f);
	BilboardObject(const std::string& textureName, const DirectX::XMFLOAT3& position, const float& offset = 0.0f);
	void setPosition(const DirectX::XMFLOAT3& position);
	DirectX::XMFLOAT3 getPosition() const;
	int getNrOfTextures() const;
	float getOffset() const;
	void setOffset(const float & offset);
	void PSbindSrv(const int& textureIndex, const int& slot);
	void VSbindVTXBuffer();
	void bindAndDraw(const int& textureIndex, const int& slot);

private:
	void updateBuffer();
};