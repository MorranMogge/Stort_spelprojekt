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

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		//meshes.push_back(readNodes(mesh, scene));
		


		//för testing purpose - Elliot tar bort
		
		
		meshes.emplace_back(readNodes(mesh, scene));
		
		readNodes2(mesh, scene);


		//Mesh2* tmpData = readNodes(mesh, scene);
		//bank.addMeshBuffers(filePath, tmpData->getVertexBuffer(), tmpData->getIndexBuffer());

		aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		//här srv inläsning

		aiString Path;
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

Mesh2* ModelManager::readNodes(aiMesh* mesh, const aiScene* scene)
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

		vertexTriangle.emplace_back(vertex);
		//dataForMesh.vertexTriangle.emplace_back(vertex);

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
		{
			indexTriangle.emplace_back(face.mIndices[j]);
			//dataForMesh.indexTriangle.emplace_back(face.mIndices[j]);
		}
	}
	
	
	std::cout << "Old way mesh vertex count: " << vertexTriangle.size() << "\n";
	std::cout << "Old way mesh index count: " << indexTriangle.size() << "\n";
	
	return new Mesh2(GPU::device, vertexTriangle, indexTriangle);
}

void ModelManager::readNodes2(aiMesh* mesh, const aiScene* scene)
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

		vertexTriangle.emplace_back(vertex);
		dataForMesh.vertexTriangle.emplace_back(vertex);

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
		{
			indexTriangle.emplace_back(face.mIndices[j]);
			dataForMesh.indexTriangle.emplace_back(face.mIndices[j]);
		}
	}
	

	ID3D11Buffer* indexBuffer = {};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indexTriangle.data();
	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);


	this->vecIndexBuff.emplace_back(indexBuffer);

	std::cout << "StructVertexDataSize: " << dataForMesh.vertexTriangle.size() << "\n";
	std::cout << "StructIndexDataSize: " << dataForMesh.indexTriangle.size() << "\n";

	submeshRanges.emplace_back(dataForMesh.indexTriangle.size());

	for (int i = 0; i < submeshRanges.size(); i++)
	{
		std::cout << "submeshranges on index: " << i << " with range " << submeshRanges[i] << "\n";
	}

}

std::vector<ID3D11Buffer*> ModelManager::getBuff() const
{
	std::cout << this->vecIndexBuff.size() << "här\n";

	return this->vecIndexBuff;
}

ModelManager::ModelManager(ID3D11Device* device)
{
	this->device = device;
	ID3D11ShaderResourceView* tempSRV;
	this->makeSRV(tempSRV, "../Textures/Missing.png");
	this->bank.addSrv("Missing.png", tempSRV);
}

ModelManager::~ModelManager()
{
	/*for (int i = 0; i < meshes.size(); i++)
	{
		delete this->meshes[i];
	}*/
}

bool ModelManager::loadMeshData(const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded | aiProcess_PreTransformVertices);

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FBX-File!");
		return false;
	}

	if (bank.hasItem(filePath) == true)
	{
		return false;
	}

	//processNodes(pScene->mRootNode, pScene, filePath);

	processNodes(pScene->mRootNode, pScene, filePath);
	

	//skapa vertexBuffer

	ID3D11Buffer* vertexBuffer = {};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(vertex) * dataForMesh.vertexTriangle.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = dataForMesh.vertexTriangle.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	

	//std::cout << dataForMesh.vertexTriangle.size() << "\n";

	//skapa indexBuffer

	ID3D11Buffer* indexBuffer = {};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * dataForMesh.indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	
	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = dataForMesh.indexTriangle.data();
	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);


	//ändra i banken till vertexBuffer först och indexBuffer som tredje parametern.
	
	bank.addMeshBuffers(filePath, vertexBuffer, indexBuffer, submeshRanges);

	indexBuffer = {};
	vertexBuffer = {};
	submeshRanges.clear();
	memset(&dataForMesh, 0, sizeof(dataForMesh));

	//bank.getIndexMeshBuffers(filePath, inde)

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

bool ModelManager::getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges)
{
	return bank.getIndexMeshBuffers(filePath, indexBuffer, vertexBuffer, submeshRanges);
}
