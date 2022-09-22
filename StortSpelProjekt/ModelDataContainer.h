#pragma once
#include<iostream>
#include<d3d11.h>
#include<DirectXMath.h>
#include<unordered_map>
#include<iterator>
#include<string>
#include<tuple>

#include "mesh2.h"

class ModelDataContainer
{
public:
	ModelDataContainer();
	~ModelDataContainer();

	bool hasItem(const std::string& key);
	ID3D11ShaderResourceView* getSrv(const std::string& key);
	bool getIndexMeshBuffers(const std::string& key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges);

	void addSrv(const std::string& key, ID3D11ShaderResourceView* srv);
	void addMeshBuffers(const std::string& key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges);
	void addMesh(const std::string& key, std::vector<Mesh2*>& meshToAdd);
	std::vector<Mesh2*> getMesh(const std::string& key);
	bool getMesh(const std::string& key, std::vector<Mesh2*>& meshToGet);
private:

	enum tupelOrder
	{
		INDEXBUFFER,
		VERTEXBUFFER,
		SUBMESHRANGES
	};

	std::unordered_map<std::string, ID3D11ShaderResourceView*> srvMap;
	std::unordered_map<std::string, ID3D11ShaderResourceView*>::iterator srvIt;
	
	
	std::unordered_map<std::string, std::vector<Mesh2*>> mapOfMesh;
	std::unordered_map<std::string, std::vector<Mesh2*>>::iterator itMapOfMesh;

	std::tuple <ID3D11Buffer*, ID3D11Buffer*> meshTupel;

	std::unordered_map<std::string, std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>>> meshMap;
	std::unordered_map<std::string, std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>>>::iterator meshIt;
	

};