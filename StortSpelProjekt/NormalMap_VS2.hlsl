//cbuffer WorldCB : register(b0)
//{
//    float4x4 worldM;
//}

//cbuffer CamCB : register(b1)
//{
//    float4x4 camViewProjM;
//}

struct VSin
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
};

//struct VSout
//{
//    float4 position : SV_POSITION;
//    float3 normal : NORMAL;
//    float2 uv : UV;

//    float3 tangent : TANGENT;
//};

VSin main(VSin input)
{
    return input;
}