#pragma once
#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

struct MTL final
{
	struct Material final
	{
		std::string name;

		DirectX::SimpleMath::Vector3 ambient; // r g b are in range 0 to 1
		std::string ambientPath; // path

		DirectX::SimpleMath::Vector3 diffuse; // r g b are in range 0 to 1
		std::string diffusePath; // path

		DirectX::SimpleMath::Vector3 specular; // r g b are in range 0 to 1
		std::string specularPath; // path
		float specularHighlight = 0;// range 0 to 1000

		DirectX::SimpleMath::Vector3 transmission; // r g b are in range 0 to 1

		int illumination = 0; // range 0 to 10, represent various material lightingand shading effects
		// 0 Color on and Ambient off
		// 1 Color on and Ambient on
		// 2 Highlight on
		// 3 Reflection on and Ray trace on
		// 4 Transparency: Glass on Reflection : Ray trace on
		// 5 Reflection : Fresnel on and Ray trace on
		// 6 Transparency : Refraction on Reflection : Fresnel off and Ray trace on
		// 7 Transparency : Refraction on Reflection : Fresnel on and Ray trace on
		// 8 Reflection on and Ray trace off
		// 9 Transparency : Glass on Reflection : Ray trace off
		// 10 Casts shadows onto invisible surfaces

		float dissolve = 1.0f; //1.0 is fully opaque,  0.0 is fully dissolved completely transparent

		// 1.0 means light does not bend as it passes through an object
		// Increasing optical_density increases the amount of bending
		// Glass has index of refraction about 1.5
		// Values of less than 1.0 produce bizarre results and are not recommended.
		float refraction = 1; //range from 0.001 to 10

	};

	std::vector<Material> materials;

	MTL(){}
	MTL(const std::string loadPath)
	{
		Load(loadPath);
	}

	void Load(const std::string loadPath)
	{

		std::ifstream in(loadPath);

		if (in.is_open())
		{
			const enum struct Key {
				Name,
				Ambient,
				AmbientPath,
				Diffuse,
				DiffusePath,
				Specular,
				SpecularPath,
				Transmission,
				Illumination,
				Dissolve,
				SpecularHighlight,
				Refraction,
			};
			static std::unordered_map<std::string, Key> keyMap{
				{"newmtl", Key::Name},
				{"Ka", Key::Ambient},
				{"map_Ka", Key::AmbientPath},
				{"Kd", Key::Diffuse},
				{"map_Kd", Key::DiffusePath},
				{"Ks", Key::Specular},
				{"map_Ks", Key::SpecularPath},
				{"Tf", Key::Transmission},
				{"illum", Key::Illumination},
				{"d", Key::Dissolve},
				{"Ns", Key::SpecularHighlight},
				{"Ni", Key::Refraction},
			};

			std::string lineStr;
			while (std::getline(in, lineStr))
			{
				if (!lineStr.empty())
				{
					std::stringstream line(lineStr);
					std::string header;
					line >> header;

					switch (keyMap[header])
					{
					case(Key::Name): {
						line.ignore();
						std::string matName;
						std::getline(line, matName);
						MTL::Material material;
						material.name = matName;
						materials.push_back(material);
					}; break;

					case(Key::Ambient): {
						DirectX::SimpleMath::Vector3 value;
						line >> value.x >> value.y >> value.z;
						materials.back().ambient = value;
					}; break;

					case(Key::AmbientPath): {
						line.ignore();
						std::string path;
						std::getline(line, path);
						materials.back().ambientPath = path;

					}; break;

					case(Key::Diffuse): {
						DirectX::SimpleMath::Vector3 value;
						line >> value.x >> value.y >> value.z;
						//if (value.x == 0.0f && value.y == 0.0f && value.z == 0.0f)
						//{
						//	value.x = 1.0f; value.y = 1.0f; value.z = 1.0f;
						//}
						materials.back().diffuse = value;
					}; break;

					case(Key::DiffusePath): {
						line.ignore();
						std::string path;
						std::getline(line, path);
						materials.back().diffusePath = path;
					}; break;

					case(Key::Specular): {
						DirectX::SimpleMath::Vector3 value;
						line >> value.x >> value.y >> value.z;
						materials.back().specular = value;
					}; break;

					case(Key::SpecularPath): {
						line.ignore();
						std::string path;
						std::getline(line, path);
						materials.back().specularPath = path;
					}; break;

					case(Key::Transmission): {
						DirectX::SimpleMath::Vector3 value;
						line >> value.x >> value.y >> value.z;
						materials.back().transmission = value;
					}; break;

					case(Key::Illumination): {
						int value;
						line >> value;
						materials.back().illumination = value;
					}; break;

					case(Key::Dissolve): {
						float value;
						line >> value;
						materials.back().dissolve = value;
					}; break;

					case(Key::SpecularHighlight): {
						float value;
						line >> value;
						materials.back().specularHighlight = value;
					}; break;

					case(Key::Refraction): {
						float value;
						line >> value;
						materials.back().refraction = value;
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