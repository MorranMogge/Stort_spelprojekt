struct Vertex
{
    float3 position : Position;
    float2 uv : UV;
    float3 normal : Normal;
};

//constant output
struct HS_CONSTANT_DATA_OUTPUT
{
    float Edges[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

cbuffer TessCB : register(b0)
{
    float4 factor;
};

cbuffer CamPos : register(b1)
{
    float4 camPosition;
};

cbuffer MeshPos : register(b2)
{
    float4 meshPosition;
};


HS_CONSTANT_DATA_OUTPUT ConstantHS(
	InputPatch<Vertex, 3> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    const float x = (camPosition.x - meshPosition.x) * (camPosition.x - meshPosition.x);
    const float y = (camPosition.y - meshPosition.y) * (camPosition.y - meshPosition.y);
    const float z = (camPosition.z - meshPosition.z) * (camPosition.z - meshPosition.z);
    const float disToCam = sqrt(x + y + z);
    
    if (disToCam < 10.0f)
    {
        Output.Edges[0] = factor.x;
        Output.Edges[1] = factor.y;
        Output.Edges[2] = factor.z;
        Output.Inside = factor.w;
    }
    else
    {
        Output.Edges[0] = 1;
        Output.Edges[1] = 1;
        Output.Edges[2] = 1;
        Output.Inside = 1;
    }

	return Output;
}

[domain("tri")]
[partitioning("integer")] //integer//fractional_odd//pow2//fractional_even
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]//how many times this fuction runs
[patchconstantfunc("ConstantHS")]
Vertex main(
	InputPatch<Vertex, 3> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	return ip[i];
}
