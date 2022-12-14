#include "stdafx.h"
#include "ObjSaver.h"
#include <sstream>
#include "Vertex.h"

void saveObj(const std::string& fileName, const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices)
{
	std::ofstream createdFile;

	createdFile.open("../Meshes/" + fileName + ".obj");
	if (!createdFile.is_open()) return;

	int loops = vertices.size();
	std::string inStr;
	Vertex inVertex = {};
	for (int i = 0; i < loops; i++)
	{
		inVertex = vertices[i];
		inStr = "v " + std::to_string(inVertex.position.x) + " " + std::to_string(inVertex.position.y) + " " + std::to_string(inVertex.position.z) + "\n";
		createdFile << inStr;
	}

	for (int i = 0; i < loops; i++)
	{
		inVertex = vertices[i];
		inStr = "vt " + std::to_string(inVertex.normal.x) + " " + std::to_string(inVertex.normal.y) + " " + std::to_string(inVertex.normal.z) + "\n";
		createdFile << inStr;
	}

	for (int i = 0; i < loops; i++)
	{
		inVertex = vertices[i];
		inStr = "vn " + std::to_string(inVertex.uv.x) + " " + std::to_string(inVertex.uv.y) + "\n";
		createdFile << inStr;
	}

	loops = indices.size();
	for (int i = 0; i < loops; i++)
	{
		inVertex = vertices[i];
		inStr = "f " + std::to_string(indices[i]) + "/" + std::to_string(indices[i]) + "/" + std::to_string(indices[i]) + "\n";
		createdFile << inStr;
	}

	createdFile.close();
}