#pragma once
#include <SimpleMath.h>
#include <vector>
#include "StructuredBuffer.h"
#include "GPU.h"

class JointAnimation
{

	struct JointInfo
	{
		int parent;
		DirectX::SimpleMath::Matrix bindposeInverse; // Inverse of model transform in bind pose
		DirectX::SimpleMath::Matrix transform; // (dynamic) Current model transform
		DirectX::SimpleMath::Matrix localTransform; // (dynamic) Current local transform
		DirectX::SimpleMath::Matrix finalTransform; // (dynamic) What to upload to GPU
		std::vector<int> keys;
	};

	std::vector<JointInfo> joints;
	inline static int framerate = 60;

	StructuredBuffer<DirectX::SimpleMath::Matrix> sb;

public:

	JointAnimation()
	{

		// Root do not have a parent
		joints[0].transform = joints[0].localTransform;
		joints[0].bindposeInverse = joints[0].transform.Invert();

		for (auto& joint : joints)
		{
			// joint.transform changes the base from model space
			// to the base of the bone in the current pose.
			joint.transform = joints[joint.parent].transform * joint.localTransform;

			// joint.bindposeInverse changes the base of the bone
			// IN BIND POSE to the model space.
			joint.bindposeInverse = joint.transform.Invert();
		}


		std::vector<DirectX::SimpleMath::Matrix> structVector;
		sb.Initialize(GPU::device, GPU::immediateContext, structVector);
		sb.applyData();

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
			joints[i].transform = joints[joints[i].parent].transform * joints[i].localTransform;

			// joint.finalTransform transforms a vertex in model space to the
			// currently animated pose, also in model space.
			joints[i].finalTransform = joints[i].transform * joints[i].bindposeInverse;

			sb.getData(i) = joints[i].finalTransform;
		}

		sb.applyData();

	}

	void Bind(UINT toRegister)
	{
		sb.Bind(toRegister);
	}

};