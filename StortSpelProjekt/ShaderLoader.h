#include "stdafx.h"
#include <fstream>
#include "ErrorLog.h"

//Simplifies loading shaders

bool LoadVertexShader(ID3D11Device* device, ID3D11VertexShader*& vShader, std::string& vShaderByteCode, const std::string path);

bool LoadDomainShader(ID3D11Device* device, ID3D11DomainShader*& dShader, const std::string path);

bool LoadHullShader(ID3D11Device* device, ID3D11HullShader*& hShader, const std::string path);

bool LoadPixelShader(ID3D11Device* device, ID3D11PixelShader*& pShader, const std::string path);

bool LoadComputeShader(ID3D11Device* device, ID3D11ComputeShader*& cShader, const std::string path);

bool LoadGeometryShader(ID3D11Device* device, ID3D11GeometryShader*& cShader, const std::string path);