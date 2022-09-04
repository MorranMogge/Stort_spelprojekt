
cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}
cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}
float4 main(float3 position : Position) : SV_Position
{
    float4 output;
    output = mul(float4(position, 1.0f), worldM);
    output = mul(output, camViewProjM);
    return output;
}