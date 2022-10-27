#pragma once
#define NOMINMAX
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>
#include <vector>
#include <string>
#include "ErrorLog.h"
#include "ModelDataContainer.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//#include "../DirectXTK-main/Src/SDKMesh.h"

struct vertex
{
	DirectX::XMFLOAT3 pos; // Position
	DirectX::XMFLOAT2 uv; // UV coordination
	DirectX::XMFLOAT3 nor; // Normal
	/* DirectX::XMFLOAT3 tangent;*/

	vertex() {
		pos = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		uv = DirectX::XMFLOAT2{ 0.0f,0.0f };
		nor = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		/*tangent = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };*/
	};
	vertex(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& uv, DirectX::XMFLOAT3& nor, DirectX::XMFLOAT3& tangent) : pos(pos), uv(uv), nor(nor)/*, tangent(tangent)*/ {};
};


class ModelManager
{
private:

	struct meshData
	{
		std::vector<vertex> vertexTriangle;
		std::vector<DWORD> indexTriangle;
	};

	
	struct meshData dataForMesh;
	std::vector<int> submeshRanges;
	std::vector<int> amountOfvertices;
	std::vector<ID3D11Buffer*> vecIndexBuff;
	bool makeSRV(ID3D11ShaderResourceView*& srv, std::string finalFilePath);
	void processNodes(aiNode* node, const aiScene* scene, const std::string& filePath);
	void readNodes(aiMesh* mesh, const aiScene* scene);
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;


	ModelDataContainer bank;
	ID3D11Device* device;
public:
	ModelManager();
	ModelManager(ID3D11Device* device);
	~ModelManager();
	bool loadMeshData(const std::string& filePath);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	//std::vector<Mesh2*> getMeshes() const;
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;
	//fills the params with meshdata
	bool getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces);
	std::vector<ID3D11Buffer*> getBuff() const;
	void setDevice(ID3D11Device* device);
};