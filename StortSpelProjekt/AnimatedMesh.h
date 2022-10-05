#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <iostream>
#include "mesh2.h"

class AnimatedMesh : public Mesh2
{
private:

	aiNode rootNode;
	std::vector<aiAnimation> animationData;

	void uppdateMatrices(int animationIndex, float animationTime, const aiNode* node, const aiMatrix4x4& parentTrasform);
public:
	AnimatedMesh(ID3D11Device* device, std::vector<vertex> vertexTriangle, std::vector<DWORD> indexTriangle);

	void Draw(ID3D11DeviceContext* immediateContext, int animationIndex);
};