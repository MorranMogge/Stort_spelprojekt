#pragma once
#include<iostream>
#include<d3d11.h>
#include<DirectXMath.h>
#include<map>
#include<iterator>
#include<string>

class MDC
{
public:
	MDC();
	~MDC();

	bool hasItem(const std::string key);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	ID3D11Buffer* getVertexBuffer(const std::string key);
	ID3D11Buffer* getIndexBuffer(const std::string key);

	void addSrv(std::string key, ID3D11ShaderResourceView* srv);
	void addVertexBuffer(std::string key, ID3D11Buffer* vertexBuf);
	void addIndexBuffer(std::string key, ID3D11Buffer* indexBuf);

private:

	std::map<std::string, ID3D11ShaderResourceView*> srvMap;
	std::map<std::string, ID3D11ShaderResourceView*>::iterator srvIt;

	std::map<std::string, ID3D11Buffer*> vertexMap;
	std::map<std::string, ID3D11Buffer*> indexMap;
	std::map<std::string, ID3D11Buffer*>::iterator buffIt;


};