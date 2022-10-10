#include "Light.hlsli"

struct Material
{
    float4 ambient;
    float4 diffuse;
    float3 specular;
    float specularPower;
};

Texture2D diffuseTex : register(t0);
Texture2D ambientTex : register(t1);
Texture2D specularTex : register(t2);
Texture2DArray shadowMaps : register(t3);
StructuredBuffer<Light> lights : register(t4); //Structured buffer with Light data
SamplerState samplerState : register(s0);

cbuffer Mat : register(b0)
{
    Material mat;
};

cbuffer cameraValue : register(b1)
{
    float4 cameraPosition;
};

cbuffer numLightBuffer : register(b2)
{
    int nrOfLights;
};


float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float4 worldPosition : WorldPosition) : SV_TARGET
{
    float3 diffuseColor = diffuseTex.Sample(samplerState, uv).xyz;
    return float4(diffuseColor, 1.0f);
}