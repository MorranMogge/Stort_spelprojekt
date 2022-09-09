#include "ModelDataContainer.h"

MDC::MDC()
{
}

MDC::~MDC()
{
	for (this->srvIt = this->srvMap.begin(); this->srvIt != this->srvMap.end(); this->srvIt++)
	{
		this->srvIt->second->Release();
	}

	for (this->buffIt = this->vertexMap.begin(); this->buffIt != this->vertexMap.end(); this->buffIt++)
	{
		this->buffIt->second->Release();
	}

	for (this->buffIt = this->indexMap.begin(); this->buffIt != this->indexMap.end(); this->buffIt++)
	{
		this->buffIt->second->Release();
	}
}

bool MDC::hasItem(std::string key)
{
	this->srvIt = this->srvMap.find((const std::string) key);
	if (this->srvIt != this->srvMap.end())
	{
		return true;
	}

	this->buffIt = this->vertexMap.find((const std::string)key);
	if (this->buffIt != this->vertexMap.end())
	{
		return true;
	}
	return false;
}

ID3D11ShaderResourceView* MDC::getSrv(const std::string key)
{
	this->srvIt = this->srvMap.find(key);
	if (this->srvIt == this->srvMap.end())
	{
		return nullptr;
	}
	return this->srvIt->second;
}

ID3D11Buffer* MDC::getVertexBuffer(const std::string key)
{
	this->buffIt = this->vertexMap.find(key);
	if (this->buffIt == this->vertexMap.end())
	{
		return nullptr;
	}
	return this->buffIt->second;
}

ID3D11Buffer* MDC::getIndexBuffer(const std::string key)
{
	this->buffIt = this->indexMap.find(key);
	if (this->buffIt == this->indexMap.end())
	{
		return nullptr;
	}
	return this->buffIt->second;
}

void MDC::addSrv(std::string key, ID3D11ShaderResourceView* srv)
{
	this->srvMap.insert(std::pair<std::string, ID3D11ShaderResourceView*>(key, srv));
}

void MDC::addVertexBuffer(std::string key, ID3D11Buffer* vertexBuf)
{
	this->vertexMap.insert(std::pair<std::string, ID3D11Buffer*>(key, vertexBuf));
}

void MDC::addIndexBuffer(std::string key, ID3D11Buffer* indexBuf)
{
	this->indexMap.insert(std::pair<std::string, ID3D11Buffer*>(key, indexBuf));
}