#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "OBJ.h"

#include "GPU.h"
#include "MaterialLibrary.h"

#include "Vertex.h"
#include "Bound.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"


class Mesh
{

private:

	std::vector<unsigned int> submeshVerCounts;
	struct vertex
	{
		DirectX::XMFLOAT3 pos; // Position
		DirectX::XMFLOAT2 uv; // UV coordination
		DirectX::XMFLOAT3 nor; // Normal

		vertex() {
			pos = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
			uv = DirectX::XMFLOAT2{ 0.0f,0.0f };
			nor = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		};
		vertex(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& uv, DirectX::XMFLOAT3& nor) : pos(pos), uv(uv), nor(nor) {};
	};
	DirectX::XMFLOAT4X4 matrix;
public:

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	ID3D11Buffer* vertexBuff = nullptr;
	ID3D11Buffer* indexBuff = nullptr;
	std::vector<int> submeshRanges;
	std::vector<int> amountOfVertices;


	ConstantBuffer worldCB;
	ConstantBuffer positionCB;

	std::vector<std::string> matKey;

	DirectX::SimpleMath::Vector3 position;
	DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
	DirectX::SimpleMath::Vector3 scale{ 1.0f, 1.0f, 1.0f };

	Bound bound;

	Mesh() { DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity()); }
	Mesh(OBJ& obj)
	{
		Load(obj);
	}
	Mesh(ID3D11Buffer* vertexBuff, ID3D11Buffer* indexBuff, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertices)
	{
		//CalcBound();
		this->amountOfVertices = amountOfVertices;
		this->submeshRanges = submeshRanges;
		this->vertexBuff = vertexBuff;
		this->indexBuff = indexBuff;
		CreateCB();
	}
	
	Mesh(std::string path)
	{
		// load obj file
		OBJ testObj(path);

		// load all materials for Obj
		int nrOfMat = (int)testObj.mtl.materials.size();
		for (int i = 0; i < nrOfMat; i++)
		{
			MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
		}
		Load(testObj);
	}



	void Load(OBJ& obj)
	{

#pragma region LoadObj

		std::vector<Vertex> vertices;

		//load full obj into vector
		for (auto& group : obj.groups)
		{
			unsigned int vertexCount = 0;

			if (!group.materialName.empty())
				matKey.emplace_back(group.materialName);
			else
				matKey.emplace_back("Default");

			for (auto& face : group.faces)
			{
				for (auto& index : face.indices)
				{
					vertexCount++;
					vertices.emplace_back(obj.vertex[index.x], obj.uv[index.y], obj.normal[index.z]);
				}
			}
			submeshVerCounts.emplace_back(vertexCount);//add submesh vertex count to list
		}

#pragma endregion

#pragma region RemoveDumplicates

		std::vector<Vertex> finalVertices;

		std::vector<unsigned int> indices32;
		std::vector<unsigned short> indices16;

		const bool is16bit = vertices.size() < 65535;

		//foreach vertex in submesh
		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			bool exist = false;

			//get index from final vertices by loop through it
			for (unsigned int k = 0; k < finalVertices.size(); k++)
			{
				//found vertex at index by use custom equal/not equal operator
				if (vertices[i] == finalVertices[k])
				{
					is16bit ? indices16.emplace_back((unsigned short)k) : indices32.emplace_back(k); //add index to indices
					exist = true;
					break;
				}
			}

			if (!exist)
			{
				is16bit ? indices16.emplace_back((unsigned short)finalVertices.size()) : indices32.emplace_back((unsigned int)finalVertices.size());
				finalVertices.emplace_back(vertices[i]);
			}

			// get bound min
			if (vertices[i].position.x < bound.min.x)
				bound.min.x = vertices[i].position.x;
			if (vertices[i].position.y < bound.min.y)
				bound.min.y = vertices[i].position.y;
			if (vertices[i].position.z < bound.min.z)
				bound.min.z = vertices[i].position.z;

			// get bound max
			if (vertices[i].position.x > bound.max.x)
				bound.max.x = vertices[i].position.x;
			if (vertices[i].position.y > bound.max.y)
				bound.max.y = vertices[i].position.y;
			if (vertices[i].position.z > bound.max.z)
				bound.max.z = vertices[i].position.z;

		}

#pragma endregion

		CalcBound();
		vertexBuffer.Create(finalVertices);
		is16bit ? indexBuffer.Create(indices16) : indexBuffer.Create(indices32);
		CreateCB();
	}
	Mesh(Mesh* other)
	{
		this->amountOfVertices = other->amountOfVertices;
		this->submeshRanges = other->submeshRanges;
		this->vertexBuff = other->vertexBuff;
		this->indexBuff = other->indexBuff;
	}

	void draw(ID3D11ShaderResourceView* srv, UINT stride = sizeof(vertex))
	{
		worldCB.BindToVS(0u);
		//UINT stride = stride;
		UINT offset = 0;

		int startIndex = 0;
		int startVertex = 0;
		GPU::immediateContext->PSSetShaderResources(0, 1, &srv);
		GPU::immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuff, &stride, &offset);
		GPU::immediateContext->IASetIndexBuffer(this->indexBuff, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		for (int i = 0; i < submeshRanges.size(); i++)
		{
			GPU::immediateContext->DrawIndexed(submeshRanges[i], startIndex, startVertex);
			startIndex += submeshRanges[i];
			startVertex += this->amountOfVertices[i];
		}
	}
	void draw(std::vector<ID3D11ShaderResourceView*>& allTextures, UINT stride = sizeof(vertex))
	{
		worldCB.BindToVS(0u);
		//UINT stride = stride;
		UINT offset = 0;

		int startIndex = 0;
		int startVertex = 0;
		GPU::immediateContext->IASetVertexBuffers(0, 1, &this->vertexBuff, &stride, &offset);
		GPU::immediateContext->IASetIndexBuffer(this->indexBuff, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		for (int i = 0; i < submeshRanges.size(); i++)
		{
			
			GPU::immediateContext->PSSetShaderResources(0, 1, &allTextures[i]);
			

			GPU::immediateContext->DrawIndexed(submeshRanges[i], startIndex, startVertex);
			startIndex += submeshRanges[i];
			startVertex += this->amountOfVertices[i];
		}
	}

	// without material
	void Draw(bool tesselation = false)
	{
		vertexBuffer.Bind();
		indexBuffer.Bind();

		tesselation ? worldCB.BindToDS(0u) : worldCB.BindToVS(0u);

		unsigned int currentVer = 0;
		for (unsigned int& verCount : submeshVerCounts)
		{
			GPU::immediateContext->DrawIndexed(verCount, currentVer, 0);
			currentVer += verCount;
		}
	}
	// with material
	void DrawWithMat(bool tesselation = false) const
	{
		vertexBuffer.Bind();
		indexBuffer.Bind();

		tesselation ? worldCB.BindToDS(0u) : worldCB.BindToVS(0u);

		unsigned int currentVer = 0;

		static std::string diffuseKey;
		static std::string ambientKey;
		static std::string specularKey;

		for (int i = 0; i < submeshVerCounts.size(); i++)
		{
			diffuseKey = MaterialLibrary::info[matKey[i]].diffuseKey;
			ambientKey = MaterialLibrary::info[matKey[i]].ambientKey;
			specularKey = MaterialLibrary::info[matKey[i]].specularKey;

			if (!diffuseKey.empty())
				MaterialLibrary::textures[diffuseKey].BindToPS(0u);
			else
				MaterialLibrary::textures["default_Diffuse.png"].BindToPS(0u);// default Diffuse

			if (!ambientKey.empty())
				MaterialLibrary::textures[ambientKey].BindToPS(1u);
			else
				MaterialLibrary::textures["default_Ambient.png"].BindToPS(1u);// default Ambient

			if (!specularKey.empty())
				MaterialLibrary::textures[specularKey].BindToPS(2u);
			else
				MaterialLibrary::textures["default_Specular.png"].BindToPS(2u);// default Specular

			MaterialLibrary::materials[matKey[i]].BindToPS(0u);

			GPU::immediateContext->DrawIndexed(submeshVerCounts[i], currentVer, 0);
			currentVer += submeshVerCounts[i];
		}

	}

	void DrawWithMat(int submeshCount,  bool reverse = false) const
	{
		vertexBuffer.Bind();
		indexBuffer.Bind();

		worldCB.BindToVS(0u);


		unsigned int currentVer = 0;

		static std::string diffuseKey;
		static std::string ambientKey;
		static std::string specularKey;


		if (reverse)
		{
			UINT nrOfVert = 0;
			for (int i = 0; i < submeshVerCounts.size(); i++)
			{
				nrOfVert += submeshVerCounts[i];
			}

			for (int i = submeshCount; i; i--)
			{
				diffuseKey = MaterialLibrary::info[matKey[i]].diffuseKey;
				ambientKey = MaterialLibrary::info[matKey[i]].ambientKey;
				specularKey = MaterialLibrary::info[matKey[i]].specularKey;

				if (!diffuseKey.empty())
					MaterialLibrary::textures[diffuseKey].BindToPS(0u);
				else
					MaterialLibrary::textures["default_Diffuse.png"].BindToPS(0u);// default Diffuse

				if (!ambientKey.empty())
					MaterialLibrary::textures[ambientKey].BindToPS(1u);
				else
					MaterialLibrary::textures["default_Ambient.png"].BindToPS(1u);// default Ambient

				if (!specularKey.empty())
					MaterialLibrary::textures[specularKey].BindToPS(2u);
				else
					MaterialLibrary::textures["default_Specular.png"].BindToPS(2u);// default Specular

				MaterialLibrary::materials[matKey[i]].BindToPS(0u);

				//currentVer -= nrOfVert;
				nrOfVert -= submeshVerCounts[i];
				GPU::immediateContext->DrawIndexed(submeshVerCounts[i], nrOfVert, 0);

			}
		}
		else
		{
			for (int i = 0; i < submeshCount; i++)
			{
				diffuseKey = MaterialLibrary::info[matKey[i]].diffuseKey;
				ambientKey = MaterialLibrary::info[matKey[i]].ambientKey;
				specularKey = MaterialLibrary::info[matKey[i]].specularKey;

				if (!diffuseKey.empty())
					MaterialLibrary::textures[diffuseKey].BindToPS(0u);
				else
					MaterialLibrary::textures["default_Diffuse.png"].BindToPS(0u);// default Diffuse

				if (!ambientKey.empty())
					MaterialLibrary::textures[ambientKey].BindToPS(1u);
				else
					MaterialLibrary::textures["default_Ambient.png"].BindToPS(1u);// default Ambient

				if (!specularKey.empty())
					MaterialLibrary::textures[specularKey].BindToPS(2u);
				else
					MaterialLibrary::textures["default_Specular.png"].BindToPS(2u);// default Specular

				MaterialLibrary::materials[matKey[i]].BindToPS(0u);

				GPU::immediateContext->DrawIndexed(submeshVerCounts[i], currentVer, 0);
				currentVer += submeshVerCounts[i];
			}
		}
	}

	unsigned int NumberOfVertices()
	{
		unsigned int total = 0;
		for (unsigned int subMeshVerCount : submeshVerCounts)
		{
			total += subMeshVerCount;
		}
		return total;
	}
	void AddSubmeshVertexCount(const unsigned int count)
	{
		submeshVerCounts.emplace_back(count);
	}
	void setMatrix(DirectX::XMFLOAT4X4 matrix)
	{
		this->matrix = matrix;



	}
	void updateONLINE()
	{
		static MatrixS worldS;
		DirectX::XMMATRIX xm = DirectX::XMLoadFloat4x4(&matrix);
		DirectX::XMStoreFloat4x4(&worldS.matrix, xm);

		worldCB.Update(&worldS, sizeof(MatrixS));
	}
	void UpdateCB(const DirectX::SimpleMath::Vector3& position, const DirectX::XMMATRIX& rotation, const DirectX::SimpleMath::Vector3& scale)
	{
		using namespace DirectX;

		static MatrixS worldS;

		XMStoreFloat4x4(&worldS.matrix, XMMatrixTranspose({ (XMMatrixScaling(scale.x, scale.y, scale.z) * rotation * XMMatrixTranslation(position.x, position.y, position.z))}));
		worldCB.Update(&worldS, sizeof(MatrixS));

		static VectorS positionS;
		positionS.vector = DirectX::SimpleMath::Vector4(position);
		positionCB.Update(&positionS, sizeof(VectorS));
	}
	void CreateCB()
	{
		const MatrixS worldS{};
		worldCB.Create((void*)&worldS, sizeof(MatrixS)); //world CB

		const VectorS positionS;
		positionCB.Create((void*)&positionS, sizeof(VectorS)); //position CB
	}
	void CalcBound()
	{
		bound.center = (bound.min + bound.max) * 0.5f;
		bound.width = (bound.max - bound.min) * 0.5f;

		bound.aabb.Center = bound.center + position;
		bound.aabb.Extents = bound.width;
	}
};