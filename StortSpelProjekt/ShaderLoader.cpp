#include "ShaderLoader.h"

bool LoadVertexShader(ID3D11Device* device, ID3D11VertexShader*& vShader, std::string& vShaderByteCode, const std::string path)
{
	std::ifstream reader("../Shaders/" + path + ".cso", std::ios::binary | std::ios::ate);

	if (reader.is_open())
	{
		reader.seekg(0, std::ios::end);
		vShaderByteCode.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);
		vShaderByteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

		reader.close();
		HRESULT hr = device->CreateVertexShader(vShaderByteCode.c_str(), vShaderByteCode.length(), nullptr, &vShader);

		return !FAILED(hr);
	}
	ErrorLog::Log("Failed to create Vertex shader!");
	return false;
}

bool LoadVertexShader(ID3D11Device* device, ID3D11VertexShader*& vShader, const std::string path)
{
	std::ifstream reader("../x64/Debug/" + path + ".cso", std::ios::binary | std::ios::ate);

	if (reader.is_open())
	{
		std::string vShaderByteCode;
		reader.seekg(0, std::ios::end);
		vShaderByteCode.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);
		vShaderByteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

		reader.close();
		HRESULT hr = device->CreateVertexShader(vShaderByteCode.c_str(), vShaderByteCode.length(), nullptr, &vShader);

		return !FAILED(hr);
	}
	ErrorLog::Log("Failed to create Domain shader!");
	return false;
}

bool LoadDomainShader(ID3D11Device* device, ID3D11DomainShader*& dShader, const std::string path)
{
	std::ifstream reader("../x64/Debug/" + path + ".cso", std::ios::binary | std::ios::ate);

	if (reader.is_open())
	{
		std::string vShaderByteCode;
		reader.seekg(0, std::ios::end);
		vShaderByteCode.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);
		vShaderByteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

		reader.close();
		HRESULT hr = device->CreateDomainShader(vShaderByteCode.c_str(), vShaderByteCode.length(), nullptr, &dShader);

		return !FAILED(hr);
	}
	ErrorLog::Log("Failed to create Domain shader!");
	return false;
}

bool LoadHullShader(ID3D11Device* device, ID3D11HullShader*& hShader, const std::string path)
{
	std::ifstream reader("../x64/Debug/" + path + ".cso", std::ios::binary | std::ios::ate);

	if (reader.is_open())
	{
		std::string vShaderByteCode;
		reader.seekg(0, std::ios::end);
		vShaderByteCode.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);
		vShaderByteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

		reader.close();
		HRESULT hr = device->CreateHullShader(vShaderByteCode.c_str(), vShaderByteCode.length(), nullptr, &hShader);

		return !FAILED(hr);
	}
	ErrorLog::Log("Failed to create Hull shader!");
	return false;
}

bool LoadPixelShader(ID3D11Device* device, ID3D11PixelShader*& pShader, const std::string path)
{
	std::ifstream reader("../Shaders/" + path + ".cso", std::ios::binary | std::ios::ate);

	if (reader.is_open())
	{
		std::string vShaderByteCode;
		reader.seekg(0, std::ios::end);
		vShaderByteCode.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);
		vShaderByteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

		reader.close();
		HRESULT hr = device->CreatePixelShader(vShaderByteCode.c_str(), vShaderByteCode.length(), nullptr, &pShader);

		return !FAILED(hr);
	}
	ErrorLog::Log("Failed to create Pixel shader!");
	return false;
}

bool LoadComputeShader(ID3D11Device* device, ID3D11ComputeShader*& cShader, const std::string path)
{
	std::ifstream reader("../Shaders/" + path + ".cso", std::ios::binary | std::ios::ate);

	if (reader.is_open())
	{
		std::string vShaderByteCode;
		reader.seekg(0, std::ios::end);
		vShaderByteCode.reserve(static_cast<unsigned int>(reader.tellg()));
		reader.seekg(0, std::ios::beg);
		vShaderByteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

		reader.close();
		HRESULT hr = device->CreateComputeShader(vShaderByteCode.c_str(), vShaderByteCode.length(), nullptr, &cShader);

		return !FAILED(hr);
	}
	ErrorLog::Log("Failed to create Compute shader!");
	return false;
}