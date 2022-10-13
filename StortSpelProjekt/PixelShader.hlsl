
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


float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float4 worldPosition : WorldPosition, float3 localPosition : LocalPosition) : SV_TARGET
{
    
    
    float3 ambient = ambientTex.Sample(samplerState, uv).xyz * mat.ambient.xyz;
    float3 diffuseColor = diffuseTex.Sample(samplerState, uv).xyz;
    float3 specular = specularTex.Sample(samplerState, uv).xyz * mat.specular;
    float3 viewDir = normalize(cameraPosition.xyz - worldPosition.xyz);

    
    LightResult litResult = { { 0, 0, 0 }, { 0, 0, 0 } };
    for (int i = 0; i < nrOfLights; ++i)
    {
        float4 lightWorldPosition = mul(worldPosition, lights[i].view);
        
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
                result = ComputeDirectionalLight(lights[i], lightDir, normal, viewDir, diffuseColor, specular, mat.specularPower);
                break;
            
            case POINT_LIGHT:
                lightDir = lights[i].position.xyz - worldPosition.xyz;
                //result = DoPointLight(lights[i], viewDir, worldPosition, normal, mat.specularPower, lightDir);
                result = ComputePointLight(lights[i], lightDir, normal, viewDir, specular, mat.specularPower);
                break;
            
            case SPOT_LIGHT:
                lightDir = lights[i].position.xyz - worldPosition.xyz;
                result = DoSpotLight(lights[i], viewDir, worldPosition, normal, mat.specularPower, lightDir);
                break;
        }
        
        float shadowFactor = HardShadow(lightWorldPosition, shadowMaps, shadowSampler, i, normal, lightDir,9);
        //float shadowFactor = SoftShadow(lightWorldPosition, 6.0, shadowMaps, shadowSampler,samplerState, i);
        
        litResult.Diffuse += result.Diffuse * shadowFactor;
        litResult.Specular += result.Specular * shadowFactor;
    }
    
    //float fres = FresnelEffect(normal, viewDir, 3);
    //float3 frescolor = { 0 * fres, 0.75 * fres, 1 * fres };
    
    
    return float4(((max(mat.ambient.xyz, 0.2f) /* + litResult.Specular*/) * diffuseColor + litResult.Diffuse)/* + frescolor*/, 1.0f);

}

