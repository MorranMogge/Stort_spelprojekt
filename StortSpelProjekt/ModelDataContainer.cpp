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
	std::tuple<ID3D11Buffer*, ID3D11Buffer*> delTupel;
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

bool ModelDataContainer::getIndexMeshBuffers(const std::string key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff)
{
	this->meshIt = this->meshMap.find(key);
	if (this->meshIt == this->meshMap.end())
	{
		return false;
	}
	std::tuple tup = meshIt->second;
	indexBuff = std::get<tupelOrder::INDEXBUFFER>(tup);
	vertexBuff = std::get<tupelOrder::VERTEXBUFFER>(tup);
	return true;
}

void ModelDataContainer::addSrv(std::string key, ID3D11ShaderResourceView* srv)
{
	this->srvMap.insert(std::pair<std::string, ID3D11ShaderResourceView*>(key, srv));
}

void ModelDataContainer::addMeshBuffers(std::string key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf)
{
	std::tuple <ID3D11Buffer*, ID3D11Buffer*> newTup;
	newTup = std::make_tuple(indexBuf, vertexBuf);

	this->meshMap.insert(std::pair<std::string, std::tuple<ID3D11Buffer*, ID3D11Buffer*>>(key, newTup));;
}
