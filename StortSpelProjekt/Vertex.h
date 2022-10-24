#pragma once
#include <SimpleMath.h>

struct Vertex
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;

	bool operator==(const Vertex& v) const
	{
		return position == v.position && uv == v.uv && normal == v.normal;
	}
	bool operator!=(const Vertex& v) const
	{
		return position != v.position || uv != v.uv || normal != v.normal;
	}
};

struct VertexAnim
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;

	DirectX::XMFLOAT4 boneID;
	DirectX::XMFLOAT4 weight;

	bool operator==(const Vertex& v) const
	{
		return position == v.position && uv == v.uv && normal == v.normal;
	}
	bool operator!=(const Vertex& v) const
	{
		return position != v.position || uv != v.uv || normal != v.normal;
	}
};