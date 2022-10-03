#include "GUISprite.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

using namespace DirectX::SimpleMath;



GUISprite::GUISprite()
{
}

GUISprite::GUISprite(const float x, const float y, const float layer)
{
    m_Position = Vector2(x, y);
    m_Layer = layer;
    //m_pOrigin =;
    m_Scale = { 1, 1 };            //same scale as object
    m_Tint = DirectX::Colors::White.v;  //.v - xmvextor should be able to store in it
    m_Alpha = 1.0f;
    m_Rotation = 0.0f;
}

GUISprite::GUISprite(const Vector2& position, float layer)
{
    m_Position = position;
    m_Layer = layer;
    //m_pOrigin =;
    m_Scale = { 1, 1 };            //same scale as object
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
    m_Origin = Vector2(m_Width / 2.0f, m_Height / 2.0f); //origin = center of texture also important to rotate as rotates around centrepoint
    m_SourceRect.left = 0;
    m_SourceRect.top = 0;
    m_SourceRect.right = m_Width;
    m_SourceRect.bottom = m_Height;

}

const Vector2& GUISprite::GetPosition() const
{
    return m_Position;
}

const Vector2& GUISprite::GetOrigin() const
{
    return m_Origin;
}

const Vector2& GUISprite::GetScale() const
{
    return m_Scale;
}

const Color& GUISprite::GetTint() const
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

void GUISprite::SetPosition(const Vector2& position)
{
    m_Position = position;
}

void GUISprite::SetOrigin(const Vector2& origin)
{
    m_Origin = origin;
}

void GUISprite::SetScale(const Vector2& scale)
{
    m_Scale = scale;
}

void GUISprite::SetTint(const Color& color)
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
#pragma region GetMousePos

    static POINT mousePos;
    if (GetCursorPos(&mousePos))
    {
        if (ScreenToClient(FindWindowW(L"Window Class", L"Projekt"), &mousePos))
        {

        }
    }

#pragma endregion

#define InsideX mousePos.x > m_Position.x - (m_Width / 2.0f) && mousePos.x < m_Position.x + (m_Width / 2.0f)
#define InsideY mousePos.y > m_Position.y - (m_Height / 2.0f) && mousePos.y < m_Position.y + (m_Height / 2.0f)
#define Intersect InsideX && InsideY

    return Intersect;
}