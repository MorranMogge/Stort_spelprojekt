#pragma once

#include <SimpleMath.h>
#include <DirectXCollision.h>

struct Bound
{
	DirectX::SimpleMath::Vector3 min{ FLT_MAX, FLT_MAX, FLT_MAX };
	DirectX::SimpleMath::Vector3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

	DirectX::SimpleMath::Vector3 center; // world space center position
	DirectX::SimpleMath::Vector3 width; // width of bounding box

	DirectX::BoundingBox aabb;

	bool Intersects(const Bound& other) const
	{
		return aabb.Intersects(other.aabb);
	}
	bool Intersects(const DirectX::BoundingBox& other) const
	{
		return aabb.Intersects(other);
	}
	bool Intersects(const DirectX::BoundingFrustum& other) const
	{
		return aabb.Intersects(other);
	}
	bool Intersects(const DirectX::BoundingOrientedBox& other) const
	{
		return aabb.Intersects(other);
	}
	bool Intersects(const DirectX::BoundingSphere& other) const
	{
		return aabb.Intersects(other);
	}

};