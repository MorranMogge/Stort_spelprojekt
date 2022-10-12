
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

float FresnelEffect(float3 Normal, float3 ViewDir, float Power)
{
    return pow((1.0 - saturate(dot(normalize(Normal), normalize(ViewDir)))), Power);
}

float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float4 worldPosition : WorldPosition, float3 localPosition : LocalPosition) : SV_TARGET
{
    
    if (screenDepth.Sample(samplerState, uv).r == 1.0f)
    {
        float3 reflectVector = normalize(cameraPosition.xyz - localPosition);
        reflectVector = reflect(reflectVector, normal);
        return texCube.Sample(samplerState, reflectVector);
    }
    
    float3 ambient = ambientTex.Sample(samplerState, uv).xyz * mat.ambient.xyz;
    float3 diffuseColor = diffuseTex.Sample(samplerState, uv).xyz;
    float3 specular = specularTex.Sample(samplerState, uv).xyz * mat.specular;

    float3 viewDir = normalize(cameraPosition.xyz - worldPosition.xyz);

    float shadow = 0;
    
    LightResult litResult = { { 0, 0, 0 }, { 0, 0, 0 } };
    for (int i = 0; i < nrOfLights; ++i)
    {
        //if (!lights[i].enabled)
        //    continue;
        
        float4 lightWorldPosition = mul(worldPosition, lights[i].view);
        
        //shadow += ShadowIntensity(lightWorldPosition, lights[i], lightDirection, worldPosition.xyz, normal, i, shadowMaps, samplerState);
        //float shadowIntensity = ShadowIntensity(lightWorldPosition, lights[i], lightDirection, worldPosition.xyz, normal, i, shadowStrenth, shadowMaps, samplerState);
        //float shadowIntensity = ShadowIntensity(lightWorldPosition, lights[i], lightDirection, worldPosition.xyz, normal, i, shadowStrenth, shadowMaps, samplerState);
        //shadow += ShadowIntensity(lightWorldPosition, lights[i], lightDirection, worldPosition.xyz, normal, i, shadowStrenth, shadowMaps, samplerState);
        //float shadowIntensity = ShadowIntensity2(lightWorldPosition, lights[i], lightDirection, worldPosition.xyz, normal, i, shadowMaps, samplerState);
        //float shadowFactor = ShadowFactor(lightWorldPosition, i, normal, lightDirection, shadowMaps, samplerState);
        
        #define POINT_LIGHT 0
        #define DIRECTIONAL_LIGHT 1
        #define SPOT_LIGHT 2
        
        LightResult result = { { 0, 0, 0}, { 0, 0, 0 } };
        float3 lightDir;
        
        switch (lights[i].type)
        {
            case DIRECTIONAL_LIGHT:
                lightDir = normalize(-lights[i].direction.xyz);
                //result = DoDirectionalLight(lights[i], viewDir, normal, mat.specularPower, lightDir);
                //result = GetDirL(lights[i], normal, viewDir, specular, mat.specularPower, lightDir);
                //result = GetDirL(lights[i], normal, viewDir, specular, mat.specularPower, lightDir);
                result = ComputeDirectionalLight(lights[i], lightDir, normal, viewDir, diffuseColor, specular, mat.specularPower);
                break;
            
            case POINT_LIGHT:
                lightDir = lights[i].position.xyz - worldPosition.xyz;
                //result = DoPointLight(lights[i], viewDir, worldPosition, normal, mat.specularPower, lightDir);
                //result = GetPointL(lights[i], worldPosition.xyz, normal, viewDir, specular, mat.specularPower, lightDir);
                result = ComputePointLight(lights[i], lightDir, normal, viewDir, specular, mat.specularPower);
                break;
            
            case SPOT_LIGHT:
                lightDir = lights[i].position.xyz - worldPosition.xyz;
                result = DoSpotLight(lights[i], viewDir, worldPosition, normal, mat.specularPower, lightDir);
                break;
        }
        
        float shadowFactor = ShadowFactor2(lightWorldPosition, shadowMaps, shadowSampler, i, normal, lightDir);
        
        litResult.Diffuse += result.Diffuse * shadowFactor;
        litResult.Specular += result.Specular * shadowFactor;
    }
    
    //float fres = FresnelEffect(normal, viewDir, 3);
    //float3 frescolor = { 0 * fres, 0.75 * fres, 1 * fres };
    
    //return float4(saturate(ambient + mat.diffuse.xyz * litResult.Diffuse/* + specular * (litResult.Specular/* * shadow*/) * diffuseColor/* + fres*/, 1.0f);
    
    return float4(((max(mat.ambient.xyz, 0.2f) /* + litResult.Specular*/) * diffuseColor + litResult.Diffuse)/* + frescolor*/, 1.0f);
    
    //return float4(saturate(ambient + mat.diffuse.xyz * litResult.Diffuse + specular * litResult.Specular) * diffuseColor, 1.0f);

}

