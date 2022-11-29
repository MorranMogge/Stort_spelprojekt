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
	DirectX::XMFLOAT4X4 trasformation = {};
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
	std::unordered_map<std::string, int> boneNameToIndex;
	std::vector<boneInfo> boneVector;
	std::vector<IndexBoneData> boneDataVec;
	StructuredBuffer<DirectX::XMFLOAT4X4> boneStrucBuf;
	DirectX::XMFLOAT4X4 globalInverseTransform = {};
};

class AnimatedMesh : public GameObject
{
private:
	//aiScene* scene;

	AnimationData MySimp;

	StructuredBuffer<DirectX::XMFLOAT4X4> strucBuff;

	float totalTime;


	//inerpolate
	double t1;
	double t2;
	DirectX::XMVECTOR start = {};
	DirectX::XMVECTOR end = {};
	double factor;

	void uppdateMatrices(int animationIndex, const float animationTime, const nodes& node, DirectX::XMFLOAT4X4& parentTrasform);

	void findlowRotationNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm);
	void InterpolateRotation(DirectX::XMFLOAT4& res, const float animationTime, const channels& animationNode, const channels& target);

	void findlowScaleNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm);
	void InterpolateScaling(DirectX::XMFLOAT3& res, const float animationTime, const channels& animationNode, const channels& target);

	void findlowPosNode(int& out, const float& AnimationTimeTicks, const channels& nodeAnm);
	void InterpolatePos(DirectX::XMFLOAT3& res, const float animationTime, const channels& animationNode, const channels& target);

	bool findNodeAnim(const std::string& nodeName, const animationNode& pAnimation, channels& res);
	void getTimeInTicks(const float& dt, const unsigned& animationIndex);

	float oldTime;
	int oldAnimId;
	int state;
	float hitStart;
	float throwStart;

	//for server
	int currentAnimId = 0;

protected:

	bool doneWithAnim;

public:
	AnimatedMesh(Mesh* useMesh, const AnimationData& data ,const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int id, GravityField* field = nullptr);
	//AnimatedMesh(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	//AnimatedMesh(ID3D11Buffer* vertexBuff, ID3D11Buffer* indexBuff, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertices);
	//AnimatedMesh();
	~AnimatedMesh();

	void forwardKinematics(const std::string& nodeName, DirectX::XMFLOAT4X4& outMatrix);
	void addData(const AnimationData& data);
	void updateAnim(const float& dt, int animIndex, float animationSpeed = 1.0f);
	void draw();
	int getAnimId();
};