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
    const float4 cameraPos = cameraPosition;        //Camera Position    (worldspace)
    const float4 fragmentPosition = input.position; //Fragment Position  (worldspace)
    const float3 fragementNormal = input.normal;    //normal for fragment
    const float4 ambientColor = float4(ambientTex.Sample(samplerState, input.uv).xyz, 1.0f);    //Ambient Color
    const float4 texColor = float4(diffuseTex.Sample(samplerState, input.uv).xyz, 1.0f);        //Diffuse Color
    const float4 specColor = float4(specularTex.Sample(samplerState, input.uv).xyz, 1.0f);      //Specular Color
    const float2 UVpos = input.uv; //UV !!UNUSED!!

    
    //Placeholder/Hardcoded//hämta in från ljuset via struct
    float specularExponent = 10.0f; //Spc Exp
    float specularStrenght = 10.5f;
    float ambientStrenght = 0.1f;
    
    //Changes per light
    float4 lightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 lightPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 fragToLight = float3(0.0f, 0.0f, 0.0f);
    
    //Cumulative
    float3 diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 specular = float3(0.0f, 0.0f, 0.0f);
    int lightType = 0;
    float spotCone = 0.0f;
    float4 direction;
    float attenuation = 0.0f;
    
    
    
    for (int i = 0; i < nrOfLights; i++)
    {
        lightColor = lightStructBuff[i].lightColor;
        lightPos = lightStructBuff[i].lightPos;
        lightType = lightStructBuff[i].angleTypePadding.x;
        spotCone = lightStructBuff[i].angleTypePadding.y;
        direction = lightStructBuff[i].lightDirection;

        
        // -------------Shading--------------- //
        
        //fragments position transformed into lights view->clip space, then convert to ndc by dividing by w. Z channel represents depth from light
        float4 fragPosInLightView = mul(fragmentPosition, lightStructBuff[i].ltViewMatix);
        fragPosInLightView.xy /= fragPosInLightView.w;
        float depth = fragPosInLightView.z / fragPosInLightView.w;
        
        //modify xy value range from (-1 to 1) to (0 to 1)
        float2 smTex = float2(0.5f * fragPosInLightView.x + 0.5f, -0.5f * fragPosInLightView.y + 0.5f);
    
        //get depth at pos in shadow map and compare to fragPos depth. Offset added to remove shadow acne.
        if (shadowMaps.SampleLevel(samplerState, float3(smTex.xy, 0), i).r > depth - 0.0005)
        {
            // ------------- Spotlight ------------------//
            float cone = dot(fragToLight, normalize(direction.xyz)); //skalärptodukt mellan vektorn från ljus till fragment och ljusets riktning
            
            // ------------- Diffuse --------------  //

            //normalized vector from fragment to light
            if (lightType == 1)
            {
                fragToLight = normalize(-direction.xyz);
            }
            else
            {
                fragToLight = normalize(lightPos.xyz - fragmentPosition.xyz);
            }

	        //calculate diffuse light
            float diff = max(dot((fragToLight), fragementNormal), 0.0);
            
            //multiply with light color
            
            if (lightType == 2)
            {
                diffuse += (diff * lightColor) * cone;
            }
            else
            {
                diffuse += diff * lightColor; //diffuseStrenght * (lightColor*?) (diff * diffuseColor) 
            }
            //	------------- Specular -------------- //

            //vector from fragment to camera
            const float3 fragToCam = normalize(cameraPos.xyz - fragmentPosition.xyz);
    
            //reflected vector
            float3 reflectedLight = normalize(reflect(-fragToLight, fragementNormal));

            //phong type
            float spec = pow(max(dot(fragToCam, reflectedLight), 0.0f), specularExponent);
    
            if (lightType == 2)
            {
                specular += (specularStrenght * spec * lightColor) * cone;
            }
            else
            {
                specular += specularStrenght * spec * lightColor; //specularStrenght * (lightColor*?) (spec * specularColor)?
            }

        }
    }
    
  
    //	------------- Ambient ---------------//
    
    const float3 ambient = ambientStrenght * lightColor; //ambientStrenght * (lightColor*) ambientColor (ta bort strenght?)
   
    //Combine Light for fragment
    

   
    const float4 resultColor = float4((diffuse + specular), 0);
    float4 test = (float4(ambient, 0.0f) + resultColor);

    return float4(texColor);
}
