#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <SimpleMath.h>
#include "GameObject.h"
#include "StructuredBuffer.h"
#include "GPU.h"


struct boneInfo
{
	int parentID;
	std::string name;
	DirectX::XMFLOAT4X4 offsetMatrix;

	boneInfo(const DirectX::XMFLOAT4X4& offset)
	{
		offsetMatrix = offset;
		parentID = -1;
		name = "";
	}
};

struct IndexBoneData
{
	//int boneName;
	int BoneIDs[4] = { 0,0,0,0 };
	float Weights[4] = { 0,0,0,0 };
};


struct nodes
{
	std::vector<nodes> children;
	std::string nodeName;
	DirectX::XMFLOAT4X4 trasformation;
};

struct float3KeyFrame
{
	DirectX::XMFLOAT3 Value;
	double Time;
	void addAiVector3D(const aiVector3D& other)
	{
		Value.x = other.x;
		Value.y = other.y;
		Value.z = other.z;
	}
};

struct float4KeyFrame
{
	DirectX::XMFLOAT4 Value;
	double Time;
	void addAiQuaternion(const aiQuaternion& other)
	{
		Value.x = other.x;
		Value.y = other.y;
		Value.z = other.z;
		Value.w = other.w;
	}
};

struct channels
{
	std::string mNodeName;
	std::vector<float3KeyFrame> posKeyFrames;
	std::vector<float4KeyFrame> rotKeyFrame;
	std::vector<float3KeyFrame> scalKeyFrames;
};

struct animationNode
{
	std::vector<channels> mChannels;
	std::string mName;
	double duration;
	double ticksPerSecond;
};

struct AnimationData
{	
	nodes rootNode;
	std::vector<animationNode> animation;
	std::map<std::string, int> boneNameToIndex;
	std::vector<boneInfo> boneVector;
	std::vector<IndexBoneData> boneDataVec;
	StructuredBuffer<DirectX::XMFLOAT4X4> boneStrucBuf;
	DirectX::XMFLOAT4X4 globalInverseTransform;
};

class AnimatedMesh : public GameObject
{
private:
	//aiScene* scene;

	AnimationData MySimp;

	StructuredBuffer<DirectX::XMFLOAT4X4> strucBuff;

	float totalTime;

	void uppdateMatrices(int animationIndex, float animationTime, const nodes& node, DirectX::XMFLOAT4X4& parentTrasform);

	void findlowRotationNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm);
	void InterpolateRotation(DirectX::XMFLOAT4& res, float animationTime, const channels& animationNode);

	void findlowScaleNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm);
	void InterpolateScaling(DirectX::XMFLOAT3& res, float animationTime, const channels& animationNode);

	void findlowPosNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm);
	void InterpolatePos(DirectX::XMFLOAT3& res, float animationTime, const channels& animationNode);

	//const aiNodeAnim* findNodeAnim(const std::string& nodeName, const aiAnimation* pAnimation);
	bool findNodeAnim(const std::string& nodeName, const animationNode pAnimation, channels& res);

	void getTimeInTicks(float dt);

	// forbidden code

public:
	AnimatedMesh(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	AnimatedMesh(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	AnimatedMesh(ID3D11Buffer* vertexBuff, ID3D11Buffer* indexBuff, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertices);
	AnimatedMesh();
	~AnimatedMesh();


	void addData(const AnimationData& data);

	void draw(const float& dt, const int& animIndex);
};