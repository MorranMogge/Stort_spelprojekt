
struct Light
{
    float4 position;
    float4 color;
    float4 direction;
    float angle;
    float type;
    float range;
    float falloff;
    float4x4 view;
};

struct LightResult
{
    float3 Diffuse;
    float3 Specular;
};




float3 DoDiffuse(Light light, float3 lightDir, float3 normal)
{
    float NdotL = max(0, dot(normal, lightDir));
    return light.color.xyz * NdotL;
}
float3 DoSpecular(Light light, float3 ViewDir, float3 lightDir, float3 normal, float specularPower)
{
    // Phong lighting.
    float3 R = normalize(reflect(-lightDir, normal));
    float RdotV = max(0, dot(R, ViewDir));

    // Blinn-Phong lighting
    float3 H = normalize(lightDir + ViewDir);
    float NdotH = max(0, dot(normal, H));

    return light.color.xyz * pow(RdotV, 0 /*specularPower*/);
}
float DoAttenuation(Light light, float d)
{
    //return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
    return saturate((1 - d / light.range) + (1 - light.falloff));
}

LightResult DoPointLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal, float specularPower)
{
    float3 L = (light.position - worldPosition).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);

    LightResult result = {
        DoDiffuse(light, L, normal) * attenuation,
        DoSpecular(light, ViewDir, L, normal, specularPower) * attenuation
    };

    return result;
}
LightResult DoDirectionalLight(Light light, float3 ViewDir, float3 normal, float specularPower)
{
    LightResult result;

    float3 lightDir = -light.direction.xyz;

    result.Diffuse = DoDiffuse(light, lightDir, normal);
    result.Specular = DoSpecular(light, ViewDir, lightDir, normal, specularPower);

    return result;
}

float DoSpotCone(Light light, float3 lightDir)
{
    float minCos = cos(light.angle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction.xyz, -lightDir);
    return smoothstep(minCos, maxCos, cosAngle);
}
LightResult DoSpotLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal, float specularPower)
{
    LightResult result;

    float3 L = (light.position - worldPosition).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);
    float spotIntensity = DoSpotCone(light, L);

    result.Diffuse = DoDiffuse(light, L, normal) * attenuation * spotIntensity;
    result.Specular = DoSpecular(light, ViewDir, L, normal, specularPower) * attenuation * spotIntensity;

    return result;
}

float ShadowIntensity(float4 lightWorldPosition, Light light, float3 LightDir, float3 worldPosition, float3 normal, int i, float shadowStrenth, Texture2DArray shadowMaps, SamplerState samplerState)
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
float ShadowFactor(float4 fragmentPositionInLightSpace, int i, float3 normal, float3 LightDir, Texture2DArray shadowMaps, SamplerState samplerState)
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