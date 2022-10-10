#include "Player.h"
using namespace DirectX;

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

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, DirectX::SimpleMath::Vector3& position, DirectX::XMMATRIX& rotation, DirectX::XMMATRIX& rotationMX, const DirectX::XMFLOAT3& grav, float& deltaTime)
{
	normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);

	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);

	upVector = DirectX::XMVector3Normalize(upVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//X-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.x < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, -0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, -0.01f);
	}
	else if (dotValue.x > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, 0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, 0.01f);
	}

	//Z-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, rightVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.z < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, 0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, 0.01f);
	}
	else if (dotValue.z > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, -0.01f);
	}

	//Movement
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 3.0f;
	}

	if (Input::KeyDown(KeyCode::W))
	{
		position += forwardVector * deltaTime * 40.0f;
		dotProduct = DirectX::XMVector3Dot(cameraForward, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (dotValue.x < -0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > -0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (dotValue.x < 0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > 0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}

		//Walking normally
		else if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.x > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else
		{
			//Checking where it is
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}

	else if (Input::KeyDown(KeyCode::S))
	{
		position += forwardVector * deltaTime * 40.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraForward, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (dotValue.x < 0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > 0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (dotValue.x < -0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > -0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}

		//Walking normally
		else if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.x > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else
		{
			//Checking where it is
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}

	else if (Input::KeyDown(KeyCode::D))
	{
		position += forwardVector * deltaTime * 40.0f;
		dotProduct = DirectX::XMVector3Dot(cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.z > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else
		{
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}

	else if (Input::KeyDown(KeyCode::A))
	{
		position += forwardVector * deltaTime * 40.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.x > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
		else
		{
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
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
	return this->normalVector;
}

DirectX::XMVECTOR Player::getForwardVec() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Player::getRightVec() const
{
	return this->rightVector;
}