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
		animationNode& basic = this->MySimp.animation[oldAnimId];

		DirectX::XMMATRIX currentNodeTrans = DirectX::XMLoadFloat4x4(&node.trasformation);
		channels amnNode;
		channels originalPose;

		if (node.nodeName == "hand3:hand3:LeftHandThumb1")
		{
			int bp = 2;
		}

		if (this->findNodeAnim(node.nodeName, an, amnNode))
		{
			if (!this->findNodeAnim(node.nodeName, basic, originalPose))
			{
				DirectX::XMVECTOR pos;
				DirectX::XMVECTOR rot;
				DirectX::XMVECTOR scale;
				DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&node.trasformation);
				DirectX::XMMatrixDecompose(&pos, &rot, &scale, temp);

				originalPose.posKeyFrames.emplace_back();
				DirectX::XMStoreFloat3(&originalPose.posKeyFrames[0].Value, pos);
				originalPose.rotKeyFrame.emplace_back();
				DirectX::XMStoreFloat4(&originalPose.rotKeyFrame[0].Value, rot);
				originalPose.scalKeyFrames.emplace_back();
				DirectX::XMStoreFloat3(&originalPose.scalKeyFrames[0].Value, scale);
			}

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
		end = start;
		start = DirectX::XMLoadFloat4(&target.rotKeyFrame[0].Value);
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
		end = start;
		start = DirectX::XMLoadFloat3(&target.scalKeyFrames[0].Value);
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
		end = start;
		start = DirectX::XMLoadFloat3(&target.posKeyFrames[0].Value);
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
	double tps = MySimp.animation[animationIndex].ticksPerSecond;
	if (tps == 0)
	{
		tps = 25.f;
	}
	double timeInTicks = totalTime * tps;
	if (timeInTicks >= MySimp.animation[animationIndex].duration)
	{
		timeInTicks -= MySimp.animation[animationIndex].duration;
		totalTime = 0;
		doneWithAnim = true;
	}

	DirectX::XMFLOAT4X4 startMatrix;
	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&startMatrix, temp);
	this->uppdateMatrices(animationIndex, (float)timeInTicks, MySimp.rootNode, startMatrix);
}

void AnimatedMesh::forwardKinematics(const std::string& nodeName, DirectX::XMFLOAT4X4& outMatrix)
{
	auto it = MySimp.boneNameToIndex.find(nodeName);
	if (it != MySimp.boneNameToIndex.end())
	{
		int index = it->second;
		outMatrix = this->strucBuff.getData(index);
		DirectX::XMMATRIX JointMatrix = DirectX::XMLoadFloat4x4(&outMatrix);
		JointMatrix = DirectX::XMMatrixTranspose(JointMatrix);

		int id = MySimp.boneNameToIndex[nodeName];
		DirectX::XMMATRIX boneOffset = DirectX::XMLoadFloat4x4(&MySimp.boneVector[id].offsetMatrix);
		boneOffset = DirectX::XMMatrixInverse(nullptr, boneOffset);
		JointMatrix = boneOffset * JointMatrix;
		const DirectX::XMFLOAT4X4 f1 = this->getMatrix();
		DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&f1);
		worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);

		DirectX::XMMATRIX m3 = DirectX::XMMatrixMultiply(JointMatrix, worldMatrix);
		//m3 = DirectX::XMMatrixTranspose(m3);
		DirectX::XMStoreFloat4x4(&outMatrix, m3);
		return;
	}
}

AnimatedMesh::AnimatedMesh(Mesh* useMesh, const AnimationData& data, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int id, GravityField* field, const bool fixStandardAnims)
	:GameObject(useMesh, pos, rot, id, field)
{
	this->currentAnimId = 1;
	doneWithAnim = true;
	this->totalTime = 0;
	oldAnimId = 0;
	oldTime = 2;
	state = 2;
	this->setScale(DirectX::XMFLOAT3(1, 1, 1));
	this->updateBuffer();

	this->MySimp = data;
	if (fixStandardAnims)
	{
		this->MySimp.animation[2].duration -= 1;

		this->hitStart = this->MySimp.animation[4].duration / 4;
		this->hitStart /= this->MySimp.animation[4].ticksPerSecond;
		this->MySimp.animation[4].duration /= 1.5;
		this->MySimp.animation[2].duration -= 1;

		throwStart = this->MySimp.animation[3].duration * 0.25;
		this->throwStart /= this->MySimp.animation[3].ticksPerSecond;
		this->MySimp.animation[3].duration *= 0.66;

		this->MySimp.animation[1].duration -= 1;
	}

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

void AnimatedMesh::updateAnim(const float& dt, int animIndex, float animationSpeed)
{
	if (this->MySimp.animation.size() <= animIndex)
	{
		//std::cout << "i got anim id: " << animIndex << " but i onlt have: " << this->MySimp.animation.size() << " number of anims\n";
		//ErrorLog::Log("Invalid Animation Id!");
		return;
	}
	this->currentAnimId = animIndex;

	if (animIndex == oldAnimId)
	{
		state = 1;
		float AnimTime = dt * animationSpeed;
		this->totalTime += AnimTime;
		this->getTimeInTicks(totalTime, animIndex);
	}
	else
	{
		// 1 = walking, 2 = returning, 0 standing still
		if (state == 1)
		{
			state = 2;
			this->oldTime = 0.f;
		}
		if (state == 2)
		{
			float timeOffset = dt;
			oldTime += timeOffset * 10;
		}
		if ((oldTime >= 1) && state == 2) // done interpolating, set new state as old so it can play
		{
			state = 2;
			this->oldTime = 0;
			this->totalTime = 0;
			int temp = oldAnimId;
			this->oldAnimId = animIndex;
			if (animIndex == 4)
			{
				this->totalTime = hitStart;
			}
			if (animIndex == 3)
			{
				this->totalTime = throwStart;
			}
		}
		//if (oldTime >= 1) //dont over interpolate
		//{
		//	oldTime = 1;
		//}
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

void AnimatedMesh::draw(std::vector<ID3D11ShaderResourceView*>& allTextures)
{
	strucBuff.applyData();
	strucBuff.BindToVS(0);
	this->tmpDraw(allTextures, sizeof(AnimatedVertex));
}

void AnimatedMesh::getAnimId(int& id)
{
	id = this->currentAnimId;
}

void AnimatedMesh::setTextures(std::vector<ID3D11ShaderResourceView*>& textures)
{
	this->textures = textures;
}

void AnimatedMesh::drawSubMeshesWithTexture()
{
	draw(this->textures);
}