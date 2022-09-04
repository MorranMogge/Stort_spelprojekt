#include "Vertex.hlsli"

cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}
cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}

void main
(in Vertex input,

out float4 position : SV_Position, 
out float2 uv : UV, 
out float3 normal : Normal, 
out float4 worldPosition : WorldPosition)
{
    uv = input.uv;
    
    //Calculate position of vertex in world
    worldPosition = mul(float4(input.position, 1.0f), worldM);
    position = mul(worldPosition, camViewProjM);
    
    //Calculate the normal vector against the world matrix only.
    normal = normalize(mul(input.normal, (float3x3) worldM));
}