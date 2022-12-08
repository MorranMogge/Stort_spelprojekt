#include "stdafx.h"
#include "ModelDataContainer.h"

ModelDataContainer::ModelDataContainer()
{
	
}

ModelDataContainer::~ModelDataContainer()
{
	for (this->srvIt = this->srvMap.begin(); this->srvIt != this->srvMap.end(); this->srvIt++)
	{
		this->srvIt->second->Release();
	}
	std::tuple<ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>> delTupel;
	for (this->meshIt = this->meshMap.begin(); this->meshIt != this->meshMap.end(); this->meshIt++)
	{
		delTupel = this->meshIt->second;
		std::get<0>(delTupel)->Release();
		std::get<1>(delTupel)->Release();
	}
}

bool ModelDataContainer::hasItem(std::string key)
{
	this->srvIt = this->srvMap.find((const std::string) key);
	if (this->srvIt != this->srvMap.end())
	{
		return true;
	}

	this->meshIt = this->meshMap.find((const std::string)key);
	if (this->meshIt != this->meshMap.end())
	{
		return true;
	}
	return false;
}

ID3D11ShaderResourceView* ModelDataContainer::getSrv(const std::string key)
{
	this->srvIt = this->srvMap.find(key);
	if (this->srvIt == this->srvMap.end())
	{
		return nullptr;
	}
	return this->srvIt->second;
}

bool ModelDataContainer::getIndexMeshBuffers(const std::string key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces)
{
	this->meshIt = this->meshMap.find(key);
	if (this->meshIt == this->meshMap.end())
	{
		return false;
	}
	std::tuple tup = meshIt->second;
	indexBuff = std::get<tupelOrder::INDEXBUFFER>(tup);
	vertexBuff = std::get<tupelOrder::VERTEXBUFFER>(tup);
	subMeshRanges = std::get<tupelOrder::SUBMESHRANGES>(tup);
	amountOfVertces = std::get<tupelOrder::VERTICESRANGES>(tup);
	return true;
}

bool ModelDataContainer::getAnimData(const std::string& key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces, AnimationData& retAnimData)
{
	this->meshIt = this->meshMap.find(key);
	this->coreAnimDataIt = this->coreAnimData.find(key);
	if (this->meshIt == this->meshMap.end() || this->coreAnimDataIt == this->coreAnimData.end())
	{
		return false;
	}
	std::tuple tup = meshIt->second;
	indexBuff = std::get<tupelOrder::INDEXBUFFER>(tup);
	vertexBuff = std::get<tupelOrder::VERTEXBUFFER>(tup);
	subMeshRanges = std::get<tupelOrder::SUBMESHRANGES>(tup);
	amountOfVertces = std::get<tupelOrder::VERTICESRANGES>(tup);

	retAnimData = coreAnimDataIt->second;
	return true;
}

void ModelDataContainer::addSrv(std::string key, ID3D11ShaderResourceView* srv)
{
	this->srvMap.insert(std::pair<std::string, ID3D11ShaderResourceView*>(key, srv));
}

void ModelDataContainer::addMeshBuffers(std::string key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces)
{
	std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>> newTup;
	newTup = std::make_tuple(indexBuf, vertexBuf, subMeshRanges, amountOfVertces);

	this->meshMap.insert(std::pair<std::string, std::tuple<ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>>>(key, newTup));
}

void ModelDataContainer::addAnimationData(std::string key, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces, AnimationData animData)
{
	this->addMeshBuffers(key, vertexBuffer, indexBuf, subMeshRanges, amountOfVertces);
	this->coreAnimData.insert(std::pair<std::string, AnimationData>(key, animData));
}

void ModelDataContainer::extendAnimations(std::string key, AnimationData animData)
{
	this->coreAnimDataIt = this->coreAnimData.find(key);
	if (this->coreAnimDataIt != this->coreAnimData.end())
	{
		AnimationData OldData = coreAnimDataIt->second;
		for (int i = 0; i < animData.animation.size(); i++)
		{
			OldData.animation.push_back(animData.animation[i]);
		}

		coreAnimData.erase(key);
		this->coreAnimData.insert(std::pair<std::string, AnimationData>(key, OldData));
	}
}

void ModelDataContainer::addAllTexturesForMesh(std::string name, std::vector<std::string>& allNames)
{
	std::vector<ID3D11ShaderResourceView*> tempVec;
	for (int i = 0; i < allNames.size(); i++)
	{
		if (getSrv(allNames[i]) != nullptr)
		{
			tempVec.emplace_back(getSrv(allNames[i]));
		}
	}
	this->mapOfAllTexturesToAMesh.insert(std::pair(name, tempVec));
}

void ModelDataContainer::getAllTexturesForMesh(std::string name, std::vector<ID3D11ShaderResourceView*>& emptyVec)
{
	auto it = this->mapOfAllTexturesToAMesh.find(name);
	emptyVec = it->second;
}