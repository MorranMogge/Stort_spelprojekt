#include "stdafx.h"
#include "GUISprite.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include "ImGui/imgui.h"

GUISprite::GUISprite()
{
    BaseWidth = GPU::windowWidth / 1264.0f;
    BaseHeight = GPU::windowHeight / 681.0f;
}

GUISprite::GUISprite(const float x, const float y, const float layer)
{
    BaseWidth = GPU::windowWidth / 1264.0f;
    BaseHeight = GPU::windowHeight / 681.0f;
    
    m_Position = DirectX::SimpleMath::Vector2(BaseWidth * x, BaseHeight * y);
    m_Layer = layer;
    m_Scale = { BaseWidth * 1, BaseWidth * 1 };            //same scale as object
    m_Tint = DirectX::Colors::White.v;  //.v - xmvextor should be able to store in it
    m_Alpha = 1.0f;
    m_Rotation = 0.0f;
}

GUISprite::GUISprite(const DirectX::SimpleMath::Vector2& position, float layer)
{
    BaseWidth = GPU::windowWidth / 1264.0f;
    BaseHeight = GPU::windowHeight / 681.0f;

    m_Position = position;
    m_Position.x *= BaseWidth;
    m_Position.y *= BaseHeight;
    m_Layer = layer;
    m_Scale = { BaseWidth * 1, BaseWidth * 1 };            //same scale as object
    m_Tint = DirectX::Colors::White.v;  //.v - xmvextor should be able to store in it
    m_Alpha = 1.0f;
    m_Rotation = 0.0f;

}

void GUISprite::Load(ID3D11Device* device, const wchar_t* file)
{
    //Import texture
    //DirectX::CreateDDSTextureFromFile(device, file, &m_pResource, &m_pTexture);
    DirectX::CreateWICTextureFromFile(device, file, &m_pResource, &m_pTexture);
    Utility::GetTextureDimentions(m_pResource.Get(), &m_Width, &m_Height);
    m_Origin = DirectX::SimpleMath::Vector2(m_Width / 2.0f, m_Height / 2.0f); //origin = center of texture also important to rotate as rotates around centrepoint
    m_SourceRect.left = 0;
    m_SourceRect.top = 0;
    m_SourceRect.right = m_Width;
    m_SourceRect.bottom = m_Height;

}

const DirectX::SimpleMath::Vector2& GUISprite::GetPosition() const
{
    return m_Position;
}

const DirectX::SimpleMath::Vector2& GUISprite::GetOrigin() const
{
    return m_Origin;
}

const DirectX::SimpleMath::Vector2& GUISprite::GetScale() const
{
    return m_Scale;
}

const DirectX::SimpleMath::Color& GUISprite::GetTint() const
{
    return m_Tint;
}

const float GUISprite::GetAlpha() const
{
    return m_Alpha;
}

const float GUISprite::GetRotation() const
{
    return m_Rotation;
}

const float GUISprite::GetWidth() const
{
    return m_Width * m_Scale.x;
}

const float GUISprite::GetHeight() const
{
    return m_Height * m_Scale.y;
}

void GUISprite::SetPosition(const DirectX::SimpleMath::Vector2& position)
{
    m_Position = position;
    m_Position *= BaseWidth;
    m_Position *= BaseHeight;
}

void GUISprite::SetOrigin(const DirectX::SimpleMath::Vector2& origin)
{
    m_Origin = origin;
}

void GUISprite::SetScale(const DirectX::SimpleMath::Vector2& scale)
{
    m_Scale = scale;
    m_Scale.x *= BaseWidth;
    m_Scale.y *= BaseHeight;
}
void GUISprite::SetScale(const float x, const float y)
{
    m_Scale = { BaseWidth * x, BaseWidth * y };
}

void GUISprite::SetTint(const DirectX::SimpleMath::Color& color)
{
    m_Tint = color;
}

void GUISprite::SetAlpha(const float alpha)
{
    m_Alpha = alpha;
}

void GUISprite::SetRotation(const float rotation)
{
    m_Rotation = rotation;
}

void GUISprite::Draw()
{
    GUI::spriteBatch->Draw(m_pTexture.Get(), m_Position, &m_SourceRect, m_Tint, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects_None, m_Layer);
}

bool GUISprite::IntersectMouse() const
{
    //static POINT mousePos;
    //if (GetCursorPos(&mousePos))
    //{
    //    if (ScreenToClient(GetActiveWindow()/*FindWindowW(L"Window Class", L"Projekt")*/, &mousePos))
    //    {
    //    }
    //}

    ImVec2 mousePos = ImGui::GetMousePos();
#define InsideX mousePos.x > m_Position.x * BaseWidth - (m_Width * m_Scale.x / 2.0f) * BaseWidth && mousePos.x < m_Position.x * BaseWidth + (m_Width * m_Scale.x / 2.0f) * BaseWidth
#define InsideY mousePos.y > m_Position.y * BaseHeight - (m_Height * m_Scale.y / 2.0f) * BaseHeight && mousePos.y < m_Position.y * BaseHeight + (m_Height * m_Scale.y / 2.0f) * BaseHeight
    return InsideX && InsideY;
}