#pragma once
#include<iostream>
#include<d3d11.h>
#include<DirectXMath.h>
#include<unordered_map>
#include<iterator>
#include<string>
#include<tuple>
#include"AnimatedMesh.h"

class ModelDataContainer
{
public:
	ModelDataContainer();
	~ModelDataContainer();

	bool hasItem(const std::string key);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	bool getIndexMeshBuffers(const std::string key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces);
	bool getAnimData(const std::string& key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces, AnimationData& retAnimData);

	void addSrv(std::string key, ID3D11ShaderResourceView* srv);
	void addMeshBuffers(std::string key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces);
	void addAnimationData(std::string key, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces, AnimationData animData);
	void extendAnimations(std::string key, AnimationData animData);
private:

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
	
	std::unordered_map<std::string, AnimationData> coreAnimData;
	std::unordered_map<std::string, AnimationData>::iterator coreAnimDataIt;
};