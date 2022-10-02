struct Light
{
    float4 position;
    float4 color;
    float4 direction;
    float  angle;
    float  type;
    float  range;
    float  falloff;
    float4x4 view;
};

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

struct LightResult
{
    float4 Diffuse;
    float4 Specular;
};

float4 DoDiffuse(Light light, float3 lightDir, float3 normal)
{
    float NdotL = max(0, dot(normal, lightDir));
    return light.color * NdotL;
}
float4 DoSpecular(Light light, float3 ViewDir, float3 lightDir, float3 normal)
{
    // Phong lighting.
    float3 R = normalize(reflect(-lightDir, normal));
    float RdotV = max(0, dot(R, ViewDir));

    // Blinn-Phong lighting
    float3 H = normalize(lightDir + ViewDir);
    float NdotH = max(0, dot(normal, H));

    return light.color * pow(RdotV, mat.specularPower);
}
float DoAttenuation(Light light, float d)
{
    //return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
    return saturate(1 - d / light.range);
}

LightResult DoPointLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal)
{
    LightResult result;

    float3 L = (light.position - worldPosition).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);

    result.Diffuse = DoDiffuse(light, L, normal) * attenuation;
    result.Specular = DoSpecular(light, ViewDir, L, normal) * attenuation;

    return result;
}
LightResult DoDirectionalLight(Light light, float3 ViewDir, float3 normal)
{
    LightResult result;

    float3 lightDir = -light.direction.xyz;

    result.Diffuse = DoDiffuse(light, lightDir, normal);
    result.Specular = DoSpecular(light, ViewDir, lightDir, normal);

    return result;
}

float DoSpotCone(Light light, float3 lightDir)
{
    float minCos = cos(light.angle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction.xyz, -lightDir);
    return smoothstep(minCos, maxCos, cosAngle);
}
LightResult DoSpotLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal)
{
    LightResult result;

    float3 L = (light.position - worldPosition).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);
    float spotIntensity = DoSpotCone(light, L);

    result.Diffuse = DoDiffuse(light, L, normal) * attenuation * spotIntensity;
    result.Specular = DoSpecular(light, ViewDir, L, normal) * attenuation * spotIntensity;

    return result;
}

float ShadowIntensity(float4 lightWorldPosition, Light light, float3 LightDir, float3 worldPosition, float3 normal, int i, float shadowStrenth)
{
    lightWorldPosition.xy /= lightWorldPosition.w; //calculate x and y in texture space
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f); //re map range to 0,1
    float lightDepth = lightWorldPosition.z / lightWorldPosition.w; // Calculate depth of light.
    float shadowTexDepth = shadowMaps.SampleLevel(samplerState, float3(smTex.xy, i), 0).r;
    float shadowIntensity = 1.0f;
    
    // fixing floating point precision issues by subtract 0.001 from the light Depth
    //lightDepth -= 0.000005f;
    lightDepth -= max(0.01f * (1.0f - dot(normal, LightDir)), 0.005f);
    
    if (shadowTexDepth > lightDepth)
    {
        float3 vectorToLight = normalize(light.position.xyz - worldPosition);
	    // Calculate amount of light on this pixel.
        shadowIntensity = saturate(dot(vectorToLight, normal) * shadowStrenth);
    }
    return shadowIntensity;
}
float ShadowFactor(float4 fragmentPositionInLightSpace, int i, float3 normal, float3 LightDir)
{
    float3 projectedCoordinates = fragmentPositionInLightSpace.xyz / fragmentPositionInLightSpace.w;
    projectedCoordinates = 0.5f * projectedCoordinates + 0.5f;
    
    float fragmentDepth = projectedCoordinates.z;
    
    float2 texSize;
    shadowMaps.GetDimensions(texSize.x, texSize.y, i);
    texSize = 1 / texSize;
    
    float result = 0.0f;
    
    int PCFsize = 3;
    int K = PCFsize / 2;
    
    float bias = max(0.01f * (1.0f - dot(normal, LightDir)), 0.005f);
    
    for (int x = -K; x <= K; x++)
    {
        for (int y = -K; y <= K; y++)
        {
            float PCFDepth = shadowMaps.SampleLevel(samplerState, float3(projectedCoordinates.xy + float2(x, y) * texSize, 0), i).r;
            result += ((fragmentDepth - bias) > PCFDepth) ? 1.0 : 0.0f;
        }
    }
    
    result /= float(PCFsize * PCFsize);
    return result;
}


float4 main(float4 position : SV_POSITION, float3 normal : NORMAL, float2 uv : UV, float4 worldPosition : WorldPosition) : SV_TARGET
{
    float4 ambient = ambientTex.Sample(samplerState, uv) * mat.ambient;
    float4 diffuseColor = diffuseTex.Sample(samplerState, uv);
    float4 specular = specularTex.Sample(samplerState, uv) * float4(mat.specular, 0);

    float3 viewDir = normalize(cameraPosition.xyz - worldPosition.xyz);

    LightResult litResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    for (int i = 0; i < nrOfLights; ++i)
    {
        //if (!lights[i].enabled)
        //    continue;
        
        #define POINT_LIGHT 0
        #define DIRECTIONAL_LIGHT 1
        #define SPOT_LIGHT 2
        
        LightResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
        switch (lights[i].type)
        {
            case DIRECTIONAL_LIGHT:
                result = DoDirectionalLight(lights[i], viewDir, normal); break;
            case POINT_LIGHT:
                result = DoPointLight(lights[i], viewDir, worldPosition, normal); break;
            case SPOT_LIGHT:
                result = DoSpotLight(lights[i], viewDir, worldPosition, normal); break;
        }
        litResult.Diffuse += result.Diffuse;
        litResult.Specular += result.Specular;
    }
    
    return saturate(ambient + mat.diffuse * litResult.Diffuse + specular * litResult.Specular) * diffuseColor;
}

