#pragma once

#include <SpriteBatch.h>
#include "DXUtil.h"
#include <SimpleMath.h>

class GUIObject
{
public:
	
	virtual ~GUIObject(){}

	//Virtual Methods

	//Load Funktion
	virtual void Load(ID3D11Device* device, const wchar_t* file) = 0; //(device, file,)


	//Get Funktions
	virtual const DirectX::SimpleMath::Vector2& GetPosition() const = 0;
	//virtual const DirectX::SimpleMath::Vector3& GetPosition() const = 0;
	virtual const DirectX::SimpleMath::Vector2& GetOrigin() const = 0;
	virtual const DirectX::SimpleMath::Vector2& GetScale() const = 0; //vector gives posibility to non-uniform scaling
	virtual const DirectX::SimpleMath::Color& GetTint() const = 0;
	virtual const float& GetAlpha() const = 0;
	virtual const float& GetRotation() const = 0;

	virtual const float GetWidth() const = 0;
	virtual const float GetHeight() const = 0;

	//Set Funktions
	virtual void SetPosition(const DirectX::SimpleMath::Vector2& position) = 0;
	//virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) = 0;
	virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin) = 0;
	virtual void SetScale(const DirectX::SimpleMath::Vector2& scale) = 0;
	virtual void SetTint(const DirectX::SimpleMath::Color& color) = 0;
	virtual void SetAlpha(const float alpha) = 0;
	virtual void SetRotation(const float rotation) = 0;

	//Render Funktion
	virtual void Draw(DirectX::SpriteBatch* spriteBatch) = 0;

private:

};

