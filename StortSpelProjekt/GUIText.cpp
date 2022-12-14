#include "stdafx.h"
#include "GUIText.h"
#include <SpriteFont.h>

GUIText::GUIText()
{

}

GUIText::GUIText(const float x, const float y, const float layer)
{
    m_Position = DirectX::SimpleMath::Vector2(x, y);
    m_Scale = DirectX::SimpleMath::Vector2(1, 1);
    m_Tint = DirectX::Colors::White.v;  //.v - xmvextor should be able to store in it
    m_Alpha = 1.0f;
    m_Rotation = 0.0f;
    m_Layer = layer;
}

GUIText::GUIText(const DirectX::SimpleMath::Vector2& position, const float layer)
{
    m_Position = position;
    m_Scale = DirectX::SimpleMath::Vector2(1, 1);
    m_Tint = DirectX::Colors::White.v;  //.v - xmvextor should be able to store in it
    m_Alpha = 1.0f;
    m_Rotation = 0.0f;
    m_Layer = layer;
}

void GUIText::setText(const wchar_t* text)
{
    m_text = text;
    //spriteFont->MeasureDrawBounds(m_text.c_str(), m_Position);
}

void GUIText::Load(ID3D11Device* device, const wchar_t* file)
{
    //Import texture
    //DirectX::CreateDDSTextureFromFile(device, file, &m_pResource, &m_pTexture);
    //Utility::GetTextureDimentions(m_pResource, &m_Width, &m_Height);

    spriteFont = std::make_unique<DirectX::SpriteFont>(device, file);

    //spriteFont->MeasureDrawBounds(m_text.c_str(), m_Position);

    m_Origin = DirectX::SimpleMath::Vector2(0.0f, 0.0f); //origin = center of texture also important to rotate as rotates around centrepoint
    m_SourceRect.left = 0;
    m_SourceRect.top = 0;
    m_SourceRect.right = m_Width;
    m_SourceRect.bottom = m_Height;

}

const DirectX::SimpleMath::Vector2 GUIText::GetPosition() const
{
    return m_Position;
}

const DirectX::SimpleMath::Vector2& GUIText::GetOrigin() const
{
    return m_Origin;
}

const DirectX::SimpleMath::Vector2& GUIText::GetScale() const
{
    return m_Scale;
}

const DirectX::SimpleMath::Color& GUIText::GetTint() const
{
    return m_Tint;
}

const float GUIText::GetAlpha() const
{
    return m_Alpha;
}

const float GUIText::GetRotation() const
{
    return m_Rotation;
}

const float GUIText::GetWidth() const
{
    return m_Width * m_Scale.x;
}

const float GUIText::GetHeight() const
{
    return m_Height * m_Scale.y;
}

void GUIText::SetPosition(const DirectX::SimpleMath::Vector2& position)
{
    m_Position = position;
}


void GUIText::SetOrigin(const DirectX::SimpleMath::Vector2& origin)
{
    m_Origin = origin;
}

void GUIText::SetScale(const DirectX::SimpleMath::Vector2& scale)
{
    m_Scale = scale;
}

void GUIText::SetTint(const DirectX::SimpleMath::Color& color)
{
    m_Tint = color;
}

void GUIText::SetAlpha(const float alpha)
{
    m_Alpha = alpha;
}

void GUIText::SetRotation(const float rotation)
{
    m_Rotation = rotation;
}

void GUIText::Draw()
{
    spriteFont->DrawString(GUI::spriteBatch.get(), m_text.c_str(), m_Position, m_Tint, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects_None, m_Layer);
}