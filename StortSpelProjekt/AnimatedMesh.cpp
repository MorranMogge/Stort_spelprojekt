#include "stdafx.h"
#include "AnimatedMesh.h"
#include "ModelManager.h"
//using namespace DirectX;

void AnimatedMesh::uppdateMatrices(int animationIndex, float animationTime, const nodes& node, DirectX::XMFLOAT4X4& parentTrasform)
{
	//std::cout << "Node Name: " << node.nodeName << "\n";
	DirectX::XMFLOAT4X4 finalTransfrom;

	if (MySimp.boneNameToIndex.find(node.nodeName) != MySimp.boneNameToIndex.end())
	{
		animationNode& an = this->MySimp.animation[animationIndex];
		animationNode& basic = this->MySimp.animation[0];

		DirectX::XMMATRIX currentNodeTrans = DirectX::XMLoadFloat4x4(&node.trasformation);
		channels amnNode;
		channels originalPose;
		this->findNodeAnim(node.nodeName, basic, originalPose);
		if (this->findNodeAnim(node.nodeName, an, amnNode))
		{
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

void AnimatedMesh::InterpolateRotation(DirectX::XMFLOAT4& res, float animationTime, const channels& animationNode, const channels& target)
{
	double t1;
	double t2;
	DirectX::XMVECTOR start = {};
	DirectX::XMVECTOR end = {};
	double factor;
	if (animationNode.rotKeyFrame.size() == 1)
	{
		res = animationNode.rotKeyFrame[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowRotationNode(lowRotaKey, animationTime, animationNode);
	//new
		//res = animationNode.rotKeyFrame[lowRotaKey].Value;
		//return;
	//end new
	int nextRotation = lowRotaKey + 1;
	t1 = animationNode.rotKeyFrame[lowRotaKey].Time;
	start = DirectX::XMLoadFloat4(&animationNode.rotKeyFrame[lowRotaKey].Value);
	if (!this->returning)
	{
		t2 = animationNode.rotKeyFrame[nextRotation].Time;
		end = DirectX::XMLoadFloat4(&animationNode.rotKeyFrame[nextRotation].Value);
		factor = (animationTime - t1) / (t2 - t1);
	}
	else
	{
		t2 = t1;
		t1 = target.rotKeyFrame[0].Time;
		end = DirectX::XMLoadFloat4(&target.rotKeyFrame[0].Value);
		factor = this->oldTime;
	}

	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, (float)factor);
	vectorRez = DirectX::XMVector4Normalize(start);
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

void AnimatedMesh::InterpolateScaling(DirectX::XMFLOAT3& res, float animationTime, const channels& animationNode, const channels& target)
{
	double t1;
	double t2;
	DirectX::XMVECTOR start = {};
	DirectX::XMVECTOR end = {};
	double factor;
	if (animationNode.scalKeyFrames.size() == 1)
	{
		res = animationNode.scalKeyFrames[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowScaleNode(lowRotaKey, animationTime, animationNode);
	//new
		//res = animationNode.scalKeyFrames[lowRotaKey].Value;
		//return;
	//end new
	int nextRotation = lowRotaKey + 1;
	t1 = animationNode.scalKeyFrames[lowRotaKey].Time;
	start = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[lowRotaKey].Value);
	if (!this->returning)
	{
		t2 = animationNode.scalKeyFrames[nextRotation].Time;
		end = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[nextRotation].Value);
		factor = (animationTime - t1) / (t2 - t1);
	}
	else
	{
		t2 = t1;
		t1 = target.scalKeyFrames[0].Time;
		end = DirectX::XMLoadFloat3(&target.scalKeyFrames[0].Value);
		factor = this->oldTime;
	}

	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, (float) factor);
	//vectorRez = DirectX::XMVector3Normalize(start);
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

void AnimatedMesh::InterpolatePos(DirectX::XMFLOAT3& res, float animationTime, const channels& animationNode, const channels& target)
{
	double t1;
	double t2;
	DirectX::XMVECTOR start = {};
	DirectX::XMVECTOR end = {};
	double factor;
	if (animationNode.posKeyFrames.size() == 1)
	{
		res = animationNode.posKeyFrames[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowPosNode(lowRotaKey, animationTime, animationNode);
	//new
		//res = animationNode.posKeyFrames[lowRotaKey].Value;
		//return;
	//end new
	int nextRotation = lowRotaKey + 1;
	t1 = animationNode.posKeyFrames[lowRotaKey].Time;
	start = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[lowRotaKey].Value);
	if (!this->returning)
	{
		t2 = animationNode.posKeyFrames[nextRotation].Time;
		end = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[nextRotation].Value);
		factor = (animationTime - t1) / (t2 - t1);
	}
	else
	{
		t2 = t1;
		t1 = target.posKeyFrames[0].Time;
		end = DirectX::XMLoadFloat3(&target.posKeyFrames[0].Value);
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
		if (pAnimation.mChannels[i].mNodeName == nodeName)
		{
			res = pAnimation.mChannels[i];
			return true;
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

AnimatedMesh::AnimatedMesh(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field)
	:GameObject(useMesh, pos, rot, id, field)
{
	this->totalTime = 0;
	oldAnimId = 0;
	oldTime = 0;
	returning = false;
}

AnimatedMesh::~AnimatedMesh()
{
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
		this->totalTime += dt;
		returning = false;
		DirectX::XMFLOAT4X4 identityFloat;
		DirectX::XMStoreFloat4x4(&identityFloat, DirectX::XMMatrixIdentity());
		float AnimTime = dt * animationSpeed;
		this->getTimeInTicks(AnimTime, animIndex);
		oldAnimId = animIndex;
	}
	else
	{
		if (!returning)
		{
			returning = true;
			this->oldTime = 1.0f;
		}
		else if (oldTime <= 0)
		{
			returning = false;
		}
		float timeOffset = dt * 4;
		oldTime -= timeOffset;
		if (oldTime <= 0)
		{
			oldTime = 0;
		}
	}
}

void AnimatedMesh::draw()
{
	strucBuff.applyData();
	strucBuff.BindToVS(0);
	this->tmpDraw(sizeof(AnimatedVertex));
}