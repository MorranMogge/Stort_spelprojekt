#include "Light.hlsli"

Texture2D tex : register(t0);
Texture2DArray shadowMapTexArray : register(t3);

SamplerState texSampler : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer MaterialsCB : register(b0)
{
    Material mat;
};

cbuffer LightsCB : register(b1)
{
    Light lights[MaxLights];
}

cbuffer CamPosCB : register(b2)
{
    float4 camPos;
}

float4 main(float4 position : SV_Position, float2 uv : UV, float3 normal : Normal, float4 worldPosition : WorldPosition) : SV_Target0
{
    float3 color = tex.Sample(texSampler, uv).xyz;
    float3 viewDirection = normalize(camPos.xyz - worldPosition.xyz);
    
    LitResult finalLit = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    [unroll]
    for (int i = 0; i < MaxLights; i++)
    {
        if (!lights[i].enabled)
            continue;
        
        LitResult currentLit = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
        float4 lightWorldPosition = mul(float4(worldPosition.xyz, 1.0f), lights[i].lightCamViewProj);
        
        lightWorldPosition.xy /= lightWorldPosition.w; //calculate x and y in texture space, projected xy
        float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f); //re map range to 0,1
        float lightDepth = lightWorldPosition.z / lightWorldPosition.w; // Calculate depth of light.
        float shadowTexDepth = shadowMapTexArray.SampleLevel(shadowSampler, float3(smTex.xy, i), 0).r;
        
        if (shadowTexDepth > lightDepth - 0.000001f) // fix floating point precision issues by subtract small value
        {
            switch (lights[i].type)
            {
                case PointL:
                    currentLit = GetPointL(lights[i], worldPosition.xyz, normal, viewDirection, mat.specular, max(mat.specularPower, 8.0f)); break;
                case SpotL:
                    currentLit = GetSpotL(lights[i], worldPosition.xyz, normal, viewDirection, mat.specular, max(mat.specularPower, 8.0f)); break;
                case DirL:
                    currentLit = GetDirL(lights[i], normal, viewDirection, mat.specular, max(mat.specularPower, 8.0f)); break;
            }

            finalLit.diffuse += currentLit.diffuse;
            finalLit.specular += currentLit.specular;
        }
    }
    
    float3 finalColor = max(mat.ambient.xyz, 0.2f);
    
    finalColor = saturate((finalLit.diffuse + finalColor) * color); //multiply light result with texture color
    finalColor = saturate(finalColor + finalLit.specular); //not multiply to put on top and not affect color of image

    return float4(finalColor, 1.0f);
}