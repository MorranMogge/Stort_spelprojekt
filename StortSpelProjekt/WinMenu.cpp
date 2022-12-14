#include "stdafx.h"
#include "WinMenu.h"
#include "SoundLibrary.h"

GAMESTATE WinMenu::Update()
{

	sexyMan->updateAnim(Time::DeltaTimeInSeconds(), 1, 1);
	sexyMan2->updateAnim(Time::DeltaTimeInSeconds(), 2, 1);

    return ui.GetGameState();
}

void WinMenu::drawFresnel()
{
	//Inverse
	basicRenderer.invFresnelPrePass();
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->drawAtmosphere();
	}
}

void WinMenu::drawObjects()
{
	//Bind light
	ltHandler.bindLightBuffers();

	//Draw with Ambient only shader
	basicRenderer.bindAmbientShader();

	//Draw light debug meshes
	ltHandler.drawDebugMesh();

	//Draw anim
	basicRenderer.changeToAnimation(false);
	sexyMan->drawSubMeshesWithTexture();
	sexyMan2->drawSubMeshesWithTexture();

	basicRenderer.resetTopology();
	GPU::immediateContext->OMSetDepthStencilState(nullptr, 0);
}

void WinMenu::Render()
{
//Render Scene
	basicRenderer.setUpScene(this->cam, false);
	drawObjects();

	//Unbind light
	ltHandler.unbindSrv();

	//Render fresnel objects
	basicRenderer.fresnelPrePass(this->cam);
	this->drawFresnel();

	//Render Skybox
	basicRenderer.skyboxPrePass();
	skybox.draw();
	basicRenderer.depthUnbind();

	ui.Draw();
}

ID3D11ShaderResourceView* WinMenu::loadTexture(const std::string& fileName)
{
	manager.addTexture(fileName);
	return manager.getSrv(fileName);
}

WinMenu::WinMenu(int& currentTeam) : manager(GPU::device)
{
	SoundLibrary::menuMusic.setVolume(0.5f);
	SoundLibrary::menuMusic.play(true);


	cam.setPosition(DirectX::XMFLOAT3(0, 0, -30));
	cam.setCameraLookAt(DirectX::XMFLOAT3(0, 0, 0));


	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, -30), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, -10), DirectX::XMFLOAT3(0, 1, 0), 1, 0.5f, 1000.5f);


	//manager.loadMeshAndBoneData("../Meshes/pinto_Run.fbx");


	ID3D11ShaderResourceView* blueTeamColour = this->manager.getSrv("../Textures/Kosmonaut_K1SG_Diffuse.png");
	

	AnimationData Anim;
	Mesh* mesh;
	if (currentTeam == 0)
	{
		manager.loadMeshAndBoneData("../Meshes/anim/character1_idle.fbx");
		manager.AdditionalAnimation("../Meshes/anim/character1_Dance1.fbx", "../Meshes/anim/character1_idle.fbx");
		manager.AdditionalAnimation("../Meshes/anim/character1_Dance2.fbx", "../Meshes/anim/character1_idle.fbx");
		manager.AdditionalAnimation("../Meshes/anim/character1_Dance3.fbx", "../Meshes/anim/character1_idle.fbx");
		manager.getAnimData("../Meshes/anim/character1_idle.fbx", vBuff, iBuff, subMeshRanges, verticies, Anim);
		mesh = new Mesh(vBuff, iBuff, subMeshRanges, verticies);

		sexyMan = new AnimatedMesh(mesh, Anim, { 0,0,-10 }, { 0,0,0 }, 0, nullptr, false);
		sexyMan->setPos(DirectX::XMFLOAT3(2, -2, -20));
		sexyMan->setScale(0.3f);
		sexyMan->setRot(DirectX::XMFLOAT3{ 0, 3.1415926f, 0 });
		std::vector<ID3D11ShaderResourceView*> allPlayerTextures;
		this->manager.getTextureMaps("../Meshes/anim/character1_idle.fbx", allPlayerTextures);
		sexyMan->setTextures(allPlayerTextures);

		sexyMan2 = new AnimatedMesh(mesh, Anim, { 0,0,-10 }, { 0,0,0 }, 0, nullptr, false);
		sexyMan2->setPos(DirectX::XMFLOAT3(-2, -2, -20));
		sexyMan2->setScale(0.3f);
		sexyMan2->setRot(DirectX::XMFLOAT3{ 0, 3.1415926f, 0 });
		sexyMan2->setTextures(allPlayerTextures);
	}
	else //Blue
	{
		manager.loadMeshAndBoneData("../Meshes/anim/character2_idle.fbx");
		manager.AdditionalAnimation("../Meshes/anim/character2_Dance1.fbx", "../Meshes/anim/character2_idle.fbx");
		manager.AdditionalAnimation("../Meshes/anim/character2_Dance2.fbx", "../Meshes/anim/character2_idle.fbx");
		manager.AdditionalAnimation("../Meshes/anim/character2_Dance3.fbx", "../Meshes/anim/character2_idle.fbx");
		manager.getAnimData("../Meshes/anim/character2_idle.fbx", vBuff, iBuff, subMeshRanges, verticies, Anim);
		mesh = new Mesh(vBuff, iBuff, subMeshRanges, verticies);

		sexyMan = new AnimatedMesh(mesh, Anim, { 0,0,-10 }, { 0,0,0 }, 0, nullptr, false);
		sexyMan->setPos(DirectX::XMFLOAT3(2, -2, -20));
		sexyMan->setScale(0.3f);
		sexyMan->setRot(DirectX::XMFLOAT3{ 0, 3.1415926f, 0 });
		std::vector<ID3D11ShaderResourceView*> allPlayerTextures;
		this->manager.getTextureMaps("../Meshes/anim/character2_idle.fbx", allPlayerTextures);
		sexyMan->setTextures(allPlayerTextures);

		sexyMan2 = new AnimatedMesh(mesh, Anim, { 0,0,-10 }, { 0,0,0 }, 0, nullptr, false);
		sexyMan2->setPos(DirectX::XMFLOAT3(-2, -2, -20));
		sexyMan2->setScale(0.3f);
		sexyMan2->setRot(DirectX::XMFLOAT3{ 0, 3.1415926f, 0 });
		sexyMan2->setTextures(allPlayerTextures);
	}	

	

	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

WinMenu::~WinMenu()
{
	delete sexyMan;
	delete sexyMan2;
}