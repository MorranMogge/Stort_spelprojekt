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
		animationNode an = this->MySimp.animation[animationIndex];

		DirectX::XMMATRIX currentNodeTrans = DirectX::XMLoadFloat4x4(&node.trasformation);
		channels amnNode;



		if (this->findNodeAnim(node.nodeName, an, amnNode))
		{
			DirectX::XMFLOAT3 Scaling;
			this->InterpolateScaling(Scaling, animationTime, amnNode);
			DirectX::XMMATRIX ScalingM = DirectX::XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);

			DirectX::XMFLOAT4 r;
			this->InterpolateRotation(r, animationTime, amnNode);
			DirectX::XMMATRIX RotationM = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(r.x, r.y, r.z, r.w));

			DirectX::XMFLOAT3 Translation;
			this->InterpolatePos(Translation, animationTime, amnNode);
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

	for (int i = 0, end = node.children.size(); i < end; i++)
	{

		uppdateMatrices(animationIndex, animationTime, node.children[i], finalTransfrom);
	}
}

void AnimatedMesh::findlowRotationNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm)
{
	for (int i = 0, end = nodeAnm.rotKeyFrame.size() - 1; i < end; i++)
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

void AnimatedMesh::InterpolateRotation(DirectX::XMFLOAT4& res, float animationTime, const channels& animationNode)
{
	if (animationNode.rotKeyFrame.size() == 1)
	{
		res = animationNode.rotKeyFrame[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowRotationNode(lowRotaKey, animationTime, animationNode);
	//new
		res = animationNode.rotKeyFrame[lowRotaKey].Value;
		return;
	//end new
	int nextRotation = lowRotaKey + 1;
	float t1 = animationNode.rotKeyFrame[lowRotaKey].Time;
	float t2 = animationNode.rotKeyFrame[nextRotation].Time;

	float factor = (animationTime - t1) / (t2 - t1);
	const DirectX::XMVECTOR& start = DirectX::XMLoadFloat4(&animationNode.rotKeyFrame[lowRotaKey].Value);
	const DirectX::XMVECTOR& end = DirectX::XMLoadFloat4(&animationNode.rotKeyFrame[nextRotation].Value);
	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, factor);
	vectorRez = DirectX::XMVector4Normalize(start);
	DirectX::XMStoreFloat4(&res, vectorRez);
}

void AnimatedMesh::findlowScaleNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm)
{
	for (int i = 0, end = nodeAnm.scalKeyFrames.size() - 1; i < end; i++)
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

void AnimatedMesh::InterpolateScaling(DirectX::XMFLOAT3& res, float animationTime, const channels& animationNode)
{
	if (animationNode.scalKeyFrames.size() == 1)
	{
		res = animationNode.scalKeyFrames[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowScaleNode(lowRotaKey, animationTime, animationNode);
	//new
		res = animationNode.scalKeyFrames[lowRotaKey].Value;
		return;
	//end new
	int nextRotation = lowRotaKey + 1;
	float t1 = animationNode.scalKeyFrames[lowRotaKey].Time;
	float t2 = animationNode.scalKeyFrames[nextRotation].Time;

	float factor = (animationTime - t1) / (t2 - t1);
	const DirectX::XMVECTOR& start = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[lowRotaKey].Value);
	const DirectX::XMVECTOR& end = DirectX::XMLoadFloat3(&animationNode.scalKeyFrames[nextRotation].Value);
	//res = start + factor * (end-start);
	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, factor);
	DirectX::XMStoreFloat3(&res, vectorRez);
}

void AnimatedMesh::findlowPosNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm)
{
	for (int i = 0, end = nodeAnm.posKeyFrames.size() - 1; i < end; i++)
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

void AnimatedMesh::InterpolatePos(DirectX::XMFLOAT3& res, float animationTime, const channels& animationNode)
{
	if (animationNode.posKeyFrames.size() == 1)
	{
		res = animationNode.posKeyFrames[0].Value;
		return;
	}
	int lowRotaKey = 0;
	this->findlowPosNode(lowRotaKey, animationTime, animationNode);
	//new
		res = animationNode.posKeyFrames[lowRotaKey].Value;
		return;
	//end new
	int nextRotation = lowRotaKey + 1;
	float t1 = animationNode.posKeyFrames[lowRotaKey].Time;
	float t2 = animationNode.posKeyFrames[nextRotation].Time;

	float factor = (animationTime - t1) / (t2 - t1);
	const DirectX::XMVECTOR& start = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[lowRotaKey].Value);
	const DirectX::XMVECTOR& end = DirectX::XMLoadFloat3(&animationNode.posKeyFrames[nextRotation].Value);
	//res = start + factor * (end-start);
	DirectX::XMVECTOR vectorRez = DirectX::XMVectorLerp(start, end, factor);
	//vectorRez = DirectX::XMVector4Normalize(vectorRez);
	DirectX::XMStoreFloat3(&res, start);
}

bool AnimatedMesh::findNodeAnim(const std::string& nodeName,  const animationNode pAnimation, channels& res)
{
	for (int i = 0, end = pAnimation.mChannels.size(); i < end; i++)
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
	this->totalTime += dt;
	float tps = MySimp.animation[0].ticksPerSecond;
	if (tps == 0)
	{
		tps = 25.f;
	}
	float timeInTicks = totalTime * tps;
	if (timeInTicks >= MySimp.animation[0].duration)
	{
		timeInTicks -= MySimp.animation[0].duration;
		totalTime = 0;
		lastFramesTick = -1;
	}
	int wholeNumner = (int)timeInTicks;
	if (wholeNumner != lastFramesTick)
	{
		//std::cout << "new Tick, last was: " << lastFramesTick << " new tick is: " << wholeNumner << std::endl;
		lastFramesTick = wholeNumner;
	}
	else
	{
		std::cout << "old tick" << std::endl;
		return;
	}

	DirectX::XMFLOAT4X4 startMatrix;
	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&startMatrix, temp);
	this->uppdateMatrices(animationIndex, timeInTicks, MySimp.rootNode, startMatrix);
}

AnimatedMesh::AnimatedMesh(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field)
	:GameObject(useMesh, pos, rot, id, field)
{
	this->totalTime = 0;
	lastFramesTick = -1;
	this->setScale(DirectX::XMFLOAT3(5, 5, 5));
	this->updateBuffer();
	//strucBuff.Initialize(GPU::device, GPU::immediateContext, finalTransforms);
	///();
}

AnimatedMesh::AnimatedMesh(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(objectPath, pos, rot, id)
{
	//strucBuff.Initialize(GPU::device, GPU::immediateContext, finalTransforms);

}

AnimatedMesh::AnimatedMesh()
{
	//strucBuff.Initialize(GPU::device, GPU::immediateContext, finalTransforms);
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

	//std::vector<DirectX::XMFLOAT4X4> structVector(boneVector.size());
	//this->boneStrucBuf.Initialize(GPU::device, GPU::immediateContext, structVector);
}

void AnimatedMesh::draw(const float& dt, const unsigned& animationIndex)
{
	if (this->MySimp.animation.size() <= animationIndex)
	{
		ErrorLog::Log("Invalid Animation Id!");
		return;
	}
	DirectX::XMFLOAT4X4 identityFloat;
	DirectX::XMStoreFloat4x4(&identityFloat, DirectX::XMMatrixIdentity());
	this->getTimeInTicks(dt, animationIndex);
	strucBuff.applyData();
	strucBuff.BindToVS(0);

	this->tmpDraw(sizeof(AnimatedVertex));
}