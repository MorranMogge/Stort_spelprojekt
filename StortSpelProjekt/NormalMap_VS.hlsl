cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}

cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}

struct VSin
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
};

struct VSout
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;

    float3 tangent : TANGENT;

    float4 worldPosition : WorldPosition;
    float3 localPosition : LocalPosition;
};

VSout main(VSin input)
{
    VSout output;

    output.uv = input.uv;
    output.tangent = mul(float4(input.tangent, 1.0f), worldM).xyz;
    //Calculate position of vertex in world
    output.worldPosition = mul(float4(input.position, 1.0f), worldM);
    output.position = mul(output.worldPosition, camViewProjM);
    output.localPosition = input.position;


    //Calculate the normal vector against the world matrix only.
    output.normal = normalize(mul(input.normal, (float3x3) worldM));

    output.localPosition = input.position;
    return output;
}