#pragma once
#define NOMINMAX
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>
#include <vector>
#include <string>
#include "ErrorLog.h"

#include "mesh2.h"
//#include "../DirectXTK-main/Src/SDKMesh.h"


class ModelManager
{
private:
	void processNodes(aiNode* node, const aiScene* scene);
	Mesh2* readNodes(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh2*> meshes;
public:
	ModelManager();
	bool loadMeshData(const std::string& filePath);
	std::vector<Mesh2*> getMeshes() const;
};