struct lightStruct
{
    float4 lightPos;
    float4 lightColor;
    float4 lightDirection;
    float4 angleTypePadding;
    float4x4 ltViewMatix;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float3 specular;
    float specularPower;
};

struct PSin
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WorldPosition;
};

Texture2D diffuseTex : register(t0);
Texture2D ambientTex : register(t1);
Texture2D specularTex : register(t2);
Texture2DArray shadowMaps : register(t3);
StructuredBuffer<lightStruct> lightStructBuff : register(t4); //Structured buffer with Light data
SamplerState samplerState : register(s0);


//cbuffer Mat : register(b0)
//{
//    Material mat;
//};


cbuffer numLightBuffer : register(b0)
{
    int nrOfLights;
};


cbuffer cameraValue : register(b1)
{
    float4 cameraPosition;
};



float GetShadowIntensity(int i, float4 lightWorldPosition, float shadowStrenth, float3 worldPosition, float3 normal)
{
    lightWorldPosition.xy /= lightWorldPosition.w;
    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
    // Calculate depth of light.
    float lightDepth = lightWorldPosition.z / lightWorldPosition.w;
    
    float shadowTexDepth = shadowMaps.SampleLevel(samplerState, float3(smTex.xy, 0), i).r;
    float shadowIntensity = 0.0f;
    
    // fixing floating point precision issues by subtract 0.001 from the light Depth
    lightDepth -= 0.00001f;
    if (lightDepth < shadowTexDepth)
    {
        float3 lightDirection = normalize(lightStructBuff[i].lightPos.xyz - worldPosition);
        
	    // Calculate amount of light on this pixel.
        shadowIntensity = saturate(dot(lightDirection, normal) * shadowStrenth);
    }
    
    return shadowIntensity;
}


float FallOff(float range, float distance)
{
    return saturate(1 - distance / range); // normalize distance in range, linear falloff
}

struct LitResult
{
    float3 diffuse;
    float3 specular;
};

LitResult GetPointL(int i, float specularExponent, float3 specularColor, float3 worldPosition, float3 normal, float3 viewDirection)
{
    LitResult result = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

    float3 lightDirection = normalize(lightStructBuff[i].lightPos.xyz - worldPosition);
    float intesity = saturate(dot(normal, lightDirection)); // dot(normal,lightDirection) ger vinkeln emellan vector och normal
    float distance = length(worldPosition - lightStructBuff[i].lightPos.xyz);
    if (distance < 100.0f /*lights[i].range*/ && intesity > 0.0f)
    {
        result.diffuse = saturate(intesity * lightStructBuff[i].lightColor.xyz) * FallOff(100.0f /*lights[i].range*/, distance);
            
        float3 reflection = normalize(2 * intesity * normal - lightDirection);
        result.specular = lightStructBuff[i].lightColor.xyz;
        result.specular *= pow(saturate(dot(reflection * specularColor, viewDirection)), specularExponent);
    }
    return result;
}


float4 main(PSin input) : SV_TARGET
{
    //variables
    //const float4 cameraPos = cameraPosition; //Camera Position    (worldspace)
    const float4 fragmentPosition = input.position; //Fragment Position  (worldspace)
    const float3 fragementNormal = input.normal; //normal for fragment
    const float4 texColor = float4(diffuseTex.Sample(samplerState, input.uv).xyz, 1.0f); //Texture Color
    
    //Placeholder/Hardcoded?//hämta in från ljuset via struct
    //float ambientStrenght =  0.1f;
    //float diffuseStrenght =  0.0f;
    float specularStrenght = 10.5f;
    
    //Changes per light
    float4 lightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 lightPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //Cumulative
    float3 diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 specular = float3(0.0f, 0.0f, 0.0f);
    //lägg till range baserad fall of på ljus
    
    float3 viewDirection = normalize(cameraPosition.xyz - fragmentPosition.xyz);
    
    LitResult finalLit = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    float shadowIntensity = 0.0f;
    
    float3 specularColor = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < nrOfLights; i++)
    {
        LitResult currentLit = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
        
        currentLit = GetPointL(i, specularStrenght /*specularExponent*/, specularColor /*specularColor.xyz*/, fragmentPosition.xyz, fragementNormal.xyz, viewDirection);

        finalLit.diffuse += currentLit.diffuse;
        finalLit.specular += currentLit.specular;
        
        float4 fragPosInLightView = mul(fragmentPosition, lightStructBuff[i].ltViewMatix);
        shadowIntensity += GetShadowIntensity(i, fragPosInLightView, 4.0f, fragmentPosition.xyz, fragementNormal.xyz);
    }
    
  
    //	------------- Ambient ---------------//
    
    
    float3 finalColor = /*ambientColor.xyz*/0.35f; //0.23
    finalColor += saturate(shadowIntensity * finalLit.diffuse);
    
    finalColor = saturate((finalLit.diffuse + finalColor) * texColor.xyz); //multiply light result with texture color
    finalColor = saturate(finalColor + finalLit.specular); //not multiply to put on top and not affect color of image

    return float4(finalColor, 1.0f);
}
