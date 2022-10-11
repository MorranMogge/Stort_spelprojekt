
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

float FallOff(float range, float distance)
{
    //return 1.0f / (1.0f + 1.0f * distance + 1.0f * distance * distance);
    return saturate(1 - distance / range); // normalize distance in range, linear falloff
}

LightResult GetPointL(Light light, float3 worldPosition, float3 normal, float3 viewDirection, float3 specularColor, float specularPower, float3 lightDirection)
{
    LightResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    float intesity = saturate(dot(normal, lightDirection)); // dot(normal,lightDirection) ger vinkeln emellan vector och normal
    float distance = length(worldPosition - light.position.xyz);
    if (distance < light.range && intesity > 0.0f)
    {
        result.Diffuse = saturate(intesity * light.color.xyz) * FallOff(light.range, distance);

        float3 reflectionVector = normalize(2 * intesity * normal - lightDirection);
        result.Specular = light.color.xyz;
        result.Specular = pow(saturate(dot(reflectionVector*specularColor, viewDirection)),specularPower);
    }
    return result;
}

LightResult GetDirL(Light light, float3 normal, float3 viewDirection, float3 specularColor, float specularPower, float3 lightDirection)
{
    LightResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    float dirLIntesity = saturate(dot(normal, lightDirection));
    if (dirLIntesity > 0.0f)
    {
        result.Diffuse = saturate(light.color.xyz * dirLIntesity);
        float3 reflection = normalize(2 * dirLIntesity * normal - lightDirection);
        result.Specular = light.color.xyz;
        result.Specular = pow(saturate(dot(reflection * specularColor, viewDirection)), specularPower);
    }

    return result;
}


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

    return light.color.xyz * pow(NdotH, 0 /*specularPower*/);
}
float DoAttenuation(Light light, float d)
{
    //return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
    return saturate(1 - d / light.range);
}

LightResult DoPointLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal, float specularPower, float3 L)
{
    //float3 L = (light.position - worldPosition).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);

    LightResult result = {
        DoDiffuse(light, L, normal) * attenuation,
        DoSpecular(light, ViewDir, L, normal, specularPower) * attenuation
    };

    return result;
}
LightResult DoDirectionalLight(Light light, float3 ViewDir, float3 normal, float specularPower, float3 lightDir)
{
    LightResult result;

    //float3 lightDir = -light.direction.xyz;

    result.Diffuse = DoDiffuse(light, lightDir, normal);
    result.Specular = DoSpecular(light, ViewDir, lightDir, normal, specularPower);

    return result;
}

LightResult ComputeDirectionalLight(Light L, float3 lightDir, float3 normal, float3 toEye, float3 diffuse, float3 specular, float spacularPower)
{
    LightResult result = { { 0, 0, 0 }, { 0, 0, 0 } };
    float diffuseFactor = dot(lightDir, normal);
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightDir, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), spacularPower);

        result.Diffuse = diffuseFactor * diffuse * L.color.xyz;
        result.Specular = specFactor * specular /* * L.specular*/;
    }
    return result;
}
LightResult ComputePointLight(Light L, float3 lightDir, float3 normal, float3 toEye, float3 diffuse, float3 specular, float spacularPower)
{
    LightResult result = { { 0, 0, 0 }, { 0, 0, 0 } };

    float d = length(lightDir);

    if (d > L.range)
        return result;

    lightDir /= d;

    float diffuseFactor = dot(lightDir, normal);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightDir, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), spacularPower);

        result.Diffuse = diffuseFactor * diffuse /* * L.diffuse*/;
        result.Specular = specFactor * specular /* * L.specular*/;
    }

    float att = DoAttenuation(L, d); //1.0f / dot(L.att, float3(1.0f, d, d * d));

    result.Diffuse *= att;
    result.Specular *= att;
    
    return result;

}

float DoSpotCone(Light light, float3 lightDir)
{
    float minCos = cos(light.angle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction.xyz, -lightDir);
    return smoothstep(minCos, maxCos, cosAngle);
}
LightResult DoSpotLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal, float specularPower, float3 L)
{
    LightResult result;

    //float3 L = (light.position - worldPosition).xyz;
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
    lightDepth -= 0.000005f;
    //lightDepth -= max(0.01f * (1.0f - dot(normal, LightDir)), 0.005f);
    
    if (shadowTexDepth > lightDepth)
    {
        float3 vectorToLight = normalize(light.position.xyz - worldPosition);
	    // Calculate amount of light on this pixel.
        shadowIntensity = saturate(dot(vectorToLight, normal) * shadowStrenth);
        //return 1;
    }
    //else
    //{
    //    return 0;
    //}
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
    
    //float bias = max(0.01f * (1.0f - dot(normal, LightDir)), 0.005f);
    float bias = 0.000005f;
    
    for (int x = -K; x <= K; x++)
    {
        for (int y = -K; y <= K; y++)
        {
            //float PCFDepth = shadowMaps.SampleLevel(samplerState, float3(projectedCoordinates.xy + float2(x, y) * texSize, i), 0).r;
            float PCFDepth = shadowMaps.SampleLevel(samplerState, float3(projectedCoordinates.xy + float2(x, y) * texSize, i), 0).r;
            result += ((fragmentDepth - bias) > PCFDepth) ? 1.0 : 0.0f;
        }
    }
    
    result /= float(PCFsize * PCFsize);
    return result;
}

float ShadowIntensity2(float4 lightWorldPosition, Light light, float3 LightDir, float3 worldPosition, float3 normal, int i, Texture2DArray shadowMaps, SamplerState samplerState)
{
    lightWorldPosition.xy /= lightWorldPosition.w; //calculate x and y in texture space
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f); //re map range to 0,1
    float depth = lightWorldPosition.z / lightWorldPosition.w; // Calculate depth of light.
    
    float SMAP_SIZE = 2048;
    float bias = 0.000005f;
    
    //2x2 percentage closest filter. Point Sampling
    float dx = 1 / SMAP_SIZE.x;
    float s0 = shadowMaps.SampleLevel(samplerState, float3(0, 0, i), 0).r + bias < depth ? 0.0f : 1.0f;
    float s1 = shadowMaps.SampleLevel(samplerState, float3(dx, 0, i), 0).r + bias < depth ? 0.0f : 1.0f;
    float s2 = shadowMaps.SampleLevel(samplerState, float3(0, dx, i), 0).r + bias < depth ? 0.0f : 1.0f;
    float s3 = shadowMaps.SampleLevel(samplerState, float3(dx, dx, i), 0).r + bias < depth ? 0.0f : 1.0f;
    
    //transform shadow map uv coord to texel space
    float2 texelPos = smTex * SMAP_SIZE;
    
    //determine the lerp amount, if texelPos = (340.3, 200.1) then lerp = (0.3,0.1)
    float2 lerps = frac(texelPos);
    
    float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);
    
    return shadowCoeff;
}

float ShadowIntensity4(float4 lightWorldPosition, Light light, float3 LightDir, float3 worldPosition, float3 normal, int i, Texture2DArray shadowMaps, SamplerState samplerState)
{
    lightWorldPosition.xy /= lightWorldPosition.w;

    float2 smTexcoord = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
    float depth = lightWorldPosition.z / lightWorldPosition.w;
    float SHADOW_EPSILON = 0.0000001f;
    float dx = 1.0f / 2048.0f;
    float d0 = shadowMaps.SampleLevel(samplerState, float3(0, 0, i), 0).r + SHADOW_EPSILON < depth ? 0.0f : 1.0f;
    float d1 = shadowMaps.SampleLevel(samplerState, float3(dx, 0, i), 0).r + SHADOW_EPSILON < depth ? 0.0f : 1.0f;
    float d2 = shadowMaps.SampleLevel(samplerState, float3(0, dx, i), 0).r + SHADOW_EPSILON < depth ? 0.0f : 1.0f;
    float d3 = shadowMaps.SampleLevel(samplerState, float3(dx, dx, i), 0).r + SHADOW_EPSILON < depth ? 0.0f : 1.0f;

    float2 texelPos = smTexcoord * 2048.0f;
    float2 leps = frac(texelPos);
    float shadowco = (d0 + d1 + d2 + d3) / 4;
    //if (shadowco <= 0.3)
    //    shadowco = 0.3f;
    
    return saturate(shadowco);
}

float ShadowIntensity3(float4 lightWorldPosition, Light light, float3 LightDir, float3 worldPosition, float3 normal, int i, Texture2DArray shadowMaps, SamplerState samplerState)
{
    lightWorldPosition.xyz /= lightWorldPosition.w; //calculate x and y in texture space
    float depth = lightWorldPosition.z; // Calculate depth of light.
    
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f); //re map range to 0,1
    
    float bias = 0.000005f;
    
    float dx = 1.0f / 2048.0f;
    
    float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };
    
    float percentLit = 0.0f;
    [unroll]
    for (int i = 0; i < 9; i++)
    {

        percentLit += shadowMaps.SampleLevel(samplerState, float3(smTex + offsets[i], i), 0).r;
    }

    // Average the samples
    return percentLit /= 9.0f;
}

float ShadowFactor2(float4 lightWorldPosition, Texture2DArray shadowMap, SamplerComparisonState shadowMapSampler, int index, float3 normal, float3 lightDirection)
{
    lightWorldPosition.xyz /= lightWorldPosition.w;
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
    const float bias = max(0.0006f * (1.0 - dot(normal, normalize(lightDirection))), 0.00006f); // 0.000003f
    const float depth = lightWorldPosition.z - bias;

    const float dx = 1.0f / 2048.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };

    //PCF(percentage-closer filtering)
    float percentLit = 0.0f;
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(shadowMapSampler, float3(smTex/*lightWorldPosition.xy*/ + offsets[i], index), depth).r;
    }

    return percentLit /= 9.0f;
}
