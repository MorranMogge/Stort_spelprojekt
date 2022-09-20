#pragma once

#include <vector>
#include "StructuredBuffer.h"
#include "GPU.h"
#include <fstream>

struct Joint
{
	int parentIndex;
	DirectX::XMMATRIX bindposeInverse;
	DirectX::XMMATRIX localTransform;
};

class JointAnimation
{

	struct JointPrivate : Joint
	{
		DirectX::XMMATRIX transform; // (dynamic) Current model transform
		DirectX::XMMATRIX finalTransform; // (dynamic) What to upload to GPU
	};

	std::vector<JointPrivate> joints;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	StructuredBuffer<DirectX::XMMATRIX> sb;

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

	JointAnimation(std::vector<Joint>& bones)
	{

		//// Root do not have a parent
		//joints[0].transform = joints[0].localTransform;
		//joints[0].bindposeInverse = joints[0].transform.Invert();
		//
		//for (auto& joint : joints)
		//{
		//	// joint.transform changes the base from model space
		//	// to the base of the bone in the current pose.
		//	joint.transform = joints[joint.parent].transform * joint.localTransform;
		//
		//	// joint.bindposeInverse changes the base of the bone
		//	// IN BIND POSE to the model space.
		//	joint.bindposeInverse = joint.transform.Invert();
		//}

		//create structure buffer
		std::vector<DirectX::XMMATRIX> structVector(joints.size());
		sb.Initialize(GPU::device, GPU::immediateContext, structVector);
		sb.applyData();

		InputLayoutAndVertexShader();

	}

	void Update()
	{

		// Root do not have a parent
		joints[0].transform = joints[0].localTransform;
		joints[0].finalTransform = joints[0].transform * joints[0].bindposeInverse;

		// Iterate the rest of the bones
		for (int i = 0; i < joints.size(); i++)
		{
			// joint.transform changes the base from model space
			// to the base of the bone in the current pose.
			joints[i].transform = joints[joints[i].parentIndex].transform * joints[i].localTransform;

			// joint.finalTransform transforms a vertex in model space to the
			// currently animated pose, also in model space.
			joints[i].finalTransform = joints[i].transform * joints[i].bindposeInverse;

			sb.getData(i) = DirectX::XMMatrixTranspose(joints[i].finalTransform);
		}

		//update structure buffer
		sb.applyData();

	}

	void Bind(UINT toRegister)
	{
		sb.BindToVS(toRegister);
		GPU::immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
		GPU::immediateContext->IASetInputLayout(inputLayout.Get());
	}

};