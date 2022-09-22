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
	std::tuple<ID3D11Buffer*, ID3D11Buffer*, std::vector<int>> delTupel;
	for (this->meshIt = this->meshMap.begin(); this->meshIt != this->meshMap.end(); this->meshIt++)
	{
		delTupel = this->meshIt->second;
		std::get<0>(delTupel)->Release();
		std::get<1>(delTupel)->Release();
	}
	/*for (this->itMapOfMesh = this->mapOfMesh.begin(); this->itMapOfMesh != this->mapOfMesh.end(); this->itMapOfMesh++)
	{
		for (int i = 0; i < this->itMapOfMesh->second.size(); i++)
		{
			delete this->itMapOfMesh->second[i];
		}
	}*/
}

bool ModelDataContainer::hasItem(const std::string& key)
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

ID3D11ShaderResourceView* ModelDataContainer::getSrv(const std::string& key)
{
	this->srvIt = this->srvMap.find(key);
	if (this->srvIt == this->srvMap.end())
	{
		return nullptr;
	}
	return this->srvIt->second;
}

bool ModelDataContainer::getIndexMeshBuffers(const std::string& key, ID3D11Buffer*& indexBuff, ID3D11Buffer*& vertexBuff, std::vector<int>& subMeshRanges)
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
	return true;
}

void ModelDataContainer::addSrv(const std::string& key, ID3D11ShaderResourceView* srv)
{
	this->srvMap.insert(std::pair<std::string, ID3D11ShaderResourceView*>(key, srv));
}

void ModelDataContainer::addMeshBuffers(const std::string& key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges)
{
	std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>> newTup;
	newTup = std::make_tuple(indexBuf, vertexBuf, subMeshRanges);

	this->meshMap.insert(std::pair<std::string, std::tuple<ID3D11Buffer*, ID3D11Buffer*, std::vector<int>>>(key, newTup));
}

void ModelDataContainer::addMesh(const std::string& key, std::vector<Mesh2*>& meshToAdd)
{
	this->mapOfMesh.insert(std::pair<std::string, std::vector<Mesh2*>>(key, meshToAdd));
}

std::vector<Mesh2*> ModelDataContainer::getMesh(const std::string& key)
{
	this->itMapOfMesh = this->mapOfMesh.find(key);
	if (this->itMapOfMesh == this->mapOfMesh.end())
	{
		return std::vector<Mesh2*>();
	}
	return this->itMapOfMesh->second;
}

bool ModelDataContainer::getMesh(const std::string& key, std::vector<Mesh2*>& meshToGet)
{
	this->itMapOfMesh = this->mapOfMesh.find(key);
	if (this->itMapOfMesh == this->mapOfMesh.end())
	{
		return false;
	}
	meshToGet = this->itMapOfMesh->second;
	return true;
}