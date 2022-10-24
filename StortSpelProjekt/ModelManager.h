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
#include <d3d11.h>
#include <SimpleMath.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "AnimatedMesh.h"
#include "mesh2.h"
#include <assimp/cimport.h>
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
	void readNodes(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh2*> meshes;
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;


	ModelDataContainer bank;
	ID3D11Device* device;

	//bones 
	
	aiMatrix4x4 globalInverseTransform;
	AnimationData aniData;

	void aiMatrixToXMmatrix(const aiMatrix4x4& in, DirectX::XMFLOAT4X4& out);
	void normalizeWeights(float weights[]);
	void addBoneData(const int vertexID, const int boneId, const float weight);
	void loadBones(const aiMesh* mesh, const int mesh_index);
	void numberBone(aiNode* node, int parentNode, const DirectX::XMFLOAT4X4& prevOffsets);
	int findAndAddBoneID(const std::string& name);
	int findBoneID(const std::string& name);
	void recParseNodes(nodes* node, const aiNode* ainode);
	void parseNode(const aiScene* scene);
	void parseAnimation(const aiScene* scene);

public:
	ModelManager();
	ModelManager(ID3D11Device* device);
	~ModelManager();
	bool loadMeshData(const std::string& filePath);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	//std::vector<Mesh2*> getMeshes() const;
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;

	bool loadMeshAndBoneData(const std::string& filePath, AnimatedMesh& gameObject);
	//fills the params with meshdata
	bool getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces);
	std::vector<ID3D11Buffer*> getBuff() const;
	void setDevice(ID3D11Device* device);
};