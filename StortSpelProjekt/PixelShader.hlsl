struct lightStruct
{
    float4 lightPos;
    float4 lightColor;
    float4 lightDirection;
    float4 angleTypePadding;
    float4x4 ltViewMatix;
};

Texture2D diffuseTex : register(t0);
Texture2D ambientTex : register(t1);
Texture2D specularTex : register(t2);
Texture2DArray shadowMaps : register(t3);
StructuredBuffer<lightStruct> lightStructBuff : register(t4); //Structured buffer with Light data
SamplerState samplerState : register(s0);


struct Material
{
    float4 ambient;
    float4 diffuse;
    float3 specular;
    float specularPower;
};

cbuffer Mat : register(b0)
{
    Material mat;
};

cbuffer numLightBuffer : register(b1)
{
    int lightNr;
};

struct PSin
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WorldPosition;
};

float4 main(PSin input) : SV_TARGET
{
    float4 texColour = float4(diffuseTex.Sample(samplerState, input.uv).xyz, 1.0f);
    float4 position = float4(input.worldPosition.xyz, 0.0f);
    float4 normal = float4(input.normal, 0.0f);

    float4 ambientColor = ambientTex.Sample(samplerState, input.uv);
    float4 specularColor = specularTex.Sample(samplerState, input.uv);

    float4 ambient = mat.ambient * ambientColor;
   /* output.diffuse = float4(mat.diffuse.xyz, 0.0f);
    output.specular = float4(mat.specular, 0.0f) * specularColor;*/

    float4 colour = texColour;

    return colour;
}