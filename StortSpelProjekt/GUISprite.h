#pragma once
#include "GUIObject.h"

class GUISprite :public GUIObject
{
public:

	GUISprite(void);
	GUISprite(const DirectX::SimpleMath::Vector2& position, float layer);
	//GUISprite(const DirectX::SimpleMath::Vector3& position);
	~GUISprite(void);

	
	// Inherited via IElement2D

	//Load
	virtual void Load(ID3D11Device* device, const wchar_t* file) override;

	//Get Funktions
	virtual const DirectX::SimpleMath::Vector2& GetPosition() const;
	//virtual const DirectX::SimpleMath::Vector3& GetPosition() const;
	virtual const DirectX::SimpleMath::Vector2& GetOrigin() const;
	virtual const DirectX::SimpleMath::Vector2& GetScale() const;
	virtual const DirectX::SimpleMath::Color& GetTint() const;
	virtual const float& GetAlpha() const;
	virtual const float& GetRotation() const;
	virtual const float GetWidth() const;
	virtual const float GetHeight() const;

	//Set
	virtual void SetPosition(const DirectX::SimpleMath::Vector2& position);
	//virtual void SetPosition(const DirectX::SimpleMath::Vector3& position);
	virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin);
	virtual void SetScale(const DirectX::SimpleMath::Vector2& scale);
	virtual void SetTint(const DirectX::SimpleMath::Color& color);
	virtual void SetAlpha(const float alpha);
	virtual void SetRotation(const float rotation);

	//Render
	virtual void Draw(DirectX::SpriteBatch* spriteBatch);

protected:

	ID3D11Resource* m_pResource;
	ID3D11ShaderResourceView* m_pTexture;
	UINT		m_Width;
	UINT		m_Height;
	RECT		m_SourceRect;

	

	DirectX::SimpleMath::Vector2  m_Position;
	//DirectX::SimpleMath::Vector3  m_Position;
	
	DirectX::SimpleMath::Vector2  m_Origin;
	DirectX::SimpleMath::Vector2  m_Scale;
	DirectX::SimpleMath::Color	 m_Tint;
	float    m_Alpha;
	float    m_Rotation;
	float	 m_Layer;

};

