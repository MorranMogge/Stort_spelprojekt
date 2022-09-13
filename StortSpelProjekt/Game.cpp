#include "Game.h"

void Game::loadObjects()
{
	// load obj file
	std::vector<OBJ>objs_Static{
		OBJ("../Meshes/Planet")
	};

	// foreach obj in objs_static variable
	for (auto& obj : objs_Static)
	{
		meshes_Static.emplace_back(obj); // create mesh from obj

		// load obj material
		for (auto& mat : obj.mtl.materials)
		{
			MaterialLibrary::LoadMaterial(mat);
		}

	}
	meshes_Static[0].scale = DirectX::SimpleMath::Vector3(20, 20, 20);

	// re-calculate bounding box
	for (auto& mesh : meshes_Static)
	{
		mesh.CalcBound();
	}

	// load obj file
	std::vector<OBJ>objs_Dynamic{
		OBJ("../Meshes/Player"),
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
	meshes_Dynamic[0].position = DirectX::SimpleMath::Vector3(0, 0, -20);
	//meshes_Dynamic[0].rotation.y = (DirectX::XM_PI / DirectX::XM_PI) / -2;
}

void Game::drawObjects()
{
	for (auto& mesh : meshes_Static) //draw Static meshes
	{
		mesh.DrawWithMat();
	}
	for (auto& mesh : meshes_Dynamic) //draw Dynamic meshes
	{
		mesh.DrawWithMat();
	}
}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain)
	:camera(Camera(immediateContext, device)), immediateContext(immediateContext)
{
	MaterialLibrary::LoadDefault();
	/*UINT WIDTH = 1264;
	UINT HEIGHT = 681;*/

	UINT WIDTH = 1280;
	UINT HEIGHT = 720;

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	loadObjects();
	camera.updateCamera(immediateContext);
}

Game::~Game()
{
}

GAMESTATE Game::Update()
{
	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	float pos[3]{ meshes_Dynamic[0].position.x,meshes_Dynamic[0].position.y ,meshes_Dynamic[0].position.z };
	if (Input::KeyDown(KeyCode::D))
	{
		pos[0] += 0.1;
		//meshes_Dynamic[0].rotation.y += 0.001;
	}

	if (Input::KeyDown(KeyCode::A))
	{
		pos[0] -= 0.1;
		//meshes_Dynamic[0].rotation.y -= 0.001;
	}

	if (Input::KeyDown(KeyCode::ARROW_Up))
	{
		pos[1] += 0.1;
	}

	if (Input::KeyDown(KeyCode::ARROW_Down))
	{
		pos[1] -= 0.1;
	}

	if (Input::KeyDown(KeyCode::W))
	{
		pos[2] += 0.1;
		meshes_Dynamic[0].rotation.x += 0.001;
	}

	if (Input::KeyDown(KeyCode::S))
	{
		pos[2] -= 0.1;
		meshes_Dynamic[0].rotation.x -= 0.001;
	}

	/*OutputDebugString(L"PLAYER: ");
	OutputDebugString(std::to_wstring(pos[2]).c_str());
	OutputDebugString(L"\n");*/

	/*for (int i = 0; i < 3; i++)
	{
		if (pos[i] > 10) pos[i] = 10;
		else if (pos[i] < -10) pos[i] = -10;
	}*/

	meshes_Dynamic[0].position = {pos[0], pos[1] , pos[2] };
	camera.moveCamera(immediateContext, 1.f / 100.f, meshes_Dynamic[0].position, meshes_Dynamic[0].rotation);

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}

	return NOCHANGE;
}

void Game::Render()
{
	basicRenderer.setUpScene();
	drawObjects();
}
