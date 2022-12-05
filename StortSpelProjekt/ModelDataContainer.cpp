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
	//std::unordered_map<std::string, ConstantBuffer*>::iterator constantBuffIt;
	//for (constantBuffIt = this->constantBuffMap.begin(); constantBuffIt != this->constantBuffMap.end(); constantBuffIt)
	//{
	//	delete constantBuffIt->second;
	//
	//}
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

ConstantBuffer ModelDataContainer::getMaterial(const std::string& key)
{
	std::unordered_map<std::string, ConstantBuffer>::iterator constantBuffIt;
	constantBuffIt = constantBuffMap.find(key);
	
	if (constantBuffIt != this->constantBuffMap.end())
	{
		return constantBuffIt->second;
	}

}

void ModelDataContainer::addSrv(std::string key, ID3D11ShaderResourceView* srv)
{
	this->srvMap.insert(std::pair<std::string, ID3D11ShaderResourceView*>(key, srv));
}

void ModelDataContainer::addMaterial(const std::string& key, ConstantBuffer& buffer)
{
	this->constantBuffMap.insert(std::pair<std::string, ConstantBuffer>(key, buffer));
}

bool ModelDataContainer::hasMaterial(const std::string& key)
{
	std::unordered_map<std::string, ConstantBuffer>::iterator constantBuffIt;

	constantBuffIt = this->constantBuffMap.find((const std::string)key);
	if (constantBuffIt != this->constantBuffMap.end())
	{
		std::cout << "match in unordered map key: " << key << "\n";
		return true;
	}

	return false;
}

void ModelDataContainer::addMeshBuffers(std::string key, ID3D11Buffer* vertexBuf, ID3D11Buffer* indexBuf, std::vector<int>& subMeshRanges, std::vector<int>& amountOfVertces)
{
	std::tuple <ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>> newTup;
	newTup = std::make_tuple(indexBuf, vertexBuf, subMeshRanges, amountOfVertces);

	this->meshMap.insert(std::pair<std::string, std::tuple<ID3D11Buffer*, ID3D11Buffer*, std::vector<int>, std::vector<int>>>(key, newTup));
}
