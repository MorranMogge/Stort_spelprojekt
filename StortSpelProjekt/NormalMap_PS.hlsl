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

Texture2D screenDepth : register(t5);
TextureCube texCube : register(t6);

//Normalmap
Texture2D normalTex : register(t7);

SamplerState samplerState : register(s0);
SamplerComparisonState shadowSampler : register(s1);

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

//normal map
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    float3 normalT = 2.0f * normalMapSample - 1.0f;
    
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);
    
    float3x3 TBN = float3x3(T, B, N);
    float3 bumpedNormalW = mul(normalT, TBN);
    
    return bumpedNormalW;
}

float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float3 tangent : TANGENT, float4 worldPosition : WorldPosition, float3 localPosition : LocalPosition) : SV_TARGET
{
     

    float3 ambient = ambientTex.Sample(samplerState, uv).xyz * mat.ambient.xyz;
    float3 diffuseColor = diffuseTex.Sample(samplerState, uv).xyz;
    float3 specular = specularTex.Sample(samplerState, uv).xyz * mat.specular;
    float3 viewDir = normalize(cameraPosition.xyz - worldPosition.xyz);

    
    //normal map
    float3 normalMapSample = normalTex.Sample(samplerState, uv).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, normal, tangent);

    LightResult litResult = { { 0, 0, 0 }, { 0, 0, 0 } };
    for (int i = 0; i < nrOfLights; ++i)
    {
        float4 lightWorldPosition = mul(worldPosition, lights[i].view);

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

        LightResult result = { { 0, 0, 0 }, { 0, 0, 0 } };
        float3 lightDir = float3(0, 0, 0);

        
        //ändra i light calculations för att ta hänsyn till normalMap
        switch (lights[i].type)
        {
            case DIRECTIONAL_LIGHT:
                lightDir = normalize(-lights[i].direction.xyz);
                //result = DoDirectionalLight(lights[i], viewDir, normal, mat.specularPower, lightDir);
                result = ComputeDirectionalLight(lights[i], lightDir, bumpedNormalW, viewDir, diffuseColor, specular, mat.specularPower);
                break;

            case POINT_LIGHT:
                lightDir = lights[i].position.xyz - worldPosition.xyz;
                //result = DoPointLight(lights[i], viewDir, worldPosition, normal, mat.specularPower, lightDir);
                result = ComputePointLight(lights[i], lightDir, bumpedNormalW, viewDir, specular, mat.specularPower);
                break;

            case SPOT_LIGHT:
                lightDir = lights[i].position.xyz - worldPosition.xyz;
                result = DoSpotLight(lights[i], viewDir, worldPosition, bumpedNormalW, mat.specularPower, lightDir);
                break;
        }

        float shadowFactor = DoShadow(lightWorldPosition, shadowMaps, shadowSampler, i, bumpedNormalW, lightDir, 300);
        //float shadowFactor = SoftShadow(lightWorldPosition, 6.0, shadowMaps, shadowSampler,samplerState, i);

        litResult.Diffuse += result.Diffuse * shadowFactor;
        litResult.Specular += result.Specular * shadowFactor;
    }

    float fres = FresnelEffect(normal, viewDir, 5.5);
    float3 frescolor = { 0 * fres, 0.75 * fres, 1 * fres };

    //return float4(diffuseColor, 0);
    //return float4(bumpedNormalW, 0);
    return float4(((max(mat.ambient.xyz, 0.4f) /* + litResult.Specular*/) * diffuseColor + litResult.Diffuse) + frescolor, 1.0f);

}