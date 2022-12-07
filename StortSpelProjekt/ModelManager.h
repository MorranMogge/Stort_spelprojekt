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
#include <DirectXMath.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "AnimatedMesh.h"
//#include "mesh2.h"
#include <assimp/cimport.h>
//#include "../DirectXTK-main/Src/SDKMesh.h"

struct AnimatedVertex
{
	DirectX::XMFLOAT3 pos; // Position
	DirectX::XMFLOAT3 nor; // Normal
	DirectX::XMFLOAT2 uv; // UV coordination

	DirectX::XMINT4 boneId;
	DirectX::XMFLOAT4 weights;

	AnimatedVertex() {
		pos = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		uv = DirectX::XMFLOAT2{ 0.0f,0.0f };
		nor = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };

		boneId = DirectX::XMINT4{ 0, 0, 0, 0 };
		weights = DirectX::XMFLOAT4{ 0.0f,0.0f,0.0f,0.0f };
	};
	AnimatedVertex(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& uv, DirectX::XMFLOAT3& nor, DirectX::XMINT4& boneId,
		DirectX::XMFLOAT4& weight) : pos(pos), uv(uv), nor(nor), boneId(boneId), weights(weight) {};
};

struct vertex
{
	DirectX::XMFLOAT3 pos; // Position
	DirectX::XMFLOAT3 nor; // Normal
	DirectX::XMFLOAT2 uv; // UV coordination
	DirectX::XMFLOAT3 tangent; // Normal

	vertex() {
		pos = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		uv = DirectX::XMFLOAT2{ 0.0f,0.0f };
		nor = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		tangent = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
	};
	vertex(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& uv, DirectX::XMFLOAT3& nor, DirectX::XMFLOAT3& tangent) : pos(pos), uv(uv), nor(nor), tangent(tangent) {};
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
	void processNodes(aiNode* node, const aiScene* scene, const std::string& filePath, bool wantToBone = false);
	void readNodes(aiMesh* mesh, const aiScene* scene);
	//std::vector<Mesh2*> meshes;
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;
	std::vector<std::string> allTextureNamesForMesh;

	ModelDataContainer bank;
	ID3D11Device* device;

	//bones 
	
	AnimationData aniData;
	unsigned VertexOffset;

	void aiMatrixToXMmatrix(const aiMatrix4x4& in, DirectX::XMFLOAT4X4& out);
	void normalizeWeights(float weights[]);
	void addBoneData(const int vertexID, const int boneId, const float weight);
	void loadBones(const aiMesh* mesh, const int vertexOffset);
	int findAndAddBoneID(const std::string& name);
	int findBoneID(const std::string& name);
	void recParseNodes(nodes& node, const aiNode* ainode);
	void parseNode(const aiScene* scene);
	void parseAnimation(const aiScene* scene);
	

public:
	ModelManager();
	ModelManager(ID3D11Device* device);
	~ModelManager();
	bool loadMeshData(const std::string& filePath);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	//std::vector<Mesh2*> getMeshes() const;
	void getTextureMaps(std::string name, std::vector<ID3D11ShaderResourceView*>& emptyVec);
	bool addTexture(const std::string& filePath);
	bool loadMeshAndBoneData(const std::string& filePath);
	bool AdditionalAnimation(const std::string& newAnimationFile, const std::string& destination);
	//fills the params with meshdata
	bool getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces);
	bool getAnimData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces, AnimationData& AnimData);
	std::vector<ID3D11Buffer*> getBuff() const;
	void setDevice(ID3D11Device* device);
	
};