
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

float FresnelEffect(float3 Normal, float3 ViewDir, float Power)
{
    return pow((1.0 - saturate(dot(normalize(Normal), normalize(ViewDir)))), Power);
}

float FallOff(float range, float distance)
{
    //return 1.0f / (1.0f + 1.0f * distance + 1.0f * distance * distance);
    return saturate(1 - distance / range); // normalize distance in range, linear falloff
}

float3 DoDiffuse(Light light, float3 lightDir, float3 normal)
{
    float NdotL = max(0, dot(lightDir, normal));
    return NdotL * light.color.xyz;
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

float FallOff2(float3 lightVec)
{
    //return 1.0f / (1.0f + 1.0f * distance + 1.0f * distance * distance);
    return 1 / (1 + dot(lightVec, lightVec));
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
LightResult ComputePointLight(Light L, float3 lightDir, float3 normal, float3 toEye, float3 specular, float spacularPower)
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

        result.Diffuse = diffuseFactor * L.color.xyz;
        result.Specular = specFactor * specular;
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

float HardShadow(float4 lightWorldPosition, Texture2DArray shadowMap, SamplerComparisonState shadowMapSampler, int index, float3 normal, float3 lightDirection, int numSamples)
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

    return percentLit / 9.0f;
}

float FindBlocker(float2 uv, float4 Lpos, float searchWidth, float numSamples, Texture2DArray shadowMap, SamplerState shadowMapSampler, int index)
{
    float stepSize = 2.0 * searchWidth / numSamples;
	// Starting point uv coords for search
    uv -= searchWidth;
    float blockerSum = 0.0;
    float blockerCount = 0.0;
    float receiver = Lpos.z;
    // iterate through search region and add up depth values
    for (int i = 0; i < numSamples; i++)
    {
        for (int j = 0; j < numSamples; j++)
        {
            float shadMapDepth = shadowMap.SampleLevel(shadowMapSampler, float3(uv + float2(i * stepSize, j * stepSize), index), Lpos.z).r;
            
            if (shadMapDepth < receiver)
            {
                blockerSum += shadMapDepth;
                blockerCount++;
            }
        }
    }
    
    return blockerSum / blockerCount;
}

float EstimatePenumbra(float2 uv, float4 Lpos, float blocker, float lightSize)
{
    // receiver depth
    float receiver = Lpos.z;
    // estimate penumbra using parallel planes approximation
    float penumbra = (receiver - blocker) * lightSize / blocker;
    return penumbra;
}

float SoftShadow(float4 lightWorldPosition, float numSamples, Texture2DArray shadowMap, SamplerComparisonState shadowMapSampler, SamplerState samplertest,int index)
{
    lightWorldPosition.xyz /= lightWorldPosition.w;
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
    
    float blocker = FindBlocker(smTex, lightWorldPosition, 1.0, numSamples, shadowMap, samplertest, index);
    
    float penumbra = EstimatePenumbra(smTex, lightWorldPosition, blocker, 0.01);
    
    if (penumbra > 0.01)
        penumbra = 0.01;
    
    float stepSize = 2.0 * penumbra / numSamples;
    smTex -= penumbra;
    float sum = 0.0;

    for (float i = 0.0; i < numSamples; i += 1.0)
    {
        for (float j = 0.0; j < numSamples; j += 1.0)
        {
            sum += shadowMap.SampleCmpLevelZero(shadowMapSampler, float3(smTex + float2(i * stepSize, j * stepSize), index), lightWorldPosition.z - 0.00001).r;
        }
    }

    return sum / (numSamples * numSamples);
}