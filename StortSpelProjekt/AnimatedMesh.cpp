#include "AnimatedMesh.h"

void AnimatedMesh::uppdateMatrices(int animationIndex, float animationTime, const aiNode* node, const DirectX::XMMATRIX& parentTrasform)
{
	aiAnimation* an = this->scene->mAnimations[animationIndex];
	DirectX::XMMATRIX meshToBoneTransform = XMMatrixTranspose(DirectX::XMMATRIX(
			node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
			node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
			node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
			node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4));

	const aiNodeAnim* amnNode = this->findNodeAnim(node->mName.data, an);

	if (amnNode)
	{
		aiVector3D Scaling;
		this->InterpolateScaling(Scaling, animationTime, amnNode);
		DirectX::XMMATRIX ScalingM = DirectX::XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);

		aiQuaternion r;
		this->InterpolateRotation(r, animationTime, amnNode);
		DirectX::XMMATRIX RotationM = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(r.x, r.y, r.z, r.w));

		aiVector3D Translation;
		this->InterpolateTranslation(Translation, animationTime, amnNode);
		DirectX::XMMATRIX TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

		meshToBoneTransform = TranslationM * RotationM * ScalingM;
	}
	DirectX::XMMATRIX globalTrasform = parentTrasform * meshToBoneTransform;

	if (boneNameToIndex.find(node->mName.data) != boneNameToIndex.end())
	{
		int id = boneNameToIndex[node->mName.data];
		boneVector[id].offsetMatrix = globalTrasform;
	}
	for (int i = 0, end = node->mNumChildren; i < end; i++)
	{
		uppdateMatrices(animationIndex, animationTime, node->mChildren[i], globalTrasform);
	}
}

void AnimatedMesh::findlowRotationNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm)
{
	for (int i = 0, end = nodeAnm->mNumRotationKeys-1; i < end; i++)
	{
		if (AnimationTimeTicks < nodeAnm->mRotationKeys[i + 1].mTime)
		{
			out = i;
			return;
		}
	}
	out = 0;
	return;
}

void AnimatedMesh::InterpolateRotation(aiQuaternion& res, float animationTime, const aiNodeAnim* animationNode)
{
	if (animationNode->mNumRotationKeys == 1)
	{
		res = animationNode->mRotationKeys[0].mValue;
		return;
	}
	int lowRotaKey = 0;
	this->findlowRotationNode(lowRotaKey, animationTime, animationNode);
	int nextRotation = lowRotaKey + 1;
	float t1 = animationNode->mRotationKeys[lowRotaKey].mTime;
	float t2 = animationNode->mRotationKeys[nextRotation].mTime;

	float factor = (animationTime - t1) / (t2 - t1);
	const aiQuaternion& start = animationNode->mRotationKeys[lowRotaKey].mValue;
	const aiQuaternion& end = animationNode->mRotationKeys[nextRotation].mValue;
	aiQuaternion::Interpolate(res, start, end, factor);
	res = res.Normalize();
}

void AnimatedMesh::findlowScaleNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm)
{
	for (int i = 0, end = nodeAnm->mNumScalingKeys - 1; i < end; i++)
	{
		if (AnimationTimeTicks < nodeAnm->mScalingKeys[i + 1].mTime)
		{
			out = i;
			return;
		}
	}
	out = 0;
	return;
}

void AnimatedMesh::InterpolateScaling(aiVector3D& res, float animationTime, const aiNodeAnim* animationNode)
{
	if (animationNode->mNumScalingKeys == 1)
	{
		res = animationNode->mScalingKeys[0].mValue;
		return;
	}
	int lowRotaKey = 0;
	this->findlowRotationNode(lowRotaKey, animationTime, animationNode);
	int nextRotation = lowRotaKey + 1;
	float t1 = animationNode->mScalingKeys[lowRotaKey].mTime;
	float t2 = animationNode->mScalingKeys[nextRotation].mTime;

	float factor = (animationTime - t1) / (t2 - t1);
	const aiVector3D& start = animationNode->mScalingKeys[lowRotaKey].mValue;
	const aiVector3D& end = animationNode->mScalingKeys[nextRotation].mValue;
	res = start + factor * (end-start);
}

void AnimatedMesh::findlowTransNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm)
{
	for (int i = 0, end = nodeAnm->mNumPositionKeys - 1; i < end; i++)
	{
		if (AnimationTimeTicks < nodeAnm->mPositionKeys[i + 1].mTime)
		{
			out = i;
			return;
		}
	}
	out = 0;
	return;
}

void AnimatedMesh::InterpolateTranslation(aiVector3D& res, float animationTime, const aiNodeAnim* animationNode)
{
	if (animationNode->mNumScalingKeys == 1)
	{
		res = animationNode->mPositionKeys[0].mValue;
		return;
	}
	int lowRotaKey = 0;
	this->findlowRotationNode(lowRotaKey, animationTime, animationNode);
	int nextRotation = lowRotaKey + 1;
	float t1 = animationNode->mPositionKeys[lowRotaKey].mTime;
	float t2 = animationNode->mPositionKeys[nextRotation].mTime;

	float factor = (animationTime - t1) / (t2 - t1);
	const aiVector3D& start = animationNode->mPositionKeys[lowRotaKey].mValue;
	const aiVector3D& end = animationNode->mPositionKeys[nextRotation].mValue;
	res = start + factor * (end - start);
}

const aiNodeAnim* AnimatedMesh::findNodeAnim(const std::string& nodeName, const aiAnimation* pAnimation)
{
	for (int i = 0, end = pAnimation->mNumChannels; i < end; i++)
	{
		if (pAnimation->mChannels[i]->mNodeName.data == nodeName)
		{
			return pAnimation->mChannels[i];
		}
	}
	return NULL;
}

void AnimatedMesh::getTimeInTicks(float dt)
{
	float tps = scene->mAnimations[0]->mTicksPerSecond;
	if (tps == 0)
	{
		tps = 25.f;
	}
	float timeInTicks = dt * tps;
	if (timeInTicks >= scene->mAnimations[0]->mDuration)
	{
		timeInTicks -= scene->mAnimations[0]->mDuration;
	}
	DirectX::XMMATRIX startMatrix = DirectX::XMMatrixIdentity();
	this->uppdateMatrices(0, timeInTicks, this->scene->mRootNode, startMatrix);
}

void AnimatedMesh::render(ID3D11DeviceContext* devCon, float dt)
{

}

AnimatedMesh::AnimatedMesh(ID3D11Device* device, std::vector<vertex> vertexTriangle, std::vector<DWORD> indexTriangle)
	:Mesh2(device, vertexTriangle, indexTriangle)
{
}

void AnimatedMesh::Draw(ID3D11DeviceContext* immediateContext, int animationIndex)
{
}