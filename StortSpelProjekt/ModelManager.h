#pragma once
#define NOMINMAX
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <map>
#include <vector>
#include <string>
#include "ErrorLog.h"
#include "ModelDataContainer.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//#include "../DirectXTK-main/Src/SDKMesh.h"


//struct f�r en vertex
struct vertex
{
	DirectX::XMFLOAT3 pos; // Position
	DirectX::XMFLOAT3 nor; // Normal
	DirectX::XMFLOAT2 uv; // UV coordination
	/*DirectX::XMFLOAT3 tangent;*/

	vertex() {
		pos = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		uv = DirectX::XMFLOAT2{ 0.0f,0.0f };
		nor = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };
		/*tangent = DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f };*/
	};
	vertex(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& uv, DirectX::XMFLOAT3& nor, DirectX::XMFLOAT3& tangent) : pos(pos), uv(uv), nor(nor)/*, tangent(tangent)*/ {};
};


class ModelManager
{
private:

	//Struct som beskriver datan som beh�vs f�r en mesh, det �r i denna datastruktur som samtliga vertexes och indicies kommer sparas i oavsett med eller utan submeshes
	struct meshData
	{
		//Vertex data
		std::vector<vertex> vertexTriangle;
		//Indicies data
		std::vector<DWORD> indexTriangle;
	};

	
	struct meshData dataForMesh;
	
	//SubmeshRanges �r antalet draws som skall g�ras per submesh
	std::vector<int> submeshRanges;
	//AmountOfVerticies �r base vertex location - start index f�r f�rsta vertex som skall l�sas f�r mesh / submesh
	std::vector<int> amountOfvertices;
	//Skapa srv f�r given bild i finalFilePath s�kv�g
	bool makeSRV(ID3D11ShaderResourceView*& srv, std::string finalFilePath);
	//Process Assimp data
	void processNodes(aiNode* node, const aiScene* scene, const std::string& filePath);
	//Process Assimp mesh data
	void readNodes(aiMesh* mesh, const aiScene* scene);
	std::vector<ID3D11ShaderResourceView*> diffuseMaps;

	//Inneh�ller all data f�r mesh och texturer
	ModelDataContainer bank;
	ID3D11Device* device;
public:
	ModelManager();
	ModelManager(ID3D11Device* device);
	~ModelManager();
	//Startpunkt f�r att ladda in data via Assimp
	bool loadMeshData(const std::string& filePath);
	ID3D11ShaderResourceView* getSrv(const std::string key);
	std::vector<ID3D11ShaderResourceView*> getTextureMaps() const;
	//Fyller parametrarna med meshdata om nyckeln finns
	bool getMeshData(const std::string& filePath, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<int>& submeshRanges, std::vector<int>& amountOfVertces);
	void setDevice(ID3D11Device* device);
};