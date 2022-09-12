#include "Menu.h"
#include "Input.h"
#include "Console.h"
#include "SoundCollection.h"
#include "Topology.h"
#include "Shader.h"

using namespace DirectX::SimpleMath;

void Menu::UpdateConstanBuffer()
{
#pragma region Mesh

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}

#pragma endregion

#pragma region MainCam

	mainCamera.hasTarget = true;
	mainCamera.lookTo = { 0,0,1 };
	DirectX::XMStoreFloat4x4(&mainCamViewProjS.matrix, mainCamera.ViewProjectionM());
	mainCamViewProjCB.Update(&mainCamViewProjS, sizeof(MatrixS));

	mainCamPosS.vector = Vector4(mainCamera.position);
	mainCamPosCB.Update(&mainCamPosS, sizeof(VectorS));

#pragma endregion

#pragma region Light

	lightCB.Update(&lightS, sizeof(lightS));

#pragma endregion

}

void Menu::ShadowPass()
{
#pragma region Bind

	Topology::Bind(Topology::Type::Triangle_List);
	depthStencilState.Bind();
	shadowViewPort.Bind();
	show_WireFrame ? rss_Wireframe.Bind() : rss_Solid.Bind();

#pragma endregion

#pragma region Shader

	Shader::vs_Shadow.Bind();
	Shader::UnBindPS();

#pragma endregion

#pragma region Draw

	for (int i = 0; i < 4; i++) // nr of lights
	{
		lightCam.position = Vector3(lightS.lits[i].position);
		lightCam.lookTo = Vector3(lightS.lits[i].direction);
		DirectX::XMStoreFloat4x4(&lightCamViewProjS.matrix, lightCam.ViewProjectionM());
		lightCamViewProjCB.Update(&lightCamViewProjS, sizeof(MatrixS));

		//lightS.lits[i].lightCameraMatrix = lightCam.viewProjectionM();
		//lightCB.update(&lightS, sizeof(LightS));

		shadowDepthStencilView[i].Clear();

		ID3D11RenderTargetView* nullRTV = nullptr;
		GPU::immediateContext->OMSetRenderTargets(0, &nullRTV, shadowDepthStencilView[i].Get());

		lightCamViewProjCB.BindToVS(1u);

		for (auto& mesh : meshes_Static) //draw Static meshes
		{
			mesh.DrawWithMat();
		}
		for (auto& mesh : meshes_Dynamic) //draw Dynamic meshes
		{
			mesh.DrawWithMat();
		}

	}

#pragma endregion

#pragma region UnBind

	Shader::UnBindVS();
	DepthStencilState::UnBind();
	ViewPort::UnBind();

#pragma endregion

}

void Menu::GeometryPass()
{

#pragma region Clear

	depthStencilView.Clear(); // clear depth stencil
	rtv.Clear();

#pragma endregion

#pragma region Bind

	Topology::Bind(Topology::Type::Triangle_List);

	show_WireFrame ? rss_Wireframe.Bind() : rss_Solid.Bind();

	rtv.BindWithDepth(depthStencilView);

	viewPort.Bind();

#pragma endregion

#pragma region Shader

	Shader::vs_NoTess.Bind();
	mainCamViewProjCB.BindToVS(1u);

	Shader::ps_Cube.Bind();
	shadowSRV.BindToPS(3u);

	samplerState_Wrap.BindToPS(0u);

#pragma endregion

#pragma region Draw

	//draw Static meshes
	for (auto& mesh : meshes_Static)
	{
		mesh.DrawWithMat();
	}
	//draw Dynamic meshes
	for (auto& mesh : meshes_Dynamic)
	{
		mesh.DrawWithMat();
	}

#pragma endregion

#pragma region UnBind

	// unbinds
	Shader::UnBindVS();
	Shader::UnBindPS();

	RenderTargetView::UnBind();

#pragma endregion

}

Menu::Menu()
{
	std::cout << "Menu constructor \n";

	SoundCollection::testSound.play(); // play test sound

	MaterialLibrary::LoadDefault(); //load default material

	Shader::LoadAll(); // load all shader

#pragma region Mesh_Static

	manager = new ModelManager(GPU::device);
	
	manager->loadMeshData("../Meshes/goblin.fbx");
	
	this->testMeshes = manager->getMeshes();
	
	std::cout << testMeshes.size() << "\n";
	 
	
	// load obj file
	/*std::vector<OBJ>objs_Static{
		OBJ("../Meshes/gob"),
		OBJ("../Meshes/pinto"),
	};*/

	// foreach obj in objs_static variable
	//for (auto& obj : objs_Static) 
	//{
	//	meshes_Static.emplace_back(obj); // create mesh from obj

	//	// load obj material
	//	for (auto& mat : obj.mtl.materials)
	//	{
	//		MaterialLibrary::LoadMaterial(mat);
	//	}

	//}
	//
	//// set position
	//meshes_Static[1].position = { -20, 0, 0 };

	//// re-calculate bounding box
	//for (auto& mesh : meshes_Static)
	//{
	//	mesh.CalcBound();
	//}

#pragma endregion

#pragma region Mesh_Dynamic

	// load obj file
	std::vector<OBJ>objs_Dynamic{
		/*OBJ("../Meshes/pinto"),*/
	};

	// foreach obj in objs_Dynamic variable
	for (auto& obj : objs_Dynamic)
	{
		auto& mesh = meshes_Dynamic.emplace_back(obj); // create mesh from obj

		// load material
		for (auto& mat : obj.mtl.materials)
		{
			MaterialLibrary::LoadMaterial(mat);
		}

	}

#pragma endregion

	//for (int i = 0; i < testMeshes.size(); i++)
	//{
	//	testMeshes[i]->Draw(GPU::immediateContext);
	//}

#pragma region MainCamera

	mainCamPosCB.Create(&mainCamPosS, sizeof(VectorS));
	mainCamPosS.vector = Vector4(mainCamera.position);
	mainCamera.aspectRatio = (float)GPU::windowWidth / (float)GPU::windowHeight;
	DirectX::XMStoreFloat4x4(&mainCamViewProjS.matrix, mainCamera.ViewProjectionM());
	mainCamViewProjCB.Create(&mainCamViewProjS, sizeof(MatrixS));

#pragma endregion

#pragma region shadow

#pragma region DepthTexture

	ID3D11Texture2D* shadowDepthTexture = nullptr;

	D3D11_TEXTURE2D_DESC shadowDSVTexDesc{};
	shadowDSVTexDesc.Width = GPU::windowWidth;
	shadowDSVTexDesc.Height = GPU::windowHeight;
	shadowDSVTexDesc.MipLevels = 1u;
	shadowDSVTexDesc.ArraySize = 4u;
	shadowDSVTexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS //DXGI_FORMAT_R32_TYPELESS
	shadowDSVTexDesc.SampleDesc.Count = 1;
	shadowDSVTexDesc.SampleDesc.Quality = 0;
	shadowDSVTexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	shadowDSVTexDesc.BindFlags =
		D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	shadowDSVTexDesc.CPUAccessFlags = 0;
	shadowDSVTexDesc.MiscFlags = 0;
	GPU::device->CreateTexture2D(&shadowDSVTexDesc, nullptr, &shadowDepthTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC shadowSRVDesc{};
	shadowSRVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS; //DXGI_FORMAT_R24_UNORM_X8_TYPELESS //DXGI_FORMAT_R32_FLOAT
	shadowSRVDesc.ViewDimension = D3D_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shadowSRVDesc.Texture2DArray.ArraySize = 4u;
	shadowSRVDesc.Texture2DArray.FirstArraySlice = 0;
	shadowSRVDesc.Texture2DArray.MipLevels = 1u;
	shadowSRVDesc.Texture2DArray.MostDetailedMip = 0;
	shadowSRV.Create(shadowDepthTexture, shadowSRVDesc);

	// Depth Stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSVDesc{};
	shadowDSVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT; //DXGI_FORMAT_D24_UNORM_S8_UINT  //DXGI_FORMAT_D32_FLOAT
	shadowDSVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	shadowDSVDesc.Flags = 0;
	shadowDSVDesc.Texture2DArray.MipSlice = 0;
	shadowDSVDesc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 4; i++)//light number
	{
		shadowDSVDesc.Texture2DArray.FirstArraySlice = i;
		shadowDepthStencilView[i].Create(shadowDepthTexture, shadowDSVDesc);
	}

	shadowDepthTexture->Release();

#pragma endregion

	shadowViewPort.Config((float)GPU::windowWidth, (float)GPU::windowHeight);

#pragma endregion

#pragma region Lights

	lightS.lits[0].position = { 3.0f, 1.0f, -6.0f };
	lightS.lits[0].direction = { 0.5f, -0.5f, 1.0f };
	lightS.lits[0].color = { 1.0f, 0.0f, 0.0f };
	lightS.lits[0].range = 30.0f;
	lightS.lits[0].cone = 90.0f;
	lightS.lits[0].enabled = true;
	lightS.lits[0].type = Light::Type::Spot;

	lightS.lits[1].position = { -2.0f, 1.0f, -6.0f };
	lightS.lits[1].direction = { 0.0f, -0.5f, 1.0f };
	lightS.lits[1].color = { 0.0f, 1.0f, 0.0f, 0.0f };
	lightS.lits[1].range = 50.0f;
	lightS.lits[1].cone = 90.0f;
	lightS.lits[1].enabled = true;
	lightS.lits[1].type = Light::Type::Spot;

	lightS.lits[2].position = { -3.0f, 1.0f, 6.0f };
	lightS.lits[2].direction = { 0.0f, -0.5f, -1.0f };
	lightS.lits[2].color = { 1.0f, 1.0f, 0.0f };
	lightS.lits[2].range = 100.0f;
	lightS.lits[2].cone = 90.0f;
	lightS.lits[2].enabled = true;
	lightS.lits[2].type = Light::Type::Spot;

	lightS.lits[3].position = { -12.0f, 50.0f, -70.0f };
	lightS.lits[3].direction = { 0.3f, -0.7f, 1.0f };
	lightS.lits[3].color = { 0.5f, 0.5f, 0.5f };
	lightS.lits[3].range = 30.0f;
	lightS.lits[3].cone = 90.0f;
	lightS.lits[3].enabled = true;
	lightS.lits[3].type = Light::Type::Directional;

	// fill light cam info and light cam view projection
	lightCam.hasTarget = false;
	lightCam.farZ = 1000.0f;

	for (int i = 0; i < 4; i++)
	{
		lightCam.position = Vector3(lightS.lits[i].position);
		lightCam.lookTo = Vector3(lightS.lits[i].direction);
		lightCam.aspectRatio = (float)GPU::windowWidth / (float)GPU::windowHeight;
		DirectX::XMStoreFloat4x4(&lightS.lits[i].lightCamViewProj, lightCam.ViewProjectionM());
	}

	lightCB.Create(&lightS, sizeof(LightS));//create light constant buffer

	DirectX::XMStoreFloat4x4(&lightCamViewProjS.matrix, lightCam.ViewProjectionM());
	lightCamViewProjCB.Create(&lightCamViewProjS, sizeof(MatrixS));

#pragma endregion

#pragma region depthStencil

	ID3D11Texture2D* depthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC dsvTexDesc{};
	dsvTexDesc.Width = GPU::windowWidth;
	dsvTexDesc.Height = GPU::windowHeight;
	dsvTexDesc.MipLevels = 1;
	dsvTexDesc.ArraySize = 1;
	dsvTexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	dsvTexDesc.SampleDesc.Count = 1;
	dsvTexDesc.SampleDesc.Quality = 0;
	dsvTexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	dsvTexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	dsvTexDesc.CPUAccessFlags = 0;
	dsvTexDesc.MiscFlags = 0;
	GPU::device->CreateTexture2D(&dsvTexDesc, nullptr, &depthStencilTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;
	depthStencilView.Create(depthStencilTexture, dsvDesc);

	depthStencilTexture->Release();

#pragma endregion

	rtv.CreateBackBuffer();
	depthStencilState.Create();
	viewPort.Config((float)GPU::windowWidth, (float)GPU::windowHeight);

#pragma region Sampler

	samplerState_Wrap.Create(SamplerState::Mode::Wrap, SamplerState::Filter::Linear);
	samplerState_Clamp.Create(SamplerState::Mode::Clamp, SamplerState::Filter::Linear);
	samplerState_Shadow.CreateShadow();

#pragma endregion

#pragma region Rasterizer

	rss_Solid.Create(RasterizerState::Cull::Back);
	rss_Wireframe.Create(RasterizerState::Cull::Back, true);

#pragma endregion

}

Menu::~Menu()
{

}

GAMESTATE Menu::Update()
{

#pragma region Inputs

	//return true ONCE key press, trigger only ONCE
	if (Input::KeyPress(KeyCode::W))
	{
		std::cout << "W pressed \n";
	}

	//return true as long as key holding down, trigger every frame
	if (Input::KeyDown(KeyCode::S))
	{
		std::cout << "S key is holding down \n";
	}

	//return true ONCE key release, trigger only ONCE
	if (Input::KeyUp(KeyCode::A))
	{
		std::cout << "A key released \n";
	}

	// destroy console window
	if (Input::KeyPress(KeyCode::Q))
	{
		Console::Destroy();
	}

	// exit game
	if (Input::KeyPress(KeyCode::ESC))
		return GAMESTATE::EXIT;

#pragma endregion

#pragma region Movements

	/*constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	if (zpos >= 0)
		forward = false;
	else if (zpos <= -10)
		forward = true;

	forward ? zpos += speed : zpos -= speed;
	meshes_Dynamic[0].position = { -10, 0, zpos };*/

#pragma endregion

	return NOCHANGE;
}

void Menu::Render()
{
	
	UpdateConstanBuffer();

	ShadowPass();
	GeometryPass();

}

void Menu::DrawUI()
{
	ui.Draw();
}
