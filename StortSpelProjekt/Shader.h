#pragma once

#include "VertexShader.h"
#include "PixelShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "ComputeShader.h"
#include "GeometryShader.h"

struct Shader final
{
	//Vertex shaders
	inline static VertexShader vs_NoTess;
	inline static VertexShader vs_Tess;
	inline static VertexShader vs_Shadow;
	inline static VertexShader vs_Billboard;
	inline static VertexShader vs_CubeD;
	inline static VertexShader vs_QuadTree;

	//Pixel Shaders
	inline static PixelShader ps;
	inline static PixelShader ps_Billboard;
	inline static PixelShader ps_Cube;
	inline static PixelShader ps_CubeD;
	inline static PixelShader ps_Color;

	//Compute Shaders
	inline static ComputeShader cs;
	inline static ComputeShader cs_Particle;

	//Hull Shaders
	inline static HullShader hs;

	//Domain Shaders
	inline static DomainShader ds;

	//Geometry Shaders
	inline static GeometryShader gs_Billboard;


	static void LoadAll()
	{
		LoadVS();
		LoadPS();
		LoadCS();
		LoadHS();
		LoadDS();
		LoadGS();
	}

	static void UnBindVS()
	{
		GPU::immediateContext->VSSetShader(nullptr, nullptr, 0);
	}
	static void UnBindPS()
	{
		GPU::immediateContext->PSSetShader(nullptr, nullptr, 0);
	}
	static void UnBindCS()
	{
		GPU::immediateContext->CSSetShader(nullptr, nullptr, 0);
	}
	static void UnBindHS()
	{
		GPU::immediateContext->HSSetShader(nullptr, nullptr, 0);
	}
	static void UnBindDS()
	{
		GPU::immediateContext->DSSetShader(nullptr, nullptr, 0);
	}
	static void UnBindGS()
	{
		GPU::immediateContext->GSSetShader(nullptr, nullptr, 0);
	}

private:

	static void LoadVS()
	{
		D3D11_INPUT_ELEMENT_DESC position{
			position.SemanticName = "Position",
			position.SemanticIndex = 0U,//semantic index, index of the vertex shader input, only have 1, so index 0
			position.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			position.InputSlot = 0U,
			position.AlignedByteOffset = 0U,//aligned byte offset, off set in the entaire element the structer from begining of the structer,0 is beginning in bytes,
			position.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			position.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};
		D3D11_INPUT_ELEMENT_DESC uv{
			uv.SemanticName = "UV",
			uv.SemanticIndex = 0U,//semantic index, index of the vertex shader input, we only have 1, so index 0
			uv.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
			uv.InputSlot = 0U,
			uv.AlignedByteOffset = 12U,//sizeof(float) * 3
			uv.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			uv.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};
		D3D11_INPUT_ELEMENT_DESC normal{
			normal.SemanticName = "Normal",
			normal.SemanticIndex = 0U,//semantic index, index of the vertex shader input, in this case, we only have 1, so index 0
			normal.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			normal.InputSlot = 0U,
			normal.AlignedByteOffset = 20U,//sizeof(float) * 5
			normal.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			normal.InstanceDataStepRate = 0U,//instance data step rate, for instance rendering, one single for a lot rendering
		};

		const D3D11_INPUT_ELEMENT_DESC PosUvNormal[]{ position, uv, normal };
		const D3D11_INPUT_ELEMENT_DESC PosNormal[]{ position, normal };

		vs_NoTess.Load("vs", PosUvNormal, 3u);
		vs_Tess.Load("vs_Tess", PosUvNormal, 3u);

		vs_Shadow.Load("vs_Shadow", &position, 1u);
		vs_Billboard.Load("vs_Billboard", &position, 1u);

		vs_CubeD.Load("vs_CubeD", PosNormal, 2u);

		vs_QuadTree.Load("vs_QuadTree", &position, 1u);
	}
	static void LoadPS()
	{
		ps.Load("ps");
		ps_Billboard.Load("ps_Billboard");
		ps_Cube.Load("ps_Cube");
		ps_CubeD.Load("ps_CubeD");
		ps_Color.Load("ps_Color");
	}
	static void LoadCS()
	{
		cs.Load("cs");
		cs_Particle.Load("cs_Particle");
	}
	static void LoadHS()
	{
		hs.Load("hs");
	}
	static void LoadDS()
	{
		ds.Load("ds");
	}
	static void LoadGS()
	{
		gs_Billboard.Load("gs_Billboard");
	}

};
