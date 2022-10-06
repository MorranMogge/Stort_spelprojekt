#include "SkyboxObj.h"
#include "stb_image.h"



bool CreateRenderTargets(Microsoft::WRL::ComPtr<ID3D11Texture2D>& Texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& renderedTextureView)
{
	//variables
	std::vector<std::string> filenames{ "Left.png", "Right.png", "Bottom.png", "Top.png", "Front.png", "Back.png" };// "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg", "boxTest.jpg" 
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

	int nrOfCameras = 6;

	D3D11_TEXTURE2D_DESC textureDesc = {};												//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = wth;
	textureDesc.Height = hth;
	textureDesc.MipLevels = 1u;															//olika nivååer av kompression
	textureDesc.ArraySize = nrOfCameras;												//en buffer
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;									//MapUsageTypless(usage);
	textureDesc.SampleDesc.Count = 1;													//Defaultvärden
	textureDesc.SampleDesc.Quality = 0;													//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;											//säger hur den ska användas när vi kommer åt den ()
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;														// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
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
}

void SkyboxObj::bindSkybox()
{
	GPU::immediateContext->PSSetShaderResources(1, 1, &this->textureCubeView);
	int a = 0;
	GPU::immediateContext->PSSetShaderResources(7, 1, &this->textureCubeView);
	std::cout << a << std::endl;
}

