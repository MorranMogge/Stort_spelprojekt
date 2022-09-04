#pragma once
#include "GPU.h"
#include <fstream>

class VertexShader
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> data;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

public:

	void Load(const std::string path, const D3D11_INPUT_ELEMENT_DESC* inputDesc, const unsigned int size)
	{
		std::ifstream reader(path + ".cso", std::ios::binary | std::ios::ate);

		if (reader.is_open())
		{
			std::string byteCode;
			reader.seekg(0, std::ios::end);
			byteCode.reserve(static_cast<unsigned int>(reader.tellg()));
			reader.seekg(0, std::ios::beg);
			byteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

			GPU::device->CreateVertexShader(byteCode.c_str(), byteCode.length(), nullptr, data.GetAddressOf());
			GPU::device->CreateInputLayout(inputDesc, size, byteCode.c_str(), byteCode.length(), inputLayout.GetAddressOf());

			reader.close();
		}
	}
	void Bind()
	{
		GPU::immediateContext->VSSetShader(data.Get(), nullptr, 0);
		GPU::immediateContext->IASetInputLayout(inputLayout.Get());
	}

};