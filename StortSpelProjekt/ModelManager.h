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

	std::vector<DWORD> oldIndexTriangle;
	std::vector<int> oldIndexTriangleSize;
	void checkIfSame();


	struct meshData dataForMesh;
	std::vector<int> submeshRanges;
	std::vector<ID3D11Buffer*> vecIndexBuff;
	bool makeSRV(ID3D11ShaderResourceView*& srv, const std::string& finalFilePath);
	void processNodes(aiNode* node, const aiScene* scene, const std::string& filePath);
	Mesh2* readNodes(aiMesh* mesh, const aiScene* scene);
	void readNodes2(aiMesh* mesh, const aiScene* scene);
	bool createVertexBuffer(ID3D11Buffer*& vertexBuffer, std::vector<vertex>& vertexTriangle);
	bool createIndexBuffer(ID3D11Buffer*& indexBuffer, std::vector<DWORD>& indexTriangle);
	bool createVertexBuffer(ID3D11Buffer*& vertexBuffer);
	bool createIndexBuffer(ID3D11Buffer*& indexBuffer);
	


	std::vector<Mesh2*> meshes;
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;

	ModelDataContainer bank;
	ID3D11Device* device;
public:
	ModelManager(ID3D11Device* device);
	~ModelManager();
	//checks if mesh already exists, if it does not already exist creates srv, vertexBuffer, indexBuffer
	bool loadMeshData(const std::string& filePath);
	std::vector<Mesh2*> getMeshes() const;
	//get the vector with texture maps
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;
	//fills the params with meshdata
	bool getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges);
	//debug function gets vectorbuff
	std::vector<ID3D11Buffer*> getBuff() const;

	bool getMeshData(const std::string& filePath, std::vector<Mesh2*>& mesh);

	std::vector<int> getOldIndexTriangleSize() const;
};