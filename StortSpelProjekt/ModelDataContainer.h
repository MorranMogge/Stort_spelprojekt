#pragma once
#include<iostream>
#include<d3d11.h>
#include<DirectXMath.h>
#include<unordered_map>
#include<iterator>
#include<string>
#include<tuple>

class ModelDataContainer
{
public:
	ModelDataContainer();
	~ModelDataContainer();

	bool hasItem(const std::string key);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	bool getIndexMeshBuffers(const std::string key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff);

	void addSrv(std::string key, ID3D11ShaderResourceView* srv);
	void addMeshBuffers(std::string key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf);
private:

	enum tupelOrder
	{
		INDEXBUFFER,
		VERTEXBUFFER
	};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> srvMap;
	std::unordered_map<std::string, ID3D11ShaderResourceView*>::iterator srvIt;

	std::tuple <ID3D11Buffer*, ID3D11Buffer*> meshTupel;

	std::unordered_map<std::string, std::tuple <ID3D11Buffer*, ID3D11Buffer*>> meshMap;
	std::unordered_map<std::string, std::tuple <ID3D11Buffer*, ID3D11Buffer*>>::iterator meshIt;
};