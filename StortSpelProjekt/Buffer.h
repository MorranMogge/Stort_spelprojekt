#pragma once
#include "GPU.h"

class Buffer
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> data;

public:

	ID3D11Buffer* Get()
	{
		return data.Get();
	}
	ID3D11Buffer** GetAddressOf()
	{
		return data.GetAddressOf();
	}
	

};