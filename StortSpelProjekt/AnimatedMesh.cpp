#include "AnimatedMesh.h"

void AnimatedMesh::uppdateMatrices(int animationIndex, float animationTime, const aiNode* node, const aiMatrix4x4& parentTrasform)
{
	aiAnimation an = this->animationData[animationIndex];
	aiMatrix4x4 transformMatrix = node->mTransformation;
}

AnimatedMesh::AnimatedMesh(ID3D11Device* device, std::vector<vertex> vertexTriangle, std::vector<DWORD> indexTriangle)
	:Mesh2(device, vertexTriangle, indexTriangle)
{
}

void AnimatedMesh::Draw(ID3D11DeviceContext* immediateContext, int animationIndex)
{
}
