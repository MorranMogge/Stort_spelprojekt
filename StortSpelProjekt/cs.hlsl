#include "Light.hlsli"

RWTexture2D<unorm float4> backBuffer;

SamplerState shadowMapSampler : register(s0);

Texture2D positionTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D colorTex : register(t2);

Texture2D ambientTex : register(t3);
Texture2D diffuseTex : register(t4);
Texture2D specularTex : register(t5);
Texture2D specularPowerTex : register(t6);
Texture2D lPositionTex : register(t7);

Texture2DArray shadowMapTexArray : register(t8);

cbuffer LightsCB : register(b0)
{
    Light lights[MaxLights];
}

cbuffer CamPosCB : register(b1)
{
    float4 camPos;
}

[numthreads(32, 18, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    Material mat;
    
    const float3 color = colorTex.Load(DTid).xyz;
    const float3 worldPosition = positionTex.Load(DTid).xyz;
    const float3 normal = normalTex.Load(DTid).xyz;
    //const float4 lPosition = lPositionTex.Load(DTid);
    
    const float3 viewDirection = normalize(camPos.xyz - worldPosition);
    
    mat.ambient = ambientTex.Load(DTid);
    mat.diffuse = diffuseTex.Load(DTid);
    mat.specular = specularTex.Load(DTid).xyz;
    mat.specularPower = max(specularPowerTex.Load(DTid).r, 8.0f);
    
    LitResult finalLit = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    for (int i = 0; i < MaxLights; i++)
    {
        if (!lights[i].enabled)
            continue;
        
        LitResult currentLit = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
        float4 lightWorldPosition = mul(float4(worldPosition, 1.0f), lights[i].lightCamViewProj);
        
        lightWorldPosition.xy /= lightWorldPosition.w; //calculate x and y in texture space, projected xy
        const float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f); //re map range to 0,1
        const float lightDepth = lightWorldPosition.z / lightWorldPosition.w; // Calculate depth of light.
        const float shadowTexDepth = shadowMapTexArray.SampleLevel(shadowMapSampler, float3(smTex.xy, i), 0).r;
        
        //GetShadowIntensity(1, lightWorldPosition, lights[i].position.xyz, worldPosition, normal, shadowMapTexArray, shadowMapSampler, i);
        if (shadowTexDepth > lightDepth - 0.000001f) // fix floating point precision issues by subtract small value
        {
            
            switch (lights[i].type)
            {
                case PointL:
                    currentLit = GetPointL(lights[i], worldPosition, normal, viewDirection, mat.specular, mat.specularPower); break;
                case SpotL:
                    currentLit = GetSpotL(lights[i], worldPosition, normal, viewDirection, mat.specular, mat.specularPower); break;
                case DirL:
                    currentLit = GetDirL(lights[i], normal, viewDirection, mat.specular, mat.specularPower); break;
            }
            

        }
        
        finalLit.diffuse += currentLit.diffuse;
        finalLit.specular += currentLit.specular;
        
    }
    
    float3 finalColor = max(mat.ambient.xyz, 0.2f);
    finalColor = saturate((finalLit.diffuse + finalColor) * color); //multiply light result with texture color
    finalColor = saturate(finalColor + finalLit.specular); //not multiply to put on top and not affect color of image

    backBuffer[DTid.xy] = float4(finalColor, 1.0f);
}
