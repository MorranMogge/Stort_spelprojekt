#include "AnimatedMesh.h"
using namespace DirectX;

void AnimatedMesh::uppdateMatrices(int animationIndex, float animationTime, const aiNode* node, DirectX::XMFLOAT4X4& parentTrasform)
{
	aiAnimation* an = this->scene->mAnimations[animationIndex];
	DirectX::XMMATRIX currentNodeTrans = XMMatrixTranspose(DirectX::XMMATRIX(
			node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
			node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
			node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
			node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4));


	const aiNodeAnim* amnNode = this->findNodeAnim(node->mName.data, an);

	//MySimp.animationData.mChannels = scene->mAnimations[0]->mChannels;

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

		currentNodeTrans = TranslationM * RotationM * ScalingM;
	}
	DirectX::XMMATRIX PrevNode = DirectX::XMLoadFloat4x4(&parentTrasform);

	DirectX::XMMATRIX globalTrasform = PrevNode * currentNodeTrans;
	DirectX::XMFLOAT4X4 meshToBoneTransform;
	DirectX::XMStoreFloat4x4(&meshToBoneTransform, globalTrasform);


	//if (boneNameToIndex.find(node->mName.data) != boneNameToIndex.end())
	//{
	//	int id = boneNameToIndex[node->mName.data];
	//	boneVector[id].offsetMatrix = meshToBoneTransform;
	//}
	for (int i = 0, end = node->mNumChildren; i < end; i++)
	{
		uppdateMatrices(animationIndex, animationTime, node->mChildren[i], meshToBoneTransform);
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
	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 startMatrix;
	DirectX::XMStoreFloat4x4(&startMatrix, temp);
	this->uppdateMatrices(0, timeInTicks, this->scene->mRootNode, startMatrix);
}

AnimatedMesh::AnimatedMesh(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:GameObject(useMesh, pos, rot, id)
{
	InputLayoutAndVertexShader();
}

AnimatedMesh::AnimatedMesh()
{
}

AnimatedMesh::~AnimatedMesh()
{
	free(this->scene->mAnimations);
	free(this->scene->mRootNode);
	free(this->scene);
}

void AnimatedMesh::addData(const AnimationData& data)
{
	this->MySimp = data;

	//std::vector<DirectX::XMFLOAT4X4> structVector(boneVector.size());
	//this->boneStrucBuf.Initialize(GPU::device, GPU::immediateContext, structVector);
}

void AnimatedMesh::uppdate(ID3D11DeviceContext* immediateContext, int animationIndex, const float dt)
{
	//DirectX::XMMATRIX start = DirectX::XMMatrixIdentity();
	//this->getTimeInTicks(dt);

	//this->boneVector;
	//int bp = 2;

	//----------------------------------------

	//for (int i = 0, end = this->boneVector.size(); i < end; i++)
	//{
	//	DirectX::XMStoreFloat4x4(&this->boneStrucBuf.getData(i), boneVector[i].offsetMatrix);
	//}
	//this->boneStrucBuf.applyData();
	//this->boneStrucBuf.BindToVS(0);
}