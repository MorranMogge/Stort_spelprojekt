struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

//constant output
struct HS_CONSTANT_DATA_OUTPUT
{
    float Edges[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT ConstantHS(
	InputPatch<Vertex, 3> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    #define factor 3
    Output.Edges[0] = factor;
    Output.Edges[1] = factor;
    Output.Edges[2] = factor;
    Output.Inside = factor;

    return Output;
}

[domain("tri")]
[partitioning("fractional_odd")] //integer//fractional_odd//pow2//fractional_even
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)] //how many times this fuction runs
[patchconstantfunc("ConstantHS")]
Vertex main(
	InputPatch<Vertex, 3> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    return ip[i];
}
