#include "Game.h"
#include "DirectXMathHelper.h"

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
	meshes_Static[0].scale = DirectX::SimpleMath::Vector3(0.5, 0.5, 0.5);

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
	meshes_Dynamic[0].scale = DirectX::SimpleMath::Vector3(0.25, 0.25, 0.25);
	meshes_Dynamic[0].position = DirectX::SimpleMath::Vector3(10, 10, -10);


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

	DirectX::XMFLOAT3 pos( meshes_Dynamic[0].position.x,meshes_Dynamic[0].position.y ,meshes_Dynamic[0].position.z );
	if (Input::KeyDown(KeyCode::W)) pos.z += 0.1;
	if (Input::KeyDown(KeyCode::S)) pos.z -= 0.1;
	if (Input::KeyDown(KeyCode::I)) pos.y += 0.1;
	if (Input::KeyDown(KeyCode::K)) pos.y -= 0.1;
	if (Input::KeyDown(KeyCode::D)) pos.x += 0.1;
	if (Input::KeyDown(KeyCode::A)) pos.x -= 0.1;

	/*for (int i = 0; i < 3; i++)
	{
		if (pos[i] > 10) pos[i] = 10;
		else if (pos[i] < -10) pos[i] = -10;
	}*/

	subtractionXMFLOAT3(pos, planetGravityField.calcGravFactor(pos));

	meshes_Dynamic[0].position = {pos.x, pos.y , pos.z };

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}

	//camera.moveCamera(immediateContext, 1.f/100.f);
	return NOCHANGE;
}

void Game::Render()
{
	basicRenderer.setUpScene();
	drawObjects();
}
