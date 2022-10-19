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
	int BoneIDs[4] = { 0 };
	float Weights[4] = { 0 };
};


struct nodes
{
	std::vector<nodes*> children;
	std::string nodeName;
	nodes* parent;
	DirectX::XMFLOAT4X4 trasformation;
	~nodes()
	{
		for (int i = 0; i < children.size(); i++)
		{
			delete children[i];
		}
		delete parent;
	}
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

struct animation
{
	std::vector<channels> mChannels;
	std::string mName;
	double duration;
	double ticksPerSecond;
};

struct AnimationData
{	
	nodes* rootNode;
	std::vector<animation> animation;
	std::map<std::string, int> boneNameToIndex;
	std::vector<boneInfo> boneVector;
	std::vector<IndexBoneData> boneDataVec;
	StructuredBuffer<DirectX::XMFLOAT4X4> boneStrucBuf;
};

class AnimatedMesh : public GameObject
{
private:
	aiScene* scene;

	AnimationData MySimp;

	void uppdateMatrices(int animationIndex, float animationTime, const aiNode* node, DirectX::XMFLOAT4X4& parentTrasform);

	void findlowRotationNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm);
	void InterpolateRotation(aiQuaternion& res, float animationTime, const aiNodeAnim* animationNode);

	void findlowScaleNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm);
	void InterpolateScaling(aiVector3D& res, float animationTime, const aiNodeAnim* animationNode);

	void findlowTransNode(int& out, const float& AnimationTimeTicks, const aiNodeAnim* nodeAnm);
	void InterpolateTranslation(aiVector3D& res, float animationTime, const aiNodeAnim* animationNode);

	const aiNodeAnim* findNodeAnim(const std::string& nodeName, const aiAnimation* pAnimation);

	void getTimeInTicks(float dt);

	// forbidden code

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	void InputLayoutAndVertexShader()
	{
		D3D11_INPUT_ELEMENT_DESC position{
			position.SemanticName = "POSITION",
			position.SemanticIndex = 0U,//semantic index, index of the vertex shader input, only have 1, so index 0
			position.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			position.InputSlot = 0U,
			position.AlignedByteOffset = 0U,//aligned byte offset, off set in the entaire element the structer from begining of the structer,0 is beginning in bytes,
			position.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			position.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		D3D11_INPUT_ELEMENT_DESC uv{
			uv.SemanticName = "UV",
			uv.SemanticIndex = 0U,//semantic index, index of the vertex shader input, we only have 1, so index 0
			uv.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
			uv.InputSlot = 0U,
			uv.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,//sizeof(float) * 3
			uv.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			uv.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		D3D11_INPUT_ELEMENT_DESC normal{
			normal.SemanticName = "NORMAL",
			normal.SemanticIndex = 0U,//semantic index, index of the vertex shader input, in this case, we only have 1, so index 0
			normal.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			normal.InputSlot = 0U,
			normal.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,//sizeof(float) * 5
			normal.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			normal.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		D3D11_INPUT_ELEMENT_DESC jointIndex{
			jointIndex.SemanticName = "JointIndex",
			jointIndex.SemanticIndex = 0U,//semantic index, index of the vertex shader input, in this case, we only have 1, so index 0
			jointIndex.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT,
			jointIndex.InputSlot = 0U,
			jointIndex.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,//sizeof(float) * 8
			jointIndex.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			jointIndex.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		D3D11_INPUT_ELEMENT_DESC weights{
			weights.SemanticName = "Weights",
			weights.SemanticIndex = 0U,//semantic index, index of the vertex shader input, in this case, we only have 1, so index 0
			weights.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
			weights.InputSlot = 0U,
			weights.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,//sizeof(float) * 8
			weights.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			weights.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		D3D11_INPUT_ELEMENT_DESC nrOfJoint{
			nrOfJoint.SemanticName = "NrOfJoint",
			nrOfJoint.SemanticIndex = 0U,//semantic index, index of the vertex shader input, in this case, we only have 1, so index 0
			nrOfJoint.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
			nrOfJoint.InputSlot = 0U,
			nrOfJoint.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,//sizeof(float) * 8
			nrOfJoint.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			nrOfJoint.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		const D3D11_INPUT_ELEMENT_DESC layout[]{
			position,
			uv,
			normal,
			jointIndex,
			weights,
			nrOfJoint
		};

		std::ifstream reader("VS_animation.cso", std::ios::binary | std::ios::ate);
		if (reader.is_open())
		{
			std::string byteCode;
			reader.seekg(0, std::ios::end);
			byteCode.reserve(static_cast<unsigned int>(reader.tellg()));
			reader.seekg(0, std::ios::beg);
			byteCode.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

			GPU::device->CreateVertexShader(byteCode.c_str(), byteCode.length(), nullptr, vertexShader.GetAddressOf());
			GPU::device->CreateInputLayout(layout, ARRAYSIZE(layout), byteCode.c_str(), byteCode.length(), inputLayout.GetAddressOf());

			reader.close();
		}
	}

public:
	AnimatedMesh(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	AnimatedMesh();
	~AnimatedMesh();


	void addData(const AnimationData& data);

	void uppdate(ID3D11DeviceContext* immediateContext, int animationIndex, const float dt);
};