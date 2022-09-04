
Texture2D diffuseTex : register(t0);
Texture2D ambientTex : register(t1);
Texture2D specularTex : register(t2);

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

struct Output
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
    
    float4 ambient : SV_Target3;
    float4 diffuse : SV_Target4;
    float4 specular : SV_Target5;
    float4 specularPower : SV_Target6;
    
    //float4 lPosition : SV_Target7;
};

Output main(float4 position : SV_Position, float2 uv : UV, float3 normal : Normal, float4 worldPosition : WorldPosition)
{
    Output output;
    
    output.color = float4(diffuseTex.Sample(samplerState, uv).xyz, 0.0f);
    output.position = float4(worldPosition.xyz, 0.0f);
    output.normal = float4(normal, 0.0f);
    
    float4 ambientColor = ambientTex.Sample(samplerState, uv);
    float4 specularColor = specularTex.Sample(samplerState, uv);
    
    output.ambient = mat.ambient * ambientColor;
    output.diffuse = float4(mat.diffuse.xyz, 0.0f);
    output.specular = float4(mat.specular, 0.0f) * specularColor;
    output.specularPower = float4(mat.specularPower, 0, 0, 0);
    
    //output.lPosition = position;
    
    return output;
}