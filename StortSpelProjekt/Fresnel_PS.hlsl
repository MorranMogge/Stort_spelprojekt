
#include "Light.hlsli"

struct Material
{
    float4 ambient;
    float4 diffuse;
    float3 specular;
    float specularPower;
};


cbuffer Mat : register(b0)
{
    Material mat;
};

cbuffer cameraValue : register(b1)
{
    float4 cameraPosition;
};

cbuffer colorValue : register(b2)
{
    float4 color;
};


float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float4 worldPosition : WorldPosition, float3 localPosition : LocalPosition) : SV_TARGET
{
    const float3 viewDir = normalize(cameraPosition.xyz - worldPosition.xyz);
    float fres = FresnelEffect(normal, viewDir, 3.8);
    float4 frescolor = { 0 * fres, 0.75 * fres, 1 * fres, fres };
    
    return frescolor;
}

