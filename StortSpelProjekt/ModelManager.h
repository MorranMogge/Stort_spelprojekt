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

#include "mesh2.h"
//#include "../DirectXTK-main/Src/SDKMesh.h"


class ModelManager
{
private:
	bool makeSRV(ID3D11ShaderResourceView*& srv, std::string finalFilePath);
	void processNodes(aiNode* node, const aiScene*& scene);
	Mesh2* readNodes(aiMesh* mesh, const aiScene*& scene);
	std::vector<Mesh2*> meshes;
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;

	ModelDataContainer bank;
	ID3D11Device* device;

	aiMatrix4x4 globalInverseTransform;
	struct IndexBoneData
	{
		//int boneName;
		int BoneIDs[4] = {-1};
		float Weights[4] = {0};
	};
	struct boneInfo
	{
		int parentID;
		std::string name;
		DirectX::XMMATRIX offsetMatrix;
		DirectX::XMMATRIX finalTransform;

		boneInfo(const DirectX::XMMATRIX& offset)
		{
			offsetMatrix = offset;
			finalTransform = {};
			parentID = -1;
			name = "";
		}
	};
	void aiMatrixToXMmatrix(const aiMatrix4x4& in, DirectX::XMMATRIX& out);
	std::vector<boneInfo> boneVec;
	std::vector<IndexBoneData> boneDataVec;
	void normalizeWeights(float weights[]);
	void addBoneData(const int vertexID, const int boneId, const float weight);
	void loadBones(const aiMesh* mesh, const int mesh_index);
	std::map<std::string, int> boneIndexTraslator;
	std::map<std::string, int>::iterator boneIndexTraslatorIT;
	int numberBones;
	void numberBone(aiNode* node, int parentNode, DirectX::XMMATRIX& prevOffsets);
	int findAndAddBoneID(std::string name);
	int findBoneID(const std::string& name);

	void readAnimations(aiScene* scene);
public:
	ModelManager(ID3D11Device* device);
	bool loadMeshData(const std::string& filePath);
	std::vector<Mesh2*> getMeshes() const;
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;
};