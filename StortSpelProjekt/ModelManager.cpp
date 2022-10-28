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

void ModelManager::processNodes(aiNode* node, const aiScene* scene, const std::string& filePath)
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

		if (mesh->HasBones())
		{

			this->aiMatrixToXMmatrix(scene->mRootNode->mTransformation, this->aniData.globalInverseTransform);
			DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&this->aniData.globalInverseTransform);
			temp = DirectX::XMMatrixInverse(nullptr, temp);
			DirectX::XMStoreFloat4x4(&this->aniData.globalInverseTransform, temp);

			this->loadBones(scene->mMeshes[i], i);
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
	

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.nor.x = mesh->mNormals[i].x;
		vertex.nor.y = mesh->mNormals[i].y;
		vertex.nor.z = mesh->mNormals[i].z;

		//vertex.tangent.x = mesh->mTangents[i].x;
		//vertex.tangent.y = mesh->mTangents[i].y;
		//vertex.tangent.z = mesh->mTangents[i].z;
		


		if (mesh->mTextureCoords[0])
		{
			vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertexTriangle.emplace_back(vertex);
		dataForMesh.vertexTriangle.emplace_back(vertex);
	}
	
	/*for (int i = 0; i < dataForMesh.vertexTriangle.size(); i += 3)
	{
		DirectX::XMFLOAT2 UV1 = dataForMesh.vertexTriangle[i].uv;
		DirectX::XMFLOAT2 UV2 = dataForMesh.vertexTriangle[i+1].uv;
		DirectX::XMFLOAT2 UV3 = dataForMesh.vertexTriangle[i+2].uv;

		DirectX::XMFLOAT3 Pos1 = dataForMesh.vertexTriangle[i].pos;
		DirectX::XMFLOAT3 Pos2 = dataForMesh.vertexTriangle[i+1].pos;
		DirectX::XMFLOAT3 Pos3 = dataForMesh.vertexTriangle[i+2].pos;

		DirectX::SimpleMath::Vector2 dAB = UV2;
		dAB.x - UV1.x;
		dAB.y - UV1.y;
		
		DirectX::SimpleMath::Vector2 dAC = UV3;
		dAC.x - UV1.x;
		dAC.y - UV1.y;

		DirectX::XMFLOAT3 edge1 = Pos2;
		edge1.x - Pos1.x;
		edge1.y - Pos1.y;
		edge1.z - Pos1.z;

		DirectX::XMFLOAT3 edge2 = Pos3;
		edge2.x - Pos1.x;
		edge2.y - Pos1.y;
		edge2.z - Pos1.z;

		float f = 1.0f / (dAB.x * dAC.y - dAC.x * dAB.y);

		DirectX::XMFLOAT3 tangent;
		tangent.x = f * (dAC.y * edge1.x - dAB.y * edge2);
	}*/


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

ModelManager::~ModelManager()
{
	
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

bool ModelManager::loadBoneData(const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FBX-File!");
		return false;
	}

	this->aiMatrixToXMmatrix(pScene->mRootNode->mTransformation, this->aniData.globalInverseTransform);
	DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&this->aniData.globalInverseTransform);
	temp = DirectX::XMMatrixInverse(nullptr, temp);
	DirectX::XMStoreFloat4x4(&this->aniData.globalInverseTransform, temp);

	aiMesh* mesh;
	for (int i = 0, length = pScene->mNumMeshes; i < length; i++)
	{
		mesh = pScene->mMeshes[i];
		if (mesh->HasBones())
		{
			for (int j = 0, length = mesh->mNumBones; j < length; j++)
			{
				{
					std::cout << "Bone: " << mesh->mBones[j]->mName.C_Str() << "\n	"
						<< mesh->mBones[j]->mOffsetMatrix.a1 << " " << mesh->mBones[j]->mOffsetMatrix.a2 << " " << mesh->mBones[j]->mOffsetMatrix.a3 << " " << mesh->mBones[j]->mOffsetMatrix.a4 << "\n	"
						<< mesh->mBones[j]->mOffsetMatrix.b1 << " " << mesh->mBones[j]->mOffsetMatrix.b2 << " " << mesh->mBones[j]->mOffsetMatrix.b3 << " " << mesh->mBones[j]->mOffsetMatrix.b4 << "\n	"
						<< mesh->mBones[j]->mOffsetMatrix.c1 << " " << mesh->mBones[j]->mOffsetMatrix.c2 << " " << mesh->mBones[j]->mOffsetMatrix.c3 << " " << mesh->mBones[j]->mOffsetMatrix.c4 << "\n	"
						<< mesh->mBones[j]->mOffsetMatrix.d1 << " " << mesh->mBones[j]->mOffsetMatrix.d2 << " " << mesh->mBones[j]->mOffsetMatrix.d3 << " " << mesh->mBones[j]->mOffsetMatrix.d4 << "\n";
				}
			}


			printf("has bones \n");

			this->loadBones(pScene->mMeshes[i], i);
		}
	}

	processNodes(pScene->mRootNode, pScene, filePath);
	return true;
}

ID3D11ShaderResourceView* ModelManager::getSrv(const std::string key)
{
	return bank.getSrv(key);
}

std::vector<ID3D11ShaderResourceView*> ModelManager::getTextureMaps() const
{
	return this->diffuseMaps;
}

bool ModelManager::loadMeshAndBoneData(const std::string& filePath)
{
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
	parseNode(pScene);
	parseAnimation(pScene);

	this->aniData.boneVector;
	this->aniData.boneDataVec;
	
	std::vector<AnimatedVertex> vertexAVec;
	vertexAVec.reserve(this->dataForMesh.vertexTriangle.size());
	AnimatedVertex tempVertex;
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

	/*ID3D11Buffer* vertexBuffer = {};

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

	skapa indexBuffer
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
	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);*/

	//bank.addMeshBuffers(filePath, vertexBuffer, indexBuffer, submeshRanges, amountOfvertices);

	//indexBuffer = {};
	//vertexBuffer = {};
	//this->submeshRanges.clear();
	//this->amountOfvertices.clear();
	//this->dataForMesh.indexTriangle.clear();
	//this->dataForMesh.vertexTriangle.clear();

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
