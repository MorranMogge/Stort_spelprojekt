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
[partitioning("integer")] //integer//fractional_odd//pow2//fractional_even
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
