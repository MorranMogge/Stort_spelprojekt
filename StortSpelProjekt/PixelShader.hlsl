
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
Texture2D depthTex : register(t5);
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


float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float4 worldPosition : WorldPosition, float3 localPosition : LocalPosition) : SV_TARGET
{

    //if (depthTex.Sample(samplerState, uv).r == 1.0f)
    //{
    //    float3 reflectVector = normalize(cameraPosition.xyz - localPosition);
    //    reflectVector = reflect(reflectVector, normal);
    //    return txCube.Sample(samplerState, reflectVector);
    //}
    
    
    float3 ambient = ambientTex.Sample(samplerState, uv).xyz * mat.ambient.xyz;
    float3 diffuseColor = diffuseTex.Sample(samplerState, uv).xyz;
    float3 specular = specularTex.Sample(samplerState, uv).xyz * mat.specular;
    float3 viewDir = normalize(cameraPosition.xyz - worldPosition.xyz);

    
    LightResult litResult = { { 0, 0, 0 }, { 0, 0, 0 } };
    for (int i = 0; i < nrOfLights; ++i)
    {
        //if (!lights[i].enabled)
        //    continue;
        
        #define POINT_LIGHT 0
        #define DIRECTIONAL_LIGHT 1
        #define SPOT_LIGHT 2
        
        LightResult result = { { 0, 0, 0}, { 0, 0, 0 } };
        switch (lights[i].type)
        {
            case DIRECTIONAL_LIGHT:
                result = DoDirectionalLight(lights[i], viewDir, normal, mat.specularPower); break;
            case POINT_LIGHT:
                result = DoPointLight(lights[i], viewDir, worldPosition, normal, mat.specularPower); break;
            case SPOT_LIGHT:
                result = DoSpotLight(lights[i], viewDir, worldPosition, normal, mat.specularPower); break;
        }
        litResult.Diffuse += result.Diffuse;
        litResult.Specular += result.Specular;
    }
    
    return float4(saturate(ambient + mat.diffuse.xyz * litResult.Diffuse + specular * litResult.Specular) * diffuseColor, 1.0f);
}

