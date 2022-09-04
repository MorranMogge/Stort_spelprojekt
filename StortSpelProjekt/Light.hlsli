struct Material
{
    float4 ambient;
    float4 diffuse;
    float3 specular;
    float specularPower;
};

#define PointL 0
#define SpotL 1
#define DirL 2
#define MaxLights 4
struct Light
{
    float4 position;
    float4 direction;
    float4 color;
    
    float4x4 lightCamViewProj;
    
    float range;
    float cone;
    bool enabled;
    int type;

};

struct LitResult
{
    float3 diffuse;
    float3 specular;
};

float FallOff(float range, float distance)
{
    //return 1.0f / (1.0f + 1.0f * distance + 1.0f * distance * distance);
    return saturate(1 - distance / range); // normalize distance in range, linear falloff
}

LitResult GetPointL(Light light, float3 worldPosition, float3 normal, float3 viewDirection, float3 specularColor, float specularPower)
{
    LitResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    float3 lightDirection = normalize(light.position.xyz - worldPosition);
    float intesity = saturate(dot(normal, lightDirection)); // dot(normal,lightDirection) ger vinkeln emellan vector och normal
    float distance = length(worldPosition - light.position.xyz);
    if (distance < light.range && intesity > 0.0f)
    {
        result.diffuse = saturate(intesity * light.color.xyz) * FallOff(light.range, distance);
            
        float3 reflectionVector = normalize(2 * intesity * normal - lightDirection);
        result.specular = light.color.xyz;
        result.specular *= pow(saturate(dot(reflectionVector * specularColor, viewDirection)), specularPower);
    }
    return result;
}

LitResult GetSpotL(Light light, float3 worldPosition, float3 normal, float3 viewDirection, float3 specularColor, float specularPower)
{
    LitResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    float3 lightDirection = normalize(light.position.xyz - worldPosition); //L.. point in scene to lightsource 
    float intesity = saturate(dot(normal, lightDirection)); // dot(normal,lightvector) ger vinkeln emellan vector och normal 
    float distance = length(worldPosition - light.position.xyz);

    if (distance < light.range)
    {
        float minAngle = cos(light.cone) + 1.0f; // inverted cos cone
        float maxAngle = (minAngle + 1.0f) / 2.0f; //cos angle + 180 degrees than half of this
        float angle = dot(lightDirection, -light.direction.xyz);
        float coneIntensity = smoothstep(minAngle, maxAngle, angle);
        
        intesity *= coneIntensity;
        
        if (intesity > 0.0f)
        {
            result.diffuse += saturate(intesity * light.color.xyz) * FallOff(light.range, distance); //add on light/material 
                
            float3 reflection = normalize(2 * intesity * normal - lightDirection);
            result.specular = light.color.xyz;
            result.specular *= pow(saturate(dot(reflection * specularColor, viewDirection)), specularPower);
        }
    }

    return result;
}

LitResult GetDirL(Light light, float3 normal, float3 viewDirection, float3 specularColor, float specularPower)
{
    LitResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    float3 lightDirection = -light.direction.xyz;
    float dirLIntesity = saturate(dot(normal, lightDirection));
    if (dirLIntesity > 0.0f)
    {
        result.diffuse = saturate(light.color.xyz * dirLIntesity);
        float3 reflection = normalize(2 * dirLIntesity * normal - lightDirection);
        result.specular = light.color.xyz;
        result.specular *= pow(saturate(dot(reflection * specularColor, viewDirection)), specularPower);
    }

    return result;
}


float GetShadowIntensity(float shadowStrenth, float4 lightWorldPosition, float3 lightPosition, float3 worldPosition, float3 normal, Texture2DArray shadowMap, SamplerState shadowMapSampler, int index)
{
    lightWorldPosition.xy /= lightWorldPosition.w; //calculate x and y in texture space
    
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f); //re map range to 0,1
    
    float lightDepth = lightWorldPosition.z / lightWorldPosition.w; // Calculate depth of light.
    
    //float smap_Size = 1024;
    //float dx = 1.0f / smap_Size;
    //float s0 = (shadowMap.Sample(shadowMapSampler, smTex + float2(0.0f, 0.0f)).r + 0.001f < lightDepth) ? 0.0f : 1.0f;
    //float s1 = (shadowMap.Sample(shadowMapSampler, smTex + float2(dx, 0.0f)).r + 0.001f < lightDepth) ? 0.0f : 1.0f;
    //float s2 = (shadowMap.Sample(shadowMapSampler, smTex + float2(0.0f, dx)).r + 0.001f < lightDepth) ? 0.0f : 1.0f;
    //float s3 = (shadowMap.Sample(shadowMapSampler, smTex + float2(dx, dx)).r + 0.001f < lightDepth) ? 0.0f : 1.0f;
    
    //float2 texelPos = smTex * smap_Size;
    //float2 lerps = frac(texelPos);
    //float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);
    ////float3 litColor = textureColor.rgb * shadowCoeff;
    //return shadowCoeff;
    
    float shadowTexDepth = shadowMap.SampleLevel(shadowMapSampler, float3(smTex.xy, index), 0).r;
    //float shadowTexDepth = shadowMap.SampleCmpLevelZero(shadowMapSampler, float3(smTex.xy, 0), index).r;
    
    float shadowIntensity = 1.0f;
    
    // fixing floating point precision issues by subtract 0.001 from the light Depth
    lightDepth -= 0.000005f;
    
    if (shadowTexDepth > lightDepth)
    {
        float3 vectorToLight = normalize(lightPosition - worldPosition);
        
	    // Calculate amount of light on this pixel.
        shadowIntensity = saturate(dot(vectorToLight, normal) * shadowStrenth);
    }
    
    return shadowIntensity;
}