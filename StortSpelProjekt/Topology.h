#pragma once
#include "GPU.h"

class Topology
{
public:
	enum class Type
	{
		Point_List = 1,
		Line_List = 2,
		Line_Strip = 3,
		Triangle_List = 4,
		Triangle_Strip = 5,
		Control_Point_PatchList_1 = 33,
		Control_Point_PatchList_2 = 34,
		Control_Point_PatchList_3 = 35,
		Control_Point_PatchList_4 = 36,
	};
	static void Bind(const Type type)
	{
		GPU::immediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)type);
	}
};