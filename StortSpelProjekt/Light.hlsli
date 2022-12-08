
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

float3 DoDiffuse(float3 lightColor, float3 lightDir, float3 normal)
{
    //return max(0, dot(lightDir, normal)) * lightColor;
    float NdotL = max(0, dot(normal, lightDir));
    return lightColor * NdotL;
}
float3 DoSpecular(Light light, float3 ViewDir, float3 lightDir, float3 normal, float specularPower)
{
    // Phong lighting.
    float3 R = normalize(reflect(-lightDir, normal));
    float RdotV = max(0.0001, dot(R, ViewDir));

    // Blinn-Phong lighting
    //float3 H = normalize(lightDir + ViewDir);
    //float NdotH = max(0, dot(normal, H));

    return light.color.xyz * pow(RdotV, specularPower);
}
float DoAttenuation(Light light, float distance)
{
    //static const float attConst = 1.0f;
    //static const float attLin = 0.045f;
    //static const float attQuad = 0.0075f;
    //return 1.0f / (attConst + attLin * distance + attQuad * (distance * distance));
    return saturate(1 - distance / light.range);
}
float DoAttenuation2(float distance, float range)
{
    float att = saturate(1.0f - (distance * distance / (range * range)));
    return att * att;
}

LightResult ComputeDirectionalLight(Light L, float3 lightDir, float3 normal, float3 viewDir, float3 diffuse, float3 specular, float spacularPower)
{
    LightResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    float diffuseFactor = dot(lightDir, normal);
    
    //if (diffuseFactor < 0.25)
    //{
    //    diffuseFactor = 0;
    //}
    //else if (diffuseFactor > 0.25 && diffuseFactor < 0.5)
    //{
    //    diffuseFactor = 0.5;
    //}
    //else if (diffuseFactor > 0.5 && diffuseFactor < 0.75)
    //{
    //    diffuseFactor = 0.75;
    //}
    //else if (diffuseFactor > 0.75 && diffuseFactor < 1)
    //{
    //    diffuseFactor = 1;
    //}
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        const float3 v = reflect(-lightDir, normal);
        //const float specFactor = pow(max(dot(v, viewDir), 0.0f), spacularPower);

        result.Diffuse = diffuseFactor * diffuse * L.color.xyz;
        //result.Specular = specFactor * specular;
    }
    //result.Diffuse = DoDiffuse(L.color.xyz, lightDir, normal);
    //result.Specular = DoSpecular(L,viewDir,lightDir,normal,spacularPower);
 
    return result;
}

LightResult ComputePointLight(Light L, float3 lightDir, float3 normal, float3 viewDir, float3 specular, float spacularPower)
{
    LightResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    //const float disToL = length(lightDir);
    //lightDir /= disToL;
    //const float diffuseFactor = dot(lightDir, normal);

    //[flatten]
    //if (diffuseFactor > 0.0f)
    //{
    //    const float3 v = reflect(-lightDir, normal);
    //    const float specFactor = pow(max(dot(v, viewDir), 0.0f), spacularPower);

    //    const float attenuation = DoAttenuation(L, disToL);
    //    result.Diffuse = diffuseFactor * L.color.xyz * attenuation;
    //    result.Specular = specFactor * specular * attenuation;
    //}
    
    float distance = length(lightDir);
    lightDir /= distance;
    //float attenuation = DoAttenuation2(distance, L.range);
    float attenuation = DoAttenuation(L, distance);
    
    result.Diffuse = DoDiffuse(L.color.xyz,lightDir,normal) * attenuation;
    result.Specular = DoSpecular(L,viewDir,lightDir,normal,spacularPower) * attenuation;
    
    return result;
}

float DoSpotCone(Light light, float3 lightDir)
{
    float minCos = cos(light.angle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.direction.xyz, -lightDir);
    return smoothstep(minCos, maxCos, cosAngle);
}
LightResult DoSpotLight(Light light, float3 ViewDir, float4 worldPosition, float3 normal, float specularPower, float3 lightDir)
{
    const float distance = length(lightDir);
    lightDir /= distance;

    //const float attenuation = DoAttenuation(light, distance);
    float attenuation = DoAttenuation2(distance, light.range);
    const float spotIntensity = DoSpotCone(light, lightDir);
    
    LightResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    result.Diffuse = DoDiffuse(light.color.xyz, lightDir, normal) * attenuation * spotIntensity;
    result.Specular = DoSpecular(light, ViewDir, lightDir, normal, specularPower) * attenuation * spotIntensity;
    return result;
}


static const float2 poissonDisk16[16] =
{
    float2(-0.94201624, -0.39906216),
    float2(0.94558609, -0.76890725),
    float2(-0.094184101, -0.92938870),
    float2(0.34495938, 0.29387760),
    float2(-0.91588581, 0.45771432),
    float2(-0.81544232, -0.87912464),
    float2(-0.38277543, 0.27676845),
    float2(0.97484398, 0.75648379),
    float2(0.44323325, -0.97511554),
    float2(0.53742981, -0.47373420),
    float2(-0.26496911, -0.41893023),
    float2(0.79197514, 0.19090188),
    float2(-0.24188840, 0.99706507),
    float2(-0.81409955, 0.91437590),
    float2(0.19984126, 0.78641367),
    float2(0.14383161, -0.14100790)
};

static const float2 poissonDisk32[32] =
{
    float2(0.06407013, 0.05409927),
	float2(0.7366577, 0.5789394),
	float2(-0.6270542, -0.5320278),
	float2(-0.4096107, 0.8411095),
	float2(0.6849564, -0.4990818),
	float2(-0.874181, -0.04579735),
	float2(0.9989998, 0.0009880066),
	float2(-0.004920578, -0.9151649),
	float2(0.1805763, 0.9747483),
	float2(-0.2138451, 0.2635818),
	float2(0.109845, 0.3884785),
	float2(0.06876755, -0.3581074),
	float2(0.374073, -0.7661266),
	float2(0.3079132, -0.1216763),
	float2(-0.3794335, -0.8271583),
	float2(-0.203878, -0.07715034),
	float2(0.5912697, 0.1469799),
	float2(-0.88069, 0.3031784),
	float2(0.5040108, 0.8283722),
	float2(-0.5844124, 0.5494877),
	float2(0.6017799, -0.1726654),
	float2(-0.5554981, 0.1559997),
	float2(-0.3016369, -0.3900928),
	float2(-0.5550632, -0.1723762),
	float2(0.925029, 0.2995041),
	float2(-0.2473137, 0.5538505),
	float2(0.9183037, -0.2862392),
	float2(0.2469421, 0.6718712),
	float2(0.3916397, -0.4328209),
	float2(-0.03576927, -0.6220032),
	float2(-0.04661255, 0.7995201),
	float2(0.4402924, 0.3640312),
};

static const float2 poissonDisk64[64] =
{
    float2(0.0617981, 0.07294159),
	float2(0.6470215, 0.7474022),
	float2(-0.5987766, -0.7512833),
	float2(-0.693034, 0.6913887),
	float2(0.6987045, -0.6843052),
	float2(-0.9402866, 0.04474335),
	float2(0.8934509, 0.07369385),
	float2(0.1592735, -0.9686295),
	float2(-0.05664673, 0.995282),
	float2(-0.1203411, -0.1301079),
	float2(0.1741608, -0.1682285),
	float2(-0.09369049, 0.3196758),
	float2(0.185363, 0.3213367),
	float2(-0.1493771, -0.3147511),
	float2(0.4452095, 0.2580113),
	float2(-0.1080467, -0.5329178),
	float2(0.1604507, 0.5460774),
	float2(-0.4037193, -0.2611179),
	float2(0.5947998, -0.2146744),
	float2(0.3276062, 0.9244621),
	float2(-0.6518704, -0.2503952),
	float2(-0.3580975, 0.2806469),
	float2(0.8587891, 0.4838005),
	float2(-0.1596546, -0.8791054),
	float2(-0.3096867, 0.5588146),
	float2(-0.5128918, 0.1448544),
	float2(0.8581337, -0.424046),
	float2(0.1562584, -0.5610626),
	float2(-0.7647934, 0.2709858),
	float2(-0.3090832, 0.9020988),
	float2(0.3935608, 0.4609676),
	float2(0.3929337, -0.5010948),
	float2(-0.8682281, -0.1990303),
	float2(-0.01973724, 0.6478714),
	float2(-0.3897587, -0.4665619),
	float2(-0.7416366, -0.4377831),
	float2(-0.5523247, 0.4272514),
	float2(-0.5325066, 0.8410385),
	float2(0.3085465, -0.7842533),
	float2(0.8400612, -0.200119),
	float2(0.6632416, 0.3067062),
	float2(-0.4462856, -0.04265022),
	float2(0.06892014, 0.812484),
	float2(0.5149567, -0.7502338),
	float2(0.6464897, -0.4666451),
	float2(-0.159861, 0.1038342),
	float2(0.6455986, 0.04419327),
	float2(-0.7445076, 0.5035095),
	float2(0.9430245, 0.3139912),
	float2(0.0349884, -0.7968109),
	float2(-0.9517487, 0.2963554),
	float2(-0.7304786, -0.01006928),
	float2(-0.5862702, -0.5531025),
	float2(0.3029106, 0.09497032),
	float2(0.09025345, -0.3503742),
	float2(0.4356628, -0.0710125),
	float2(0.4112572, 0.7500054),
	float2(0.3401214, -0.3047142),
	float2(-0.2192158, -0.6911137),
	float2(-0.4676369, 0.6570358),
	float2(0.6295372, 0.5629555),
	float2(0.1253822, 0.9892166),
	float2(-0.1154335, 0.8248222),
	float2(-0.4230408, -0.7129914),
};


float DoShadow(float4 lightWorldPosition, Texture2DArray shadowMap, SamplerComparisonState shadowMapSampler, int index, float3 normal, float3 lightDirection, float softness)
{
    lightWorldPosition.xyz /= lightWorldPosition.w;
    const float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
    const float bias = max(0.0006f * (1.0 - dot(normal, normalize(lightDirection))), 0.00006f); // 0.000003f
    const float depth = lightWorldPosition.z - bias;

    //const float dx = 1.0f / 2048.0f;
    //const float2 offsets[9] =
    //{
    //    float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
    //    float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
    //    float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    //};

    //PCF(percentage-closer filtering)
    float percentLit = 0.0f;
    [unroll]
    for (int i = 0; i < 32; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(shadowMapSampler, float3(smTex + poissonDisk32[i] / softness, index), depth).r;
    }

    return percentLit / 32.0f;
}