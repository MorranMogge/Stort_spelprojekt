#pragma once
#include "ErrorLog.h"
#include<iostream>
#include<d3d11.h>
#include<DirectXMath.h>
#include<unordered_map>
#include<iterator>
#include<string>
#include<tuple>
#include "ConstantBuffer.h"



class ModelDataContainer
{
public:
	ModelDataContainer();
	~ModelDataContainer();

	bool hasItem(const std::string key);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	bool getIndexMeshBuffers(const std::string key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces);
	void addMaterial(const std::string &key, ConstantBuffer* material);
	bool hasMaterial(const std::string& key);
	void addSrv(std::string key, ID3D11ShaderResourceView* srv);
	void addMeshBuffers(std::string key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces);
	ConstantBuffer* getMaterial(const std::string& key, ConstantBuffer*& constantBuff);
private:

	struct MaterialS
	{
		DirectX::SimpleMath::Vector4 ambient{ 0.0f, 0.0f, 0.0f, 0.0f };
		DirectX::SimpleMath::Vector4 diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::SimpleMath::Vector3 specular{ 0.0f, 0.0f, 0.0f };
		float specularPower = 16.0f;
	};

	enum tupelOrder
	{
		INDEXBUFFER,
		VERTEXBUFFER,
		SUBMESHRANGES,
		VERTICESRANGES
	};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> srvMap;
	std::unordered_map<std::string, ID3D11ShaderResourceView*>::iterator srvIt;

	std::tuple <ID3D11Buffer*, ID3D11Buffer*> meshTupel;

	std::unordered_map<std::string, std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>>> meshMap;
	std::unordered_map<std::string, std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>>>::iterator meshIt;
	std::unordered_map<std::string, ConstantBuffer*> constantBuffMap;
	
};