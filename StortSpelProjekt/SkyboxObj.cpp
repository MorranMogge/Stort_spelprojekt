#include "stdafx.h"
#include "SkyboxObj.h"
#include "stb_image.h"




bool CreateRenderTargets(Microsoft::WRL::ComPtr<ID3D11Texture2D>& Texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& renderedTextureView)
{
	//variables
	std::vector<std::string> filenames{ "0.png", "1.png", "2.png", "3.png", "4.png", "5.png" };// "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg" 
	std::vector<unsigned char*> images;
	int wth = 0;
	int hth = 0;
	int channels = 0;

	for (int i = 0; i < 6; i++)
	{
		unsigned char* img = stbi_load(("../Textures/" + filenames.at(i)).c_str(), &wth, &hth, &channels, STBI_rgb_alpha);
		if (img == NULL)
		{
			return 0;
		}
		else
		{
			images.push_back(img);
		}

	}

	D3D11_TEXTURE2D_DESC textureDesc = {};												//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = wth;
	textureDesc.Height = hth;
	textureDesc.MipLevels = 1u;															//olika niv??er av kompression
	textureDesc.ArraySize = 6;												//en buffer
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;									//MapUsageTypless(usage);
	textureDesc.SampleDesc.Count = 1;													//Defaultv?rden
	textureDesc.SampleDesc.Quality = 0;													//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;											//s?ger hur den ska anv?ndas n?r vi kommer ?t den ()
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;														// angeom cpu ska l?sa eller skriva (vi beh?ver varken eller s? d?rf?r 0)
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;							//misc flagga


	D3D11_SUBRESOURCE_DATA data[6];
	for (int i = 0; i < 6; i++)
	{
		data[i].pSysMem = images[i];
		data[i].SysMemPitch = wth * 4;
		data[i].SysMemSlicePitch = 0;
	}
	

	//create texture
	if (FAILED(GPU::device->CreateTexture2D(&textureDesc, data, Texture.GetAddressOf())))
	{
		std::cerr << "failed to create texture" << std::endl;
		return false;
	}

	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MipLevels = 1;
	shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;

	//create shader resource view 
	HRESULT hr = GPU::device->CreateShaderResourceView(Texture.Get(), &shaderResourceViewDesc, renderedTextureView.GetAddressOf());

	for (int i = 0; i < 6; i++)
	{
		stbi_image_free(images[i]);
	}

	return !FAILED(hr);
}

SkyboxObj::SkyboxObj()
{
	//Create SRV's & UAV's
	if (!CreateRenderTargets(this->textureCube, this->textureCubeView))
	{
		std::cerr << "error creating RenderTargets!" << std::endl;
	}
	this->skyBoxMesh = new Mesh("../Meshes/Skybox2");
	this->skyBoxObj = new GameObject(skyBoxMesh, DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), 0);
}

SkyboxObj::~SkyboxObj()
{
	delete this->skyBoxObj;
	delete this->skyBoxMesh;
}

void SkyboxObj::draw()
{
	ID3D11ShaderResourceView* nullsrv{ nullptr };
	GPU::immediateContext->PSSetShaderResources(4, 1, textureCubeView.GetAddressOf());
	this->skyBoxObj->draw();
	GPU::immediateContext->PSSetShaderResources(4, 1, &nullsrv);
}

