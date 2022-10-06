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

#include "mesh2.h"
//#include "../DirectXTK-main/Src/SDKMesh.h"


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
	//Mesh2* readNodes(aiMesh* mesh, const aiScene* scene);
	void readNodes(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh2*> meshes;
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;

	ModelDataContainer bank;
	ID3D11Device* device;
public:
	ModelManager();
	ModelManager(ID3D11Device* device);
	~ModelManager();
	bool loadMeshData(const std::string& filePath);
	std::vector<Mesh2*> getMeshes() const;
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;
	//fills the params with meshdata
	bool getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces);
	std::vector<ID3D11Buffer*> getBuff() const;
	void setDevice(ID3D11Device* device);
};