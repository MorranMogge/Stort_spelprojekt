
cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
};

cbuffer Constants : register(b1)
{
    float4x4 CamViewProj;
};

float4 main(float3 iPos : Position) : SV_Position
{
    float4 pos = mul(float4(iPos, 1.0f), worldM);
    pos = mul(pos, CamViewProj);
    return pos;
}