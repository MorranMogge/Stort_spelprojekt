#include "stdafx.h"
#include "AnimatedMesh.h"
#include "ModelManager.h"
#include <assimp/cimport.h>

/*
                               ______________                               
                        ,===:'.,            `-._                           
Art by                       `:.`---.__         `-._                       
 John VanderZwaag              `:.     `--.         `.                     
                                 \.        `.         `.                   
                         (,,(,    \.         `.   ____,-`.,                
                      (,'     `/   \.   ,--.___`.'                         
                  ,  ,'  ,--.  `,   \.;'         `                         
                   `{D, {    \  :    \;                                    
                     V,,'    /  /    //                                    
                     j;;    /  ,' ,-//.    ,---.      ,                    
                     \;'   /  ,' /  _  \  /  _  \   ,'/                    
                           \   `'  / \  `'  / \  `.' /                     
                            `.___,'   `.__,'   `.__,'  

Be aware, traveler. Upp here there be dragons
*/
void AnimatedMesh::uppdateMatrices(int animationIndex, float animationTime, const nodes& node, DirectX::XMFLOAT4X4& parentTrasform)
{
	DirectX::XMFLOAT4X4 finalTransfrom;

	if (MySimp.boneNameToIndex.find(node.nodeName) != MySimp.boneNameToIndex.end())
	{
		animationNode& an = this->MySimp.animation[animationIndex];
		animationNode& basic = this->MySimp.animation[0];

		DirectX::XMMATRIX currentNodeTrans = DirectX::XMLoadFloat4x4(&node.trasformation);
		channels amnNode;
		channels originalPose;

		if (this->findNodeAnim(node.nodeName, an, amnNode))
		{
			this->findNodeAnim(node.nodeName, basic, originalPose);

			DirectX::XMFLOAT3 Scaling;
			this->InterpolateScaling(Scaling, animationTime, amnNode, originalPose);
			DirectX::XMMATRIX ScalingM = DirectX::XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);

			DirectX::XMFLOAT4 r;
			this->InterpolateRotation(r, animationTime, amnNode, originalPose);
			DirectX::XMMATRIX RotationM = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(r.x, r.y, r.z, r.w));

			DirectX::XMFLOAT3 Translation;
			this->InterpolatePos(Translation, animationTime, amnNode, originalPose);
			DirectX::XMMATRIX TranslationM = DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

			currentNodeTrans = DirectX::XMMatrixIdentity();
			currentNodeTrans = DirectX::XMMatrixMultiply(currentNodeTrans, ScalingM);
			currentNodeTrans = DirectX::XMMatrixMultiply(currentNodeTrans, RotationM);
			currentNodeTrans = DirectX::XMMatrixMultiply(currentNodeTrans, TranslationM);
		}

		DirectX::XMMATRIX PrevNode = DirectX::XMLoadFloat4x4(&parentTrasform);
		const DirectX::XMMATRIX globalTrasform = DirectX::XMMatrixMultiply(currentNodeTrans, PrevNode);

		int id = MySimp.boneNameToIndex[node.nodeName];

		DirectX::XMMATRIX boneOffset = DirectX::XMLoadFloat4x4(&MySimp.boneVector[id].offsetMatrix);
		DirectX::XMMATRIX finalMesh = DirectX::XMMatrixMultiply(boneOffset, globalTrasform);
		finalMesh = DirectX::XMMatrixTranspose(finalMesh);
		DirectX::XMStoreFloat4x4(&finalTransfrom, finalMesh);
		strucBuff.getData(id) = finalTransfrom;
		DirectX::XMStoreFloat4x4(&finalTransfrom, globalTrasform);
	}
	else
	{
		finalTransfrom = parentTrasform;
	}

	for (int i = 0, end = (int)node.children.size(); i < end; i++)
	{

		uppdateMatrices(animationIndex, animationTime, node.children[i], finalTransfrom);
	}
}

void AnimatedMesh::findlowRotationNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm)
{
	for (int i = 0, end = (int)nodeAnm.rotKeyFrame.size() - 1; i < end; i++)
	{
		if (AnimationTimeTicks < nodeAnm.rotKeyFrame[i + 1].Time)
		{
			out = i;
			return;
		}
	}
	out = 0;
	return;
}

void AnimatedMesh::InterpolateRotation(DirectX::XMFLOAT4& res, const float animationTime, const channels& animationNode, const channels& target)
{
	t1 = 0;
	t2 = 0;
	start = {};
	end = {};
	factor = 0;
	if (animationNode.rotKeyFrame.size() == 1)
	{
		res = animationNode.rotKeyFrame[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowRotationNode(lowRotaKey, animationTime, animationNode);

	int nextRotation = lowRotaKey + 1;
	t1 = animationNode.rotKeyFrame[lowRotaKey].Time;
	start = DirectX::XMLoadFloat4(&animationNode.rotKeyFrame[lowRotaKey].Value);
	if (this->state == 1)
	{
		t2 = animationNode.rotKeyFrame[nextRotation].Time;
		end = DirectX::XMLoadFloat4(&animationNode.rotKeyFrame[nextRotation].Value);
		factor = (animationTime - t1) / (t2 - t1);
	}
	else
	{
		end = DirectX::XMLoadFloat4(&target.rotKeyFrame[0].Value);
		factor = this->oldTime;
	}

	DirectX::XMVECTOR vectorRez = DirectX::XMQuaternionSlerp(start, end, (float)factor);
	vectorRez = DirectX::XMVector4Normalize(vectorRez);
	DirectX::XMStoreFloat4(&res, vectorRez);
}

void AnimatedMesh::findlowScaleNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm)
{
	for (int i = 0, end = (int)nodeAnm.scalKeyFrames.size() - 1; i < end; i++)
	{
		if (AnimationTimeTicks < nodeAnm.scalKeyFrames[i + 1].Time)
		{
			out = i;
			return;
		}
	}
	out = 0;
	return;
}

void AnimatedMesh::InterpolateScaling(DirectX::XMFLOAT3& res, const float animationTime, const channels& animationNode, const channels& target)
{
	t1 = 0;
	t2 = 0;
	start = {};
	end = {};
	factor = 0;
	if (animationNode.scalKeyFrames.size() == 1)
	{
		res = animationNode.scalKeyFrames[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowScaleNode(lowRotaKey, animationTime, animationNode);
	int nextRotation = lowRotaKey + 1;
	t1 = animationNode.scalKeyFrames[lowRotaKey].Time;
	start = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[lowRotaKey].Value);
	if (this->state == 1)
	{
		t2 = animationNode.scalKeyFrames[nextRotation].Time;
		end = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[nextRotation].Value);
		factor = (animationTime - t1) / (t2 - t1);
	}
	else
	{
		end = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[0].Value);
		factor = this->oldTime;
	}

	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, (float) factor);
	DirectX::XMStoreFloat3(&res, vectorRez);
}

void AnimatedMesh::findlowPosNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm)
{
	for (int i = 0, end = (int)nodeAnm.posKeyFrames.size() - 1; i < end; i++)
	{
		if (AnimationTimeTicks < nodeAnm.posKeyFrames[i + 1].Time)
		{
			out = i;
			return;
		}
	}
	out = 0;
	return;
}

void AnimatedMesh::InterpolatePos(DirectX::XMFLOAT3& res, const float animationTime, const channels& animationNode, const channels& target)
{
	t1 = 0;
	t2 = 0;
	start = {};
	end = {};
	factor = 0;
	if (animationNode.posKeyFrames.size() == 1)
	{
		res = animationNode.posKeyFrames[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowPosNode(lowRotaKey, animationTime, animationNode);
	int nextRotation = lowRotaKey + 1;
	t1 = animationNode.posKeyFrames[lowRotaKey].Time;
	start = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[lowRotaKey].Value);
	if (this->state == 1)
	{
		t2 = animationNode.posKeyFrames[nextRotation].Time;
		end = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[nextRotation].Value);
		factor = (animationTime - t1) / (t2 - t1);
	}
	else
	{
		end = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[0].Value);
		factor = this->oldTime;
	}

	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, (float) factor);
	//vectorRez = DirectX::XMVector3Normalize(start);
	DirectX::XMStoreFloat3(&res, vectorRez);
}

bool AnimatedMesh::findNodeAnim(const std::string& nodeName, const animationNode& pAnimation, channels& res)
{
	for (int i = 0, end = (int)pAnimation.mChannels.size(); i < end; i++)
	{
		//memcmp(pAnimation.mChannels[i].mNodeName.c_str(), nodeName.c_str(), pAnimation.mChannels[i].mNodeName.size())==0
		//can be optimized
		if (pAnimation.mChannels[i].mNodeName.size() == nodeName.size())
		{
			if (pAnimation.mChannels[i].mNodeName == nodeName)
			{
				res = pAnimation.mChannels[i];
				return true;
			}
		}
	}
	return false;
}

void AnimatedMesh::getTimeInTicks(const float& dt, const unsigned& animationIndex)
{
	double tps = MySimp.animation[0].ticksPerSecond;
	if (tps == 0)
	{
		tps = 25.f;
	}
	double timeInTicks = totalTime * tps;
	if (timeInTicks >= MySimp.animation[0].duration)
	{
		timeInTicks -= MySimp.animation[0].duration;
		totalTime = 0;
	}

	DirectX::XMFLOAT4X4 startMatrix;
	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&startMatrix, temp);
	this->uppdateMatrices(animationIndex, (float)timeInTicks, MySimp.rootNode, startMatrix);
}

AnimatedMesh::AnimatedMesh(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int id, GravityField* field)
	:GameObject(useMesh, pos, rot, id, field)
{
	this->totalTime = 0;
	oldAnimId = 0;
	oldTime = 0;
	returning = false;
	//this->setScale(DirectX::XMFLOAT3(0.02, 0.02, 0.02));
	//this->updateBuffer();
}

AnimatedMesh::~AnimatedMesh()
{
}

bool AnimatedMesh::addAnimations(std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (pScene == nullptr)
	{
		return false;
	}

	for (int i = 0, end = pScene->mNumAnimations; i < end; i++)
	{
		this->MySimp.animation.emplace_back();
		this->MySimp.animation[this->MySimp.animation.size() - 1].mName = pScene->mAnimations[i]->mName.C_Str();
		this->MySimp.animation[this->MySimp.animation.size() - 1].duration = pScene->mAnimations[i]->mDuration;
		this->MySimp.animation[this->MySimp.animation.size() - 1].ticksPerSecond = pScene->mAnimations[i]->mTicksPerSecond;

		this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels.reserve(pScene->mAnimations[i]->mNumChannels);
		for (int j = 0, length = pScene->mAnimations[i]->mNumChannels; j < length; j++)
		{
			this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels.emplace_back();
			this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].mNodeName = pScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

			this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].posKeyFrames.reserve(pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys);
			this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].rotKeyFrame.reserve(pScene->mAnimations[i]->mChannels[j]->mNumRotationKeys);
			this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].scalKeyFrames.reserve(pScene->mAnimations[i]->mChannels[j]->mNumScalingKeys);

			for (int k = 0, length = pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k < length; k++)
			{
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].posKeyFrames.emplace_back();
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].posKeyFrames[k].Time = pScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].posKeyFrames[k].addAiVector3D(pScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue);
			}
			for (int k = 0, length = pScene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k < length; k++)
			{
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].rotKeyFrame.emplace_back();
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].rotKeyFrame[k].Time = pScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].rotKeyFrame[k].addAiQuaternion(pScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue);
			}
			for (int k = 0, length = pScene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k < length; k++)
			{
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].scalKeyFrames.emplace_back();
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].scalKeyFrames[k].Time = pScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
				this->MySimp.animation[this->MySimp.animation.size() - 1].mChannels[j].scalKeyFrames[k].addAiVector3D(pScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue);
			}
		}
	}
	return true;
}

void AnimatedMesh::addData(const AnimationData& data)
{
	this->MySimp = data;
	std::vector<DirectX::XMFLOAT4X4> tempfloatvec;
	tempfloatvec.reserve(data.boneVector.size());

	DirectX::XMFLOAT4X4 identityFloat;
	DirectX::XMStoreFloat4x4(&identityFloat, DirectX::XMMatrixIdentity());
	for (size_t i = 0; i < data.boneVector.size(); i++)
	{
		tempfloatvec.push_back(identityFloat);
	}
	strucBuff.Initialize(GPU::device, GPU::immediateContext, tempfloatvec);
}

void AnimatedMesh::updateAnim(const float& dt, const unsigned& animIndex, float animationSpeed)
{
	if (this->MySimp.animation.size() <= animIndex)
	{
		ErrorLog::Log("Invalid Animation Id!");
		return;
	}
	if (GetAsyncKeyState('W') || GetAsyncKeyState('A') || GetAsyncKeyState('S') || GetAsyncKeyState('D') || GetAsyncKeyState(' '))
	{
		state = 1;
		this->totalTime += dt;
		returning = false;
		float AnimTime = dt * animationSpeed;
		this->getTimeInTicks(AnimTime, animIndex);
	}
	else
	{
		// 1 = walking, 2 = returning, 0 standing still
		if (state == 1)
		{
			returning = true;
			state = 2;
			this->oldTime = 0.0f;
		}
		else if ((oldTime >= 1) && state == 2)
		{
			returning = false;
			state = 0;
			this->oldTime = 0;
		}
		else if (state == 2)
		{
			float timeOffset = dt * 6;
			oldTime += timeOffset;
		}
		if (oldTime >= 1)
		{
			oldTime = 1;
		}
		if (state != 0)
		{
			this->getTimeInTicks(totalTime, animIndex);
		}
	}
}

void AnimatedMesh::draw()
{
	strucBuff.applyData();
	strucBuff.BindToVS(0);
	this->tmpDraw(sizeof(AnimatedVertex));
}