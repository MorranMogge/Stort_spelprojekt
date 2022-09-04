#pragma once

#include "GPU.h"
#include <fstream>

class PixelShader final
{
	Microsoft::WRL::ComPtr<ID3D11PixelShader> data;

public:

	void Load(const std::string path)
	{
		std::ifstream reader(path + ".cso", std::ios::binary | std::ios::ate);
		if (reader.is_open())
		{
			std::string shaderData;
			reader.seekg(0, std::ios::end);
			shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
			reader.seekg(0, std::ios::beg);

			shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

			GPU::device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, data.GetAddressOf());

			shaderData.clear();
			reader.close();

		}
	}
	void Bind()
	{
		GPU::immediateContext->PSSetShader(data.Get(), nullptr, 0);
	}
};