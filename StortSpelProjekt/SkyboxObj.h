#pragma once
//#include<stdafx.h>
#include <wrl.h>
#include <vector>
#include <iostream>
#include "GPU.h"

class SkyboxObj
{
private:

	Microsoft::WRL::ComPtr<ID3D11Texture2D> textureCube;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureCubeView;

public:

	SkyboxObj();
	//void setSkybox(std::string Left, std::string Right, std::string Bottom, std::string Top, std::string Front, std::string Back);
	void bindSkybox();
};