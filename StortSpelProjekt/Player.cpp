#include "Player.h"
#include <algorithm>

Player::Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:GameObject(useMesh, pos, rot, id)
{
}

Player::Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:GameObject(objectPath, pos, rot, id)
{
}

void Player::handleInputs()
{
	////return true ONCE key press, trigger only ONCE
	//if (Input::KeyPress(KeyCode::W))
	//{
	//    std::cout << "W pressed \n";
	//}
	////return true ONCE key release, trigger only ONCE
	//if (Input::KeyUp(KeyCode::A))
	//{
	//    std::cout << "A key released \n";
	//}

	//return true as long as key holding down, trigger every frame
	/*if (Input::KeyDown(KeyCode::W))
	{
		std::cout << "W key is holding down \n";
	}
	if (Input::KeyDown(KeyCode::S))
	{
		std::cout << "S key is holding down \n";
	}


	if (Input::KeyDown(KeyCode::A))
	{
		std::cout << "A key is holding down \n";
	}
	if (Input::KeyDown(KeyCode::D))
	{
		std::cout << "D key is holding down \n";
	}*/

}

void Player::move(DirectX::XMVECTOR cameraForward, DirectX::XMVECTOR cameraUp, DirectX::XMVECTOR cameraRight, DirectX::SimpleMath::Vector3& position, DirectX::XMMATRIX& rotation, DirectX::XMMATRIX& rotationMX, const DirectX::XMFLOAT3& grav, float deltaTime)
{
	normalVector = XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);

	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);

	upVector = XMVector3Normalize(upVector);
	rightVector = XMVector3Normalize(rightVector);
	forwardVector = XMVector3Normalize(forwardVector);

	cameraForward = XMVector3Normalize(cameraForward);
	cameraRight = XMVector3Normalize(cameraRight);

	//X-Rotation
	dotProduct = XMVector3Dot(normalVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.x < -0.1f)
	{
		rotation *= XMMatrixRotationAxis(rightVector, -0.01f);
		rotationMX *= XMMatrixRotationAxis(rightVector, -0.01f);
	}
	else if (dotValue.x > 0.1f)
	{
		rotation *= XMMatrixRotationAxis(rightVector, 0.01f);
		rotationMX *= XMMatrixRotationAxis(rightVector, 0.01f);
	}

	//Z-Rotation
	dotProduct2 = XMVector3Dot(normalVector, rightVector);
	XMStoreFloat3(&dotValue2, dotProduct2);
	if (dotValue2.z < -0.1f)
	{
		rotation *= XMMatrixRotationAxis(forwardVector, 0.01f);
		rotationMX *= XMMatrixRotationAxis(forwardVector, 0.01f);
	}
	else if (dotValue2.z > 0.1f)
	{
		rotation *= XMMatrixRotationAxis(forwardVector, -0.01f);
		rotationMX *= XMMatrixRotationAxis(forwardVector, -0.01f);
	}

	//Help-rotation
	if (Input::KeyDown(KeyCode::K))
	{
		rotation *= XMMatrixRotationAxis(normalVector, 0.01f);
	}

	//Movement
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 3.0f;
	}

	if (Input::KeyDown(KeyCode::W))
	{
		position += forwardVector * deltaTime * 40.0f;

		dotProduct2 = XMVector3Dot(cameraForward, rightVector);
		XMStoreFloat3(&dotValue2, dotProduct2);
		if (dotValue2.z < -0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue2.z > 0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else if (!XMVector3Equal(cameraRight, rightVector))
		{
			rotation *= XMMatrixRotationAxis(normalVector, 0.02f);
		}
	}

	if (Input::KeyDown(KeyCode::S))
	{
		position += forwardVector * deltaTime * 40.0f;

		dotProduct2 = XMVector3Dot(-cameraForward, rightVector);
		XMStoreFloat3(&dotValue2, dotProduct2);
		if (dotValue2.z < -0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue2.z > 0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else if (!XMVector3Equal(-cameraRight, -rightVector))
		{
			rotation *= XMMatrixRotationAxis(normalVector, -0.02f);
		}
	}

	if (Input::KeyDown(KeyCode::D))
	{
		position += forwardVector * deltaTime * 40.0f;

		dotProduct2 = XMVector3Dot(cameraRight, rightVector);
		XMStoreFloat3(&dotValue2, dotProduct2);
		if (dotValue2.z < -0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue2.z > 0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else if (!XMVector3Equal(cameraUp, rightVector))
		{
			rotation *= XMMatrixRotationAxis(normalVector, 0.02f);
		}
	}

	if (Input::KeyDown(KeyCode::A))
	{
		position += forwardVector * deltaTime * 40.0f;

		dotProduct2 = XMVector3Dot(-cameraRight, rightVector);
		XMStoreFloat3(&dotValue2, dotProduct2);
		if (dotValue2.z < -0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue2.z > 0.005f)
		{
			rotation *= XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else if (!XMVector3Equal(-cameraUp, -rightVector))
		{
			rotation *= XMMatrixRotationAxis(normalVector, -0.02f);
		}
	}

	//Help keys
	if (Input::KeyDown(KeyCode::E))
	{
		//position.y += 0.1f;
		position += upVector * deltaTime * 40;
	}

	else if (Input::KeyDown(KeyCode::Q))
	{
		//position.y -= 0.1f;
		position -= upVector * deltaTime * 40;
	}
}

bool Player::getPickup(GameObject* pickup)
{
	bool pickedUp = false;


	if (Input::KeyDown(KeyCode::SPACE))                           //SPACE
	{
		//Check if should pick up
		if (this->withinRadious(pickup, 50) && this->pickup == nullptr)
		{
			this->pickup = pickup;
		}
	}

	return pickedUp;
}

void Player::releasePickup()
{
	this->pickup = nullptr;
}

DirectX::XMVECTOR Player::getUpVec() const
{
	return this->upVector;
}

DirectX::XMVECTOR Player::getForwardVec() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Player::getRightVec() const
{
	return this->rightVector;
}