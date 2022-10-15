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

void ModelManager::aiMatrixToXMmatrix(const aiMatrix4x4& in, DirectX::XMMATRIX& out)
{
	DirectX::XMFLOAT4X4 res = 
	DirectX::XMFLOAT4X4(
		float(in.a1),
		float(in.a2),
		float(in.a3),
		float(in.a4),

		float(in.b1),
		float(in.b2),
		float(in.b3),
		float(in.b4),

		float(in.c1),
		float(in.c2),
		float(in.c3),
		float(in.c4),

		float(in.d1),
		float(in.d2),
		float(in.d3),
		float(in.d4)
	);

	out = DirectX::XMLoadFloat4x4(&res);
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
	while (vertexID >= boneDataVec.size())
	{
		boneDataVec.emplace_back();
	}
	for (int j = 0; j < 4; j++)
	{
		if (boneDataVec[vertexID].Weights[j] == 0.0)
		{
			boneDataVec[vertexID].BoneIDs[j] = boneId;
			boneDataVec[vertexID].Weights[j] = weight;
			//printf("Bone: %d weight: %f index: %i\n", boneId, weight, j);
			return;
		}
	}
	//float smallest = boneDataVec[vertexID].Weights[0];
	int indexOfSmallest = 0;
	for (int i = 1; i < 4; i++)
	{
		if (boneDataVec[vertexID].Weights[indexOfSmallest] > boneDataVec[vertexID].Weights[i])
		{
			indexOfSmallest = i;
		}
	}
	boneDataVec[vertexID].BoneIDs[indexOfSmallest] = boneId;
	boneDataVec[vertexID].Weights[indexOfSmallest] = weight;
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
		if (bone_ID == boneVec.size())
		{
			DirectX::XMMATRIX temp;
			this->aiMatrixToXMmatrix(mesh->mBones[i]->mOffsetMatrix, temp);
			DirectX::XMFLOAT4X4 offset;
			DirectX::XMStoreFloat4x4(&offset, temp);
			boneInfo bi(offset);
			boneVec.push_back(bi);
			boneVec[boneVec.size() - 1].name = boneName;
		}

		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			int vId = mesh->mBones[i]->mWeights[j].mVertexId;

			this->addBoneData(vId, bone_ID, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void ModelManager::numberBone(aiNode* node, int parentNode, DirectX::XMMATRIX& prevOffsets)
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
		this->boneVec[boneID].parentID = parentNode;

	}
	
	DirectX::XMMATRIX temp;
	this->aiMatrixToXMmatrix(node->mTransformation, temp);
	DirectX::XMMatrixMultiply(prevOffsets, temp);

	if (boneID != -1)
	{
		//this->boneVec[boneID].finalTransform = prevOffsets;
	}
	
	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->numberBone(node->mChildren[i], boneID, prevOffsets);
	}

}

int ModelManager::findAndAddBoneID(const std::string& name)
{
	int bone_ID = 0;
	std::string boneName = name;

	if (boneIndexTraslator.find(boneName) == boneIndexTraslator.end())
	{
		bone_ID = boneIndexTraslator.size();
		boneIndexTraslator.insert(std::pair<std::string, int>(boneName, bone_ID));
	}
	else
	{
		bone_ID = boneIndexTraslator[boneName];
	}
	return bone_ID;
}

int ModelManager::findBoneID(const std::string& name)
{
	int bone_ID = 0;
	std::string boneName = name;

	if (boneIndexTraslator.find(boneName) == boneIndexTraslator.end())
	{
		return -1;
	}
	else
	{
		bone_ID = boneIndexTraslator[boneName];
	}
	return bone_ID;
}

void ModelManager::readAnimations(aiScene* scene)
{
	if (scene->HasAnimations())
	{
		for (int i = 0, end = scene->mNumAnimations; i < end; i++)
		{
			this->AnimationVec.push_back(scene->mAnimations[i]);
		}
	}
}

ModelManager::ModelManager(ID3D11Device* device)
{
	this->device = device;
	ID3D11ShaderResourceView* tempSRV;
	this->makeSRV(tempSRV, "../Textures/Missing.png");
	this->bank.addSrv("Missing.png", tempSRV);
}

bool ModelManager::loadMeshData(const std::string& filePath)
{
	this->boneDataVec.clear();
	this->boneVec.clear();
	this->boneIndexTraslator.clear();

	Assimp::Importer importer;
	const aiScene* pScene = this->assimpImp.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded);

	this->scene = new aiScene();

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FBX-File!");
		return false;
	}

	processNodes(pScene->mRootNode, pScene);

	for (int i = 0; i < boneDataVec.size(); i++)
	{
		this->normalizeWeights(this->boneDataVec[i].Weights);
	}

	DirectX::XMMATRIX startMatrix = DirectX::XMMatrixIdentity();

	this->boneVec;
	int bp = 2;

	this->numberBone(pScene->mRootNode, -1, startMatrix);

	this->boneVec;
	bp = 2;

	this->scene->mAnimations = pScene->mAnimations;
	this->scene->mNumAnimations = pScene->mNumAnimations;
	this->scene->mRootNode = new aiNode(*pScene->mRootNode);

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
	if (this->scene == nullptr)
	{
		return false;
	}
	if (scene->mNumAnimations == 0)
	{
		free(this->scene);
		return false;
	}
	gameObject.addData(this->boneDataVec, this->boneVec, this->boneIndexTraslator, this->scene, this->AnimationVec, this->assimpImp);
	this->boneDataVec.clear();
	this->boneVec.clear();
	this->boneIndexTraslator.clear();
	this->scene == nullptr;
	return true;
}