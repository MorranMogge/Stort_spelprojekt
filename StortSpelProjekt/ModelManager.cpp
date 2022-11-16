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
		/*if (this->meshes.size() > this->diffuseMaps.size())
		{
			this->diffuseMaps.push_back(this->bank.getSrv("Missing.png"));
		}*/

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];		
		
		readNodes(mesh, scene);

		aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		//här srv inläsning

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			
		}

		aiString diffuseName;
		material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), diffuseName);
		


		//få mat_key
		aiColor3D color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);



		aiString Path;
		


		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		std::cout << name.C_Str() << "\n";
		if (bank.hasMaterial(name.C_Str()) == false)
		{
			//ändra till dynamiskt eller 
			MaterialS materialToAdd;
			
			material->Get(AI_MATKEY_COLOR_AMBIENT, color);
			materialToAdd.ambient = DirectX::SimpleMath::Vector4{ color.r,color.g,color.b, 0};

			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			materialToAdd.diffuse = DirectX::SimpleMath::Vector4{ color.r, color.g, color.b, 0 };

			material->Get(AI_MATKEY_COLOR_SPECULAR, color);
			materialToAdd.specular = DirectX::SimpleMath::Vector3{ color.r, color.g, color.b };

			material->Get(AI_MATKEY_SHININESS, color);
			//materialToAdd.specularPower = (float)(color.r + color.g + color.b);
			materialToAdd.specularPower = (float)color.r;
			ConstantBuffer cbuffer(&materialToAdd, sizeof(MaterialS));
			
			bank.addMaterial(name.C_Str(), &cbuffer);


			//testar
			//ConstantBuffer* tmp = bank.getMaterial("default", nullptr);
			//tmp->Get();
			//tmp->GetAddressOf();
			//tmp->BindToPS(0u);
			

		}
		

	
		
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);

		/*std::cout << "AMBIENT:\n" << color.r << "\n";
		std::cout << color.g << "\n";
		std::cout << color.b << "\n";*/

		//få mat_key namn

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

		/*std::cout << "DIFFUSE:\n" << color.r << "\n";
		std::cout << color.g << "\n";
		std::cout << color.b << "\n";*/

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		/*std::cout << "Specular:\n" << color.r << "\n";
		std::cout << color.g << "\n";
		std::cout << color.b << "\n";*/


		material->Get(AI_MATKEY_SHININESS, color);
		/*std::cout << "Shinyness:\n" << color.r << "\n";
		std::cout << color.g << "\n";
		std::cout << color.b << "\n";*/


		
		if (material->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::cout << Path.C_Str() << "\n";
		}

		//if (material->GetTexture(aiTextureType_, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)

		if (material->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			if (this->bank.hasItem(Path.data))
			{
				this->diffuseMaps.emplace_back(this->bank.getSrv(Path.data));
				continue;
			};

			std::cout << Path.C_Str() << "\n";
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


		if(material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			if (this->bank.hasItem(Path.data))
			{
				this->diffuseMaps.emplace_back(this->bank.getSrv(Path.data));
				continue;
			};

			std::cout << Path.C_Str() << "\n";
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


			//vertex.tangent.x = mesh->mTangents[i].x;
			//vertex.tangent.x = mesh->mTangents[i].x;
			//vertex.tangent.y = mesh->mTangents[i].y;
			
			vertex.tangent.y = mesh->mBitangents[i].y;
			vertex.tangent.z = mesh->mBitangents[i].z;
			vertex.tangent.z = mesh->mBitangents[i].z;


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

void ModelManager::setDevice(ID3D11Device* device)
{
	this->device = device;
}

ModelManager::ModelManager()
	:device(nullptr)
{

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

	//skapa indexBuffer
	ID3D11Buffer* indexBuffer = {};

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * dataForMesh.indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
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
	return bank.getSrv(key);
}

std::vector<ID3D11ShaderResourceView*> ModelManager::getTextureMaps() const
{
	return this->diffuseMaps;
}

bool ModelManager::getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces)
{
	return bank.getIndexMeshBuffers(filePath, indexBuffer, vertexBuffer, submeshRanges, amountOfVertces);
}

ConstantBuffer* ModelManager::getMaterialData(const std::string& filePath, ConstantBuffer*& constantBuff)
{
	if (bank.hasItem(filePath))
	{
		return bank.getMaterial(filePath, constantBuff);
	}

	return nullptr;
}
