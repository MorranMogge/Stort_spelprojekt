#include "ModelManager.h"
#include "GPU.h"
#include <assimp/cimport.h>
#include <iostream>

bool ModelManager::makeSRV(ID3D11ShaderResourceView*& srv, std::string finalFilePath)
{
	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC desc = {};
	int width, height, channels;
	unsigned char* image = stbi_load(finalFilePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!image)
	{
		stbi_image_free(image);
		return false;
	}
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.MiscFlags = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = image;
	data.SysMemPitch = width * 4; //RBGA - RGBA - ...

	HRESULT hr = device->CreateTexture2D(&desc, &data, &texture);
	stbi_image_free(image);
	if (FAILED(hr))
	{
		return false;
	}
	hr = device->CreateShaderResourceView(texture, NULL, &srv);
	if (FAILED(hr))
	{
		return false;
	}
	texture->Release();
	return true;
}

void ModelManager::processNodes(aiNode* node, const aiScene*& scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		if (this->meshes.size() > this->diffuseMaps.size())
		{
			this->diffuseMaps.push_back(this->bank.getSrv("Missing.png"));
		}

		aiMesh* mesh = scene->mMeshes[i];
		meshes.push_back(readNodes(mesh, scene));
		if (mesh->HasBones())
		{
			this->globalInverseTransform = scene->mRootNode->mTransformation;
			this->globalInverseTransform = this->globalInverseTransform.Inverse();
			printf("has bones \n");
			this->loadBones(scene->mMeshes[i], i);
		}

		//printf("Number of bones: %d\n number vert: %d", scene->mMeshes()->)
		aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		//här srv inläsning
		aiString Path;
		if(material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			if (this->bank.hasItem(Path.data))
			{
				this->diffuseMaps.push_back(this->bank.getSrv(Path.data));
				continue;
			};

			ID3D11ShaderResourceView* tempSRV = {};
			std::string FullPath = "../Textures/";
			FullPath.append(Path.data);
			//make srv
			if (!this->makeSRV(tempSRV, FullPath))
			{
				continue;
			}
			//give to bank
			this->bank.addSrv(Path.data, tempSRV);
			this->diffuseMaps.push_back(tempSRV);
		}
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNodes(node->mChildren[i], scene);
	}
}

Mesh2* ModelManager::readNodes(aiMesh* mesh, const aiScene*& scene)
{
	std::vector<vertex> vertexTriangle;
	std::vector<DWORD> indexTriangle;

	float smallestX = 0.0f;
	float smallestY = 0.0f;
	float smallestZ = 0.0f;

	float biggestX = 0.0f;
	float biggestY = 0.0f;
	float biggestZ = 0.0f;

	vertex vertex;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.nor.x = mesh->mNormals[i].x;
		vertex.nor.y = mesh->mNormals[i].y;
		vertex.nor.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertexTriangle.push_back(vertex);

		if (vertex.pos.x < smallestX || vertex.pos.y < smallestY || vertex.pos.z < smallestZ)
		{
			if (vertex.pos.x < smallestX) smallestX = vertex.pos.x;
			if (vertex.pos.y < smallestY) smallestY = vertex.pos.y;
			if (vertex.pos.z < smallestZ) smallestZ = vertex.pos.z;
		}

		if (vertex.pos.x > biggestX || vertex.pos.y > biggestY || vertex.pos.z > biggestZ)
		{
			if (vertex.pos.x > biggestX) biggestX = vertex.pos.x;
			if (vertex.pos.y > biggestY) biggestY = vertex.pos.y;
			if (vertex.pos.z > biggestZ) biggestZ = vertex.pos.z;
		}
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indexTriangle.push_back(face.mIndices[j]);
	}

	//smallestCoord = XMVectorSet(smallestX, smallestY, smallestZ, 0.0f);
	//biggestCoord = XMVectorSet(biggestX, biggestY, biggestZ, 0.0f);

	return new Mesh2(GPU::device, vertexTriangle, indexTriangle);
}

void ModelManager::aiMatrixToXMmatrix(const aiMatrix4x4& in, DirectX::XMFLOAT4X4& out)
{
	out = 
		//DirectX::XMFLOAT4X4(
		//	float(in.a1),
		//	float(in.a2),
		//	float(in.a3),
		//	float(in.a4),

		//	float(in.b1),
		//	float(in.b2),
		//	float(in.b3),
		//	float(in.b4),

		//	float(in.c1),
		//	float(in.c2),
		//	float(in.c3),
		//	float(in.c4),

		//	float(in.d1),
		//	float(in.d2),
		//	float(in.d3),
		//	float(in.d4)
		//);
		DirectX::XMFLOAT4X4(
		float(in.a1), float(in.b1), float(in.c1), float(in.d1),
		float(in.a2), float(in.b2), float(in.c2), float(in.d2),
		float(in.a3), float(in.b3), float(in.c3), float(in.d3),
		float(in.a4), float(in.b4), float(in.c4), float(in.d4));
}

void ModelManager::normalizeWeights(float weights[])
{
	float totalWeight = 0;
	totalWeight += weights[0];
	totalWeight += weights[1];
	totalWeight += weights[2];
	totalWeight += weights[3];

	float divFac = 1 / totalWeight;
	weights[0] *= divFac;
	weights[1] *= divFac;
	weights[2] *= divFac;
	weights[3] *= divFac;

}

void ModelManager::addBoneData(const int vertexID, const int boneId, const float weight)
{
	while (vertexID >= this->aniData.boneDataVec.size())
	{
		this->aniData.boneDataVec.emplace_back();
	}
	for (int j = 0; j < 4; j++)
	{
		if (this->aniData.boneDataVec[vertexID].Weights[j] == 0.0)
		{
			this->aniData.boneDataVec[vertexID].BoneIDs[j] = boneId;
			this->aniData.boneDataVec[vertexID].Weights[j] = weight;
			//printf("Bone: %d weight: %f index: %i\n", boneId, weight, j);
			return;
		}
	}
	//float smallest = boneDataVec[vertexID].Weights[0];
	int indexOfSmallest = 0;
	for (int i = 1; i < 4; i++)
	{
		if (this->aniData.boneDataVec[vertexID].Weights[indexOfSmallest] > this->aniData.boneDataVec[vertexID].Weights[i])
		{
			indexOfSmallest = i;
		}
	}
	this->aniData.boneDataVec[vertexID].BoneIDs[indexOfSmallest] = boneId;
	this->aniData.boneDataVec[vertexID].Weights[indexOfSmallest] = weight;
	//norm
	//assert(0);
}

void ModelManager::loadBones(const aiMesh* mesh, const int mesh_index)
{
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		if (i == 2)
		{
			int bp = 2;
		}
		std::string boneName = (mesh->mBones[i]->mName.data);
		int bone_ID = this->findAndAddBoneID(boneName);
		if (bone_ID == this->aniData.boneVector.size())
		{
			//new bone
			DirectX::XMFLOAT4X4 offset;
			this->aiMatrixToXMmatrix(mesh->mBones[i]->mOffsetMatrix, offset);
			boneInfo bi(offset);
			this->aniData.boneVector.push_back(bi);
			this->aniData.boneVector[this->aniData.boneVector.size() - 1].name = boneName;
		}

		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			int vId = mesh->mBones[i]->mWeights[j].mVertexId;

			this->addBoneData(vId, bone_ID, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void ModelManager::numberBone(aiNode* node, int parentNode, const DirectX::XMFLOAT4X4& prevOffsets)
{
	int boneID = this->findBoneID(node->mName.data);
	bool realNode = true;
	if (boneID == -1)
	{
		realNode = false;
		boneID = parentNode;
	}
	if (realNode)
	{
		this->aniData.boneVector[boneID].parentID = parentNode;

	}
	
	DirectX::XMFLOAT4X4 floatMatrix;
	this->aiMatrixToXMmatrix(node->mTransformation, floatMatrix);
	DirectX::XMMATRIX tempMatrix = DirectX::XMLoadFloat4x4(&floatMatrix);
	DirectX::XMMATRIX prevNodeOffMatrix = DirectX::XMLoadFloat4x4(&prevOffsets);
	tempMatrix = DirectX::XMMatrixMultiply(prevNodeOffMatrix, tempMatrix);
	DirectX::XMStoreFloat4x4(&floatMatrix, tempMatrix);

	if (boneID != -1)
	{
		this->aniData.boneVector[boneID].offsetMatrix = floatMatrix;
	}
	
	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->numberBone(node->mChildren[i], boneID, floatMatrix);
	}

}

int ModelManager::findAndAddBoneID(const std::string& name)
{
	int bone_ID = 0;
	std::string boneName = name;

	if (this->aniData.boneNameToIndex.find(boneName) == this->aniData.boneNameToIndex.end())
	{
		bone_ID = this->aniData.boneNameToIndex.size();
		this->aniData.boneNameToIndex.insert(std::pair<std::string, int>(boneName, bone_ID));
	}
	else
	{
		bone_ID = this->aniData.boneNameToIndex[boneName];
	}
	return bone_ID;
}

int ModelManager::findBoneID(const std::string& name)
{
	int bone_ID = 0;
	std::string boneName = name;

	if (this->aniData.boneNameToIndex.find(boneName) == this->aniData.boneNameToIndex.end())
	{
		return -1;
	}
	else
	{
		bone_ID = this->aniData.boneNameToIndex[boneName];
	}
	return bone_ID;
}

void ModelManager::recParseNodes(nodes* node, const aiNode* ainode)
{
	node->nodeName = ainode->mName.C_Str();
	this->aiMatrixToXMmatrix(ainode->mTransformation, node->trasformation);
	if(ainode->mNumChildren > 0)
	{
		node->children.reserve(ainode->mNumChildren);
		if (ainode->mNumChildren == 3)
		{
			int bp = 2;
		}
		for (int i = 0, end = ainode->mNumChildren; i < end; i++)
		{
			node->children.push_back(new nodes);
			node->children[i]->parent = node;
			recParseNodes(node->children[i], ainode->mChildren[i]);
		}
	}
}

void ModelManager::parseNode(const aiScene* scene)
{
	this->aniData.rootNode = new nodes;
	this->aniData.rootNode->parent = nullptr;
	
	this->aniData.rootNode->nodeName = scene->mRootNode->mName.C_Str();
	this->aiMatrixToXMmatrix(scene->mRootNode->mTransformation, this->aniData.rootNode->trasformation);

	aniData.rootNode->children.reserve(scene->mRootNode->mNumChildren);
	for (size_t i = 0; i < scene->mRootNode->mNumChildren; i++)
	{
		aniData.rootNode->children.push_back(new nodes);
		aniData.rootNode->children[aniData.rootNode->children.size() - 1]->parent = aniData.rootNode;
		recParseNodes(aniData.rootNode->children[aniData.rootNode->children.size() - 1], scene->mRootNode->mChildren[i]);
	}
}

void ModelManager::parseAnimation(const aiScene* scene)
{
	this->aniData.animation.reserve(scene->mNumAnimations);
	for (int i = 0, end = scene->mNumAnimations; i < end; i++)
	{
		this->aniData.animation.emplace_back();
		this->aniData.animation[i].mName = scene->mAnimations[i]->mName.C_Str();
		this->aniData.animation[i].duration = scene->mAnimations[i]->mDuration;
		this->aniData.animation[i].ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;

		this->aniData.animation[i].mChannels.reserve(scene->mAnimations[i]->mNumChannels);
		for (int j = 0, length = scene->mAnimations[i]->mNumChannels; j < length; j++)
		{
			this->aniData.animation[i].mChannels.emplace_back();
			this->aniData.animation[i].mChannels[j].mNodeName = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

			this->aniData.animation[i].mChannels[j].posKeyFrames.reserve(scene->mAnimations[i]->mChannels[j]->mNumPositionKeys);
			this->aniData.animation[i].mChannels[j].rotKeyFrame.reserve(scene->mAnimations[i]->mChannels[j]->mNumRotationKeys);
			this->aniData.animation[i].mChannels[j].scalKeyFrames.reserve(scene->mAnimations[i]->mChannels[j]->mNumScalingKeys);

			for (int k = 0, length = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k < length; k++)
			{
				this->aniData.animation[i].mChannels[j].posKeyFrames.emplace_back();
				this->aniData.animation[i].mChannels[j].posKeyFrames[k].Time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
				this->aniData.animation[i].mChannels[j].posKeyFrames[k].addAiVector3D(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue);
			}
			for (int k = 0, length = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k < length; k++)
			{
				this->aniData.animation[i].mChannels[j].rotKeyFrame.emplace_back();
				this->aniData.animation[i].mChannels[j].rotKeyFrame[k].Time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
				this->aniData.animation[i].mChannels[j].rotKeyFrame[k].addAiQuaternion(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue);
			}
			for (int k = 0, length = scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k < length; k++)
			{
				this->aniData.animation[i].mChannels[j].scalKeyFrames.emplace_back();
				this->aniData.animation[i].mChannels[j].scalKeyFrames[k].Time = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
				this->aniData.animation[i].mChannels[j].scalKeyFrames[k].addAiVector3D(scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue);
			}
		}
	}
}

//void ModelManager::readAnimations(aiScene* scene)
//{
//	if (scene->HasAnimations())
//	{
//		for (int i = 0, end = scene->mNumAnimations; i < end; i++)
//		{
//			this->AnimationVec.push_back(scene->mAnimations[i]);
//		}
//	}
//}

ModelManager::ModelManager(ID3D11Device* device)
{
	this->device = device;
	ID3D11ShaderResourceView* tempSRV;
	this->makeSRV(tempSRV, "../Textures/Missing.png");
	this->bank.addSrv("Missing.png", tempSRV);
}

bool ModelManager::loadMeshData(const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FBX-File!");
		return false;
	}

	processNodes(pScene->mRootNode, pScene);

	for (int i = 0; i < this->aniData.boneDataVec.size(); i++)
	{
		this->normalizeWeights(this->aniData.boneDataVec[i].Weights);
	}
	
	this->parseNode(pScene);
	this->parseAnimation(pScene);
	aniData.rootNode;
	aniData.animation;

	DirectX::XMMATRIX startMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 startFloat;
	DirectX::XMStoreFloat4x4(&startFloat, startMatrix);
	this->aniData.boneVector;
	int bp = 2;

	this->numberBone(pScene->mRootNode, -1, startFloat);

	this->aniData.boneVector;
	bp = 2;

	return true;
}

std::vector<Mesh2*> ModelManager::getMeshes() const
{
	return meshes;
}

std::vector<ID3D11ShaderResourceView*> ModelManager::getTextureMaps() const
{
	return this->diffuseMaps;
}

bool ModelManager::loadMeshAndBoneData(const std::string& filePath, AnimatedMesh& gameObject)
{
	this->loadMeshData(filePath);
	this->aniData.boneVector;
	for (int i = 0, end = this->aniData.boneVector.size(); i < end; i++)
	{
		std::cout << "Bone: " << this->aniData.boneVector[i].name << "\n	"
			<< this->aniData.boneVector[i].offsetMatrix._11 << " " << this->aniData.boneVector[i].offsetMatrix._12 << " " << this->aniData.boneVector[i].offsetMatrix._13 << " " << this->aniData.boneVector[i].offsetMatrix._14 << "\n	"
			<< this->aniData.boneVector[i].offsetMatrix._21 << " " << this->aniData.boneVector[i].offsetMatrix._22 << " " << this->aniData.boneVector[i].offsetMatrix._23 << " " << this->aniData.boneVector[i].offsetMatrix._24 << "\n	"
			<< this->aniData.boneVector[i].offsetMatrix._31 << " " << this->aniData.boneVector[i].offsetMatrix._32 << " " << this->aniData.boneVector[i].offsetMatrix._33 << " " << this->aniData.boneVector[i].offsetMatrix._34 << "\n	"
			<< this->aniData.boneVector[i].offsetMatrix._41 << " " << this->aniData.boneVector[i].offsetMatrix._42 << " " << this->aniData.boneVector[i].offsetMatrix._43 << " " << this->aniData.boneVector[i].offsetMatrix._44 << "\n";
	}
	this->aniData.boneDataVec;
	//gameObject.addData(this->boneDataVec, this->boneVec, this->boneIndexTraslator, this->scene, this->AnimationVec, this->assimpImp);

	return true;
}