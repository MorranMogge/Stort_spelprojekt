#pragma once
#include "stdafx.h"

#include "ShaderResourceTexture.h"
#include "ConstantBuffer.h"
#include "MTL.h"

struct MaterialLibrary
{
	struct MaterialInfo
	{
		std::string diffuseKey;
		std::string ambientKey;
		std::string specularKey;

		DirectX::SimpleMath::Vector4 ambient{ 0.3f, 0.3f, 0.3f, 1.0f };
		DirectX::SimpleMath::Vector4 diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::SimpleMath::Vector3 specular{ 0.0f, 0.0f, 0.0f };
		float specularPower = 16.0f;
	};

	inline static std::unordered_map<std::string, ShaderResourceTexture> textures;
	inline static std::unordered_map<std::string, ConstantBuffer> materials;
	inline static std::unordered_map<std::string, MaterialInfo> info;

	static void AddTexture(const std::string path)
	{
		// return if path is empty 
		if (path.empty())
			return;

		//check if texture exist
		if (textures.find(path) == textures.end())
		{
			//not found
			ShaderResourceTexture diffuseSRT("../Textures/" + path);
			textures.emplace(path, diffuseSRT);
		}

	}
	static void LoadDefault()
	{
		MaterialInfo mat;
		mat.ambientKey = "default_Ambient.png";
		mat.specularKey = "default_Diffuse.png";
		mat.diffuseKey = "default_Specular.png";
		AddTexture(mat.ambientKey);
		AddTexture(mat.specularKey);
		AddTexture(mat.diffuseKey);
		info.emplace("Default", mat);



	}
	static void LoadMaterial(const MTL::Material& mtlMat)
	{
		//if empty
		if (mtlMat.name.empty())
			return;

		//check if material exist
		if (materials.find(mtlMat.name) == materials.end())
		{
			//not found
			MaterialInfo mat;
			mat.ambientKey = mtlMat.ambientPath.empty() ? "default_Ambient.png" : mtlMat.ambientPath;
			mat.specularKey = mtlMat.specularPath.empty() ? "default_Specular.png" : mtlMat.specularPath;
			mat.diffuseKey = mtlMat.diffusePath.empty() ? "default_Diffuse.png" : mtlMat.diffusePath;
			AddTexture(mat.ambientKey);
			AddTexture(mat.specularKey);
			AddTexture(mat.diffuseKey);

			mat.ambient = DirectX::SimpleMath::Vector4(mtlMat.ambient);
			mat.diffuse = DirectX::SimpleMath::Vector4(mtlMat.diffuse);
			mat.specular = mtlMat.specular;
			mat.specularPower = mtlMat.specularHighlight;
			info.emplace(mtlMat.name, mat);

			struct MaterialS
			{
				DirectX::SimpleMath::Vector4 ambient{ 0.0f, 0.0f, 0.0f, 0.0f };
				DirectX::SimpleMath::Vector4 diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
				DirectX::SimpleMath::Vector3 specular{ 0.0f, 0.0f, 0.0f };
				float specularPower = 16.0f;
			};
			MaterialS matS{
				{mat.ambient},
				{mat.diffuse},
				{mat.specular},
				{mat.specularPower} };

			// material proterty constant buffer
			materials.emplace(mtlMat.name, ConstantBuffer(&matS, sizeof(MaterialS)));
		}

	}

};