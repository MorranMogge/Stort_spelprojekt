#include"stdafx.h"
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

void ModelManager::processNodes(aiNode* node, const aiScene* scene, const std::string& filePath, bool wantToBone)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		if (this->meshes.size() > this->diffuseMaps.size())
		{
			this->diffuseMaps.push_back(this->bank.getSrv("Missing.png"));
		}

		//printf("Number of bones: %d\n number vert: %d", scene->mMeshes()->)
		aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		//h�r srv inl�sning
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];		

		if (mesh->HasBones() && wantToBone)
		{
			this->loadBones(mesh, VertexOffset);
			this->VertexOffset += mesh->mNumVertices;
			this->aniData.boneDataVec;
			int pb = 2;
		}
		
		readNodes(mesh, scene);

		//h�r srv inl�sning

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			
		}

		aiString diffuseName;
		material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), diffuseName);
		
		std::cout << diffuseName.C_Str() << "\n";
		aiString Path;
		//if(material->GetTexture(aiTextureType_AMBIENT, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		if(material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			if (this->bank.hasItem(Path.data))
			{
				this->diffuseMaps.emplace_back(this->bank.getSrv(Path.data));
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
			this->diffuseMaps.emplace_back(tempSRV);
		}
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNodes(node->mChildren[i], scene, filePath);
	}
}

void ModelManager::readNodes(aiMesh* mesh, const aiScene* scene)
{
	std::vector<vertex> vertexTriangle;
	std::vector<DWORD> indexTriangle;

	vertex vertex;
	if (mesh->HasTangentsAndBitangents())
	{
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

			vertexTriangle.emplace_back(vertex);
			dataForMesh.vertexTriangle.emplace_back(vertex);
		}
	}
	else
	{
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

			vertexTriangle.emplace_back(vertex);
			dataForMesh.vertexTriangle.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indexTriangle.emplace_back(face.mIndices[j]);
			dataForMesh.indexTriangle.emplace_back(face.mIndices[j]);
		}
	}

	this->submeshRanges.emplace_back(indexTriangle.size());
	this->amountOfvertices.emplace_back(vertexTriangle.size());
}

std::vector<ID3D11Buffer*> ModelManager::getBuff() const
{
	return this->vecIndexBuff;
}

void ModelManager::setDevice(ID3D11Device* device)
{
	this->device = device;
}

ModelManager::ModelManager()
	:device(nullptr)
{
	this->VertexOffset = 0;

}

void ModelManager::aiMatrixToXMmatrix(const aiMatrix4x4& in, DirectX::XMFLOAT4X4& out)
{
	out = 
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

static std::unordered_map<unsigned, int> s_vertex_indices;
static unsigned int vertex_index_min = 999999999;
static unsigned int vertex_index_max = 0;
void ModelManager::loadBones(const aiMesh* mesh, const int vertexOffset)
{
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		std::string boneName = (mesh->mBones[i]->mName.data);
		int bone_ID = this->findAndAddBoneID(boneName);
		if (bone_ID == this->aniData.boneVector.size())
		{
			//new bone
			DirectX::XMFLOAT4X4 offset;
			this->aiMatrixToXMmatrix(mesh->mBones[i]->mOffsetMatrix, offset);
			boneInfo bi(offset);
			bi.name = boneName;
			this->aniData.boneVector.push_back(bi);
		}

		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			int vId = mesh->mBones[i]->mWeights[j].mVertexId;
			vId += vertexOffset;
			this->addBoneData(vId, bone_ID, mesh->mBones[i]->mWeights[j].mWeight);

			if (s_vertex_indices.find(mesh->mBones[i]->mWeights[j].mVertexId) != s_vertex_indices.end())
				s_vertex_indices[mesh->mBones[i]->mWeights[j].mVertexId] = 0;
			++s_vertex_indices[mesh->mBones[i]->mWeights[j].mVertexId];

			vertex_index_min = mesh->mBones[i]->mWeights[j].mVertexId < vertex_index_min ? mesh->mBones[i]->mWeights[j].mVertexId : vertex_index_min;
			vertex_index_max = mesh->mBones[i]->mWeights[j].mVertexId > vertex_index_max ? mesh->mBones[i]->mWeights[j].mVertexId : vertex_index_max;
		}
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

void ModelManager::recParseNodes(nodes& node, const aiNode* ainode)
{
	node.nodeName = ainode->mName.C_Str();
	this->aiMatrixToXMmatrix(ainode->mTransformation, node.trasformation);
	if(ainode->mNumChildren > 0)
	{
		node.children.reserve(ainode->mNumChildren);
		if (ainode->mNumChildren == 3)
		{
			int bp = 2;
		}
		for (int i = 0, end = ainode->mNumChildren; i < end; i++)
		{
			node.children.emplace_back();
			recParseNodes(node.children[i], ainode->mChildren[i]);
		}
	}
}

void ModelManager::parseNode(const aiScene* scene)
{
	this->aniData.rootNode;
	
	this->aniData.rootNode.nodeName = scene->mRootNode->mName.C_Str();
	this->aiMatrixToXMmatrix(scene->mRootNode->mTransformation, this->aniData.rootNode.trasformation);

	aniData.rootNode.children.reserve(scene->mRootNode->mNumChildren);
	for (size_t i = 0; i < scene->mRootNode->mNumChildren; i++)
	{
		aniData.rootNode.children.emplace_back();
		recParseNodes(aniData.rootNode.children[aniData.rootNode.children.size() - 1], scene->mRootNode->mChildren[i]);
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


ModelManager::ModelManager(ID3D11Device* device)
{
	this->VertexOffset = 0;
	this->device = device;
	ID3D11ShaderResourceView* tempSRV;
	this->makeSRV(tempSRV, "../Textures/Missing.png");
	this->bank.addSrv("Missing.png", tempSRV);
}

ModelManager::~ModelManager()
{
	
}

bool ModelManager::loadMeshData(const std::string& filePath)
{
	this->VertexOffset = 0;
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FBX-File!");
		return false;
	}

	if (bank.hasItem(filePath) == true)
	{
		return false;
	}

	
	processNodes(pScene->mRootNode, pScene, filePath);
	
	//skapa vertexBuffer

	ID3D11Buffer* vertexBuffer = {};

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(vertex) * dataForMesh.vertexTriangle.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = dataForMesh.vertexTriangle.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	//skapa indexBuffer
	ID3D11Buffer* indexBuffer = {};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * dataForMesh.indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = dataForMesh.indexTriangle.data();
	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	bank.addMeshBuffers(filePath, vertexBuffer, indexBuffer, submeshRanges, amountOfvertices);

	indexBuffer = {};
	vertexBuffer = {};
	this->submeshRanges.clear();
	this->amountOfvertices.clear();
	this->dataForMesh.indexTriangle.clear();
	this->dataForMesh.vertexTriangle.clear();

	return true;
}

ID3D11ShaderResourceView* ModelManager::getSrv(const std::string key)
{
	ID3D11ShaderResourceView* empty = {};
	ID3D11ShaderResourceView* ret = bank.getSrv(key);
	if (ret == empty)
	{
		this->makeSRV(ret, key);
		if (ret == empty)
		{
			return nullptr;
		}
		this->bank.addSrv(key, ret);
	}
	return ret;
}

std::vector<ID3D11ShaderResourceView*> ModelManager::getTextureMaps() const
{
	return this->diffuseMaps;
}

bool ModelManager::loadMeshAndBoneData(const std::string& filePath)
{
	if (bank.hasItem(filePath))
	{
		return false;
	}

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate);

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FBX-File!");
		return false;
	}
	this->VertexOffset = 0;
	this->aniData = {};

	processNodes(pScene->mRootNode, pScene, filePath, true);
	for (int i = 0, end = this->aniData.boneDataVec.size(); i < end; i++)
	{
		this->normalizeWeights(this->aniData.boneDataVec[i].Weights);
	}
	parseNode(pScene);
	parseAnimation(pScene);

	DirectX::XMFLOAT4X4 temp;
	DirectX::XMStoreFloat4x4(&temp, DirectX::XMMatrixIdentity());
	
	std::vector<AnimatedVertex> vertexAVec;
	vertexAVec.reserve(this->dataForMesh.vertexTriangle.size());
	AnimatedVertex tempVertex;
	
	auto mesh1 = pScene->mMeshes[0];
	auto mesh2 = pScene->mMeshes[1];

	for (int i = 0, end = this->dataForMesh.vertexTriangle.size(); i < end; i++)
	{
		tempVertex.pos = this->dataForMesh.vertexTriangle[i].pos;
		tempVertex.uv = this->dataForMesh.vertexTriangle[i].uv;
		tempVertex.nor = this->dataForMesh.vertexTriangle[i].nor;

		tempVertex.boneId = DirectX::XMINT4
		(this->aniData.boneDataVec[i].BoneIDs[0], this->aniData.boneDataVec[i].BoneIDs[1], this->aniData.boneDataVec[i].BoneIDs[2], this->aniData.boneDataVec[i].BoneIDs[3]);
		tempVertex.weights = DirectX::XMFLOAT4
		(this->aniData.boneDataVec[i].Weights[0], this->aniData.boneDataVec[i].Weights[1], this->aniData.boneDataVec[i].Weights[2], this->aniData.boneDataVec[i].Weights[3]);

		vertexAVec.push_back(tempVertex);
	}

	ID3D11Buffer* vertexBuffer = {};

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(AnimatedVertex) * vertexAVec.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertexAVec.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT HR = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(HR))
	{
		int bp = 2;
	}

	//skapa indexBuffer
	ID3D11Buffer* indexBuffer = {};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * dataForMesh.indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = dataForMesh.indexTriangle.data();
	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);

	bank.addAnimationData(filePath, vertexBuffer, indexBuffer, submeshRanges, amountOfvertices, this->aniData);

	indexBuffer = {};
	vertexBuffer = {};
	this->submeshRanges.clear();
	this->amountOfvertices.clear();
	this->dataForMesh.indexTriangle.clear();
	this->dataForMesh.vertexTriangle.clear();

	int bp = 2;
	return true;
}

bool ModelManager::getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces)
{
	return bank.getIndexMeshBuffers(filePath, indexBuffer, vertexBuffer, submeshRanges, amountOfVertces);
}

bool ModelManager::getAnimData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces, AnimationData& AnimData)
{
	this->bank.getAnimData(filePath, indexBuffer, vertexBuffer, submeshRanges, amountOfVertces, aniData);
	AnimData = aniData;
	return	true;
}
