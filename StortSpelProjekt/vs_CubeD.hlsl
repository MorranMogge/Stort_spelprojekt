
cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}
cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}
cbuffer CamPosCB : register(b2)
{
    float4 camPos;
}

struct VertexOut
{
    float4 position : SV_Position;
    float3 reflectVector : ReflectVector;
};

VertexOut main(float3 position : Position, float3 normal : Normal)
{
    VertexOut output;
    
    output.reflectVector = normalize(camPos.xyz - position);
    output.reflectVector = reflect(output.reflectVector, normal);
    
    output.position = mul(float4(position, 1.0f), worldM);
    output.position = mul(output.position, camViewProjM);
    
    return output;
}