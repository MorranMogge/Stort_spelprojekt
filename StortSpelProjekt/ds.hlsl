struct Vertex
{
    float3 position : Position;
    float2 uv : UV;
    float3 normal : Normal;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float4 worldPosition : WorldPosition;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3] : SV_TessFactor;
	float InsideTessFactor : SV_InsideTessFactor;
};


cbuffer CB0 : register(b0)
{
    float4x4 worldMatrix;
}
cbuffer CB1 : register(b1)
{
    float4x4 camViewProj;
}


[domain("tri")]
PSInput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 uvw : SV_DomainLocation,
	const OutputPatch<Vertex, 3> tri)
{
    PSInput output = (PSInput) 0;

    float4 position = float4(uvw.x * tri[0].position + uvw.y * tri[1].position + uvw.z * tri[2].position, 1.0f);
    output.worldPosition = mul(position, worldMatrix);
    output.position = mul(output.worldPosition, camViewProj);

    output.normal = mul(float4(uvw.x * tri[0].normal + uvw.y * tri[1].normal + uvw.z * tri[2].normal, 0.0f), worldMatrix).xyz;
    
    output.uv = uvw.x * tri[0].uv + uvw.y * tri[1].uv + uvw.z * tri[2].uv;

    return output;
}