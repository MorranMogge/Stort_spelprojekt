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
	bool makeSRV(ID3D11ShaderResourceView*& srv, std::string finalFilePath);
	void processNodes(aiNode* node, const aiScene* scene);
	Mesh2* readNodes(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh2*> meshes;
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;

	ModelDataContainer bank;
	ID3D11Device* device;
public:
	ModelManager(ID3D11Device* device);
	bool loadMeshData(const std::string& filePath);
	std::vector<Mesh2*> getMeshes() const;
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;
};