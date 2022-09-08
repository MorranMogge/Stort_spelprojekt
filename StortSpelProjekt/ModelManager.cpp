#include "ModelManager.h"
#include "GPU.h"
#include <assimp/cimport.h>


#include <iostream>

void ModelManager::processNodes(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(readNodes(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNodes(node->mChildren[i], scene);
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

ModelManager::ModelManager()
{

}

bool ModelManager::loadMeshData(const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded | aiProcess_PreTransformVertices);

	if (pScene == nullptr)
	{
		ErrorLog::Log("Cant read FTX-File!");
		return false;
	}

	processNodes(pScene->mRootNode, pScene);
	return true;
}

std::vector<Mesh2*> ModelManager::getMeshes() const
{
	return meshes;
}

//void ModelManager::createSRV()
//{
//}
//
//void ModelManager::createIndexBuffer()
//{
//}
//
//void ModelManager::createVertexBuffer()
//{
//}
