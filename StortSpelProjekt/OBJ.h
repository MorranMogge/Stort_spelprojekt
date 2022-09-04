#pragma once

#include <vector>
#include <string>

#include <fstream>
#include <sstream>
#include <unordered_map>

#include <SimpleMath.h>
#include "MTL.h"

struct OBJ
{

	struct Face
	{
		std::vector<DirectX::XMINT3> indices;
	};

	struct Group
	{
		std::string materialName;
		bool smoothingGroup = false;
		std::string name = "";
		std::vector<Face> faces;
	};

	MTL mtl;

	std::vector<std::string> description;

	std::vector<DirectX::SimpleMath::Vector3> vertex;
	std::vector<DirectX::SimpleMath::Vector2> uv;
	std::vector<DirectX::SimpleMath::Vector3> normal;

	std::vector<Group> groups;

	//swap left hand to right or right to left
	void swapCoordinateSystem()
	{
		for (int i = 0; i < vertex.size(); i++)
			vertex[i].z *= -1.0f;
		for (int i = 0; i < normal.size(); i++)
			normal[i].z *= -1.0f;
		for (int i = 0; i < uv.size(); i++)
			uv[i].y *= 1.0f - uv[i].y;
	}
	OBJ(const std::string loadPath)
	{
		using namespace DirectX::SimpleMath;

		std::ifstream in(loadPath + ".obj");

		if (in.is_open())
		{
			enum class Key
			{
				//vertex data
				Vertex,
				UV,
				Normal,
				//group and face index
				Group,
				SmoothingGroup,
				Face,
				//material
				MaterialLibrary,
				MaterialName,
				//misc
				ObjectName,
				Description,
			};
			const std::unordered_map<std::string, Key> keyMap{
				//vertex data
				{"v", Key::Vertex},
				{"vt", Key::UV},
				{"vn", Key::Normal},

				//group and face index
				{"g", Key::Group},
				{"s", Key::SmoothingGroup},
				{"f", Key::Face},

				//material
				{"mtllib", Key::MaterialLibrary},
				{"usemtl", Key::MaterialName},

				//misc
				{"o", Key::ObjectName},
				{"#", Key::Description},
			};

			int smoothingGroup = -1;//-1 = not assign yet, 0 false, 1 true

			std::string lineStr;

			std::string header;
			std::stringstream line;

			//reuse variables
			Vector3 v;
			Vector2 vt;
			Vector3 vn;
			std::string s;
			std::string groupName;

			//f reuse variables
			std::string str;
			std::stringstream streamTemp;  //make stringstream for later
			DirectX::XMINT3 indices;
			std::vector<DirectX::XMINT3> vertexIndices;
			vertexIndices.reserve(3);

			while (std::getline(in, lineStr))
			{
				if (!lineStr.empty())
				{
					line.clear();
					line << lineStr;
					line >> header;

					switch (keyMap.at(header))
					{
					case(Key::Vertex): {
						line >> v.x >> v.y >> v.z;
						//v.z *= -1.0f; // Invert Z to change to left hand system.
						vertex.push_back(v);
					}; break;

					case(Key::UV): {
						line >> vt.x >> vt.y;
						vt.y = 1.0f - vt.y; // Invert V to change to left hand system.
						uv.push_back(vt);
					}; break;

					case(Key::Normal): {
						line >> vn.x >> vn.y >> vn.z;
						//vn.z *= -1.0f;// Invert Z to change to left hand system.
						normal.push_back(vn);
					}; break;

					case(Key::SmoothingGroup): {
						line >> s;
						if (s == "0" || s == "off" || s == "false" || s == "False")
							smoothingGroup = 0;
						else
							smoothingGroup = 1;
					}; break;

					case(Key::Group): {
						line.ignore();
						line >> groupName;
						if (groupName != "default") //we dont want to add default group from maya, skip
						{
							OBJ::Group g;
							g.name = groupName; //group name
							if (smoothingGroup != -1) //assign Smoothing Group
							{
								g.smoothingGroup = smoothingGroup;
								smoothingGroup = -1;
							}
							groups.push_back(g);
						}
					}; break;

					case(Key::Face): {
						groups.back().faces.push_back(OBJ::Face());//add face to obj
						vertexIndices.clear();

						while (line >> str) //look like 18/43/555, full is  18/43/555 18/43/555 18/43/555
						{
							streamTemp.clear();
							streamTemp << str; //put that string into strStream Temp

							streamTemp >> indices.x; //position index
							streamTemp.ignore(); //ignore "/"
							streamTemp >> indices.y; //uv index
							streamTemp.ignore(); //ignore "/"
							streamTemp >> indices.z; //normal index
							streamTemp.ignore(); //ignore space

							indices.x -= 1; //subtract one from index it start 0
							indices.y -= 1; //subtract one from index it start 0
							indices.z -= 1; //subtract one from index it start 0

							vertexIndices.push_back(indices);//put it into std::vector
						}
						//std::reverse(vertexIndices.begin(), vertexIndices.end()); //Read face data backwards to convert to left hand from right hand.
						groups.back().faces.back().indices = vertexIndices;//add indices to obj
					}; break;

					case(Key::MaterialLibrary): {
						std::string mtlPath;
						line >> mtlPath;
						mtl.Load("../Meshes/" + mtlPath);
					}; break;

					case(Key::MaterialName): {
						line >> groups.back().materialName;
					}; break;

					case(Key::ObjectName): {

					}; break;

					case(Key::Description): {
						line.ignore();
						std::getline(line, str);
						description.push_back(str);
					}; break;

					default: {

					}break;

					}
				}
			}
			in.close();
		}

	}

};