#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include "mesh2.h"

class AnimatedMesh : public Mesh2
{
private:

	aiNode rootNode;
	std::vector<aiAnimation> animationData;
	aiScene* scene;

	std::unordered_map<std::string, int> boneNameToIndex;
	struct boneInfo
	{
		int parentID;
		std::string name;
		DirectX::XMMATRIX offsetMatrix;

		boneInfo(const DirectX::XMMATRIX& offset)
		{
			offsetMatrix = offset;
			parentID = -1;
			name = "";
		}
	};
	std::vector<boneInfo> boneVector;

	void uppdateMatrices(int animationIndex, float animationTime, const aiNode* node, const DirectX::XMMATRIX& parentTrasform);

	void findlowRotationNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm);
	void InterpolateRotation(aiQuaternion& res, float animationTime, const aiNodeAnim* animationNode);

	void findlowScaleNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm);
	void InterpolateScaling(aiVector3D& res, float animationTime, const aiNodeAnim* animationNode);

	void findlowTransNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm);
	void InterpolateTranslation(aiVector3D& res, float animationTime, const aiNodeAnim* animationNode);

	const aiNodeAnim* findNodeAnim(const std::string& nodeName, const aiAnimation* pAnimation);

	void getTimeInTicks(float dt);
	void render(ID3D11DeviceContext* devCon, float dt);
public:
	AnimatedMesh(ID3D11Device* device, std::vector<vertex> vertexTriangle, std::vector<DWORD> indexTriangle);

	void Draw(ID3D11DeviceContext* immediateContext, int animationIndex);
};