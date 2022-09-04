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