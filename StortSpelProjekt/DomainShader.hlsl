struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct PSInput
{
    float4 position : SV_Position;
    float3 normal : Normal;
    float2 uv : UV;
    float4 worldPosition : WorldPosition;
    float3 localPosition : LocalPosition;
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

float3 LerpPositionBasedOfNormalProject(float3 position, float3 vertexPosition, float3 normal)
{
    return position - dot(position - vertexPosition, normal) * normal;
}


[domain("tri")]
PSInput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 uvw : SV_DomainLocation,
	const OutputPatch<Vertex, 3> tri)
{
    PSInput output = (PSInput) 0;

    float4 position = float4(uvw.x * tri[0].position + uvw.y * tri[1].position + uvw.z * tri[2].position, 1.0f);
    float3 phongTessPos = float3(0.0f, 0.0f, 0.0f);
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        phongTessPos += (uvw[i] * LerpPositionBasedOfNormalProject(position.xyz, tri[i].position, tri[i].normal));
    }
    output.position = float4(phongTessPos, 1);
    output.localPosition = output.position.xyz;
    output.worldPosition = mul(output.position, worldMatrix);
    output.position = mul(output.worldPosition, camViewProj);

    output.normal = normalize(mul(uvw.x * tri[0].normal + uvw.y * tri[1].normal + uvw.z * tri[2].normal, (float3x3)worldMatrix));
    
    output.uv = uvw.x * tri[0].uv + uvw.y * tri[1].uv + uvw.z * tri[2].uv;

    return output;
}
