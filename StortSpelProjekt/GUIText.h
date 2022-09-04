#pragma once
#include "GUIObject.h"
#include <SpriteFont.h>
#include <string>

class GUIText : public GUIObject
{
public:

	GUIText();
	GUIText(const DirectX::SimpleMath::Vector2& position, float	layer);
	//GUIText(const DirectX::SimpleMath::Vector3& position);
	~GUIText();


	void setText(const wchar_t* text);

	// Inherited via GUIObject
	virtual void Load(ID3D11Device* device, const wchar_t* file) override;

	virtual const DirectX::SimpleMath::Vector2& GetPosition() const override;
	//virtual const DirectX::SimpleMath::Vector3& GetPosition() const override;
	virtual const DirectX::SimpleMath::Vector2& GetOrigin() const override;
	virtual const DirectX::SimpleMath::Vector2& GetScale() const override;
	virtual const DirectX::SimpleMath::Color& GetTint() const override;

	virtual const float& GetAlpha() const override;
	virtual const float& GetRotation() const override;

	virtual const float GetWidth() const override;
	virtual const float GetHeight() const override;

	virtual void SetPosition(const DirectX::SimpleMath::Vector2& position) override;
	//virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) override;
	virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin) override;
	virtual void SetScale(const DirectX::SimpleMath::Vector2& scale) override;
	virtual void SetTint(const DirectX::SimpleMath::Color& color) override;
	virtual void SetAlpha(const float alpha) override;
	virtual void SetRotation(const float rotation) override;

	virtual void Draw(DirectX::SpriteBatch* spriteBatch) override;

protected:

	
	DirectX::SpriteFont* spriteFont;

	ID3D11Resource* m_pResource;
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

	std::wstring m_text;

};
