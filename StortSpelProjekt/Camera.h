#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "ConstantBufferNew.h"
#include "GPU.h"
#include "Planet.h"
#include "Player.h"

class Camera
{
private:
	int index = -1;
	float playerSpeed = 0.f;
	DirectX::XMMATRIX playerRotationMX = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR playerPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	bool collided = false;
	bool haveWon = false;
	float fieldOfView = 0.76f;
	float maxFOV = 0.8f;
	float minFOV = 0.6f;
	float yaw = 0.f;

	DirectX::XMFLOAT3 position;
	ConstantBufferNew<cameraStruct> cameraBuffer;
	ConstantBufferNew<posStruct> positionBuffer;
	ConstantBufferNew<posStruct> upVectorBuffer;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
	DirectX::XMVECTOR logicalPos = DirectX::XMVectorSet(0.0f, 60.0f, -30.0f, 0.0f);
	DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet(0.0f, 60.0f, -30.0f, 0.0f);
	DirectX::XMVECTOR oldCameraPos = cameraPos;
	DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 42.0f, 0.0f, 0.0f);

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR logicalUp = DEFAULT_UP;
	DirectX::XMVECTOR upVector = logicalUp;

	DirectX::XMVECTOR velocityVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 planetVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 cameraVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

public:
	Camera();
	~Camera();

	void updateCamera();
	void moveCamera(Player* player, const float& deltaTime);
	void moveVelocity(Player* player, const float& deltaTime);
	void collisionCamera(Player* player, const std::vector<Planet*>& planets, const float& deltaTime);
	void controllCamera(const float& deltaTime);

	void winScene(const DirectX::XMVECTOR& shipPosition, const DirectX::XMMATRIX& shipRotation);
	void landingMinigameScene(const Planet* planet, const DirectX::XMVECTOR& shipPosition, const DirectX::XMMATRIX& shipRotation);
	DirectX::XMVECTOR getForwardVector() const;
	DirectX::XMVECTOR getRightVector() const;
	DirectX::XMVECTOR getUpVector() const;
	DirectX::XMVECTOR getPosition() const;
	DirectX::XMVECTOR getRealPosition() const;
	int getCollidedWith() const;
	ID3D11Buffer* getViewBuffer();
	ID3D11Buffer* getPositionBuffer();
	void setPosition(const DirectX::XMFLOAT3& position);
	void setCameraLookAt(const DirectX::XMFLOAT3& position);
	void setRotToStart();
	void setHaveWon(bool haveWon);

	void VSbindPositionBuffer(const int &slot);
	void VSbindViewBuffer(const int& slot);
	void PSbindPositionBuffer(const int& slot);
	void PSbindViewBuffer(const int& slot);
	void GSbindPositionBuffer(const int& slot);
	void GSbindViewBuffer(const int& slot);
	void GSbindUpBuffer(const int& slot);
	void CSbindUpBuffer(const int& slot);
	void DSbindViewBuffer(const int& slot);
};