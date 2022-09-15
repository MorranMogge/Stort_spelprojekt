
cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}
cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}

StructuredBuffer<float4x4> Tx : register (t0);

struct VertexAnim
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    int4 bones : BONES;
    float4 weights : WEIGHTS;
    int joints : JOINTS;
};

void main(
in VertexAnim v,

out float4 position : SV_POSITION,
out float2 uv : UV,
out float3 normal : NORMAL,
out float4 worldPosition : WORLDPOSITION)
{
    const float4 startPosition = float4(v.position, 1.0f);
    
    float3 sumPos = float3(0, 0, 0);
    
    [unroll]
    for (int i = 0; i < v.joints; i++)
    {
        sumPos += mul(startPosition, Tx[v.bones[i]] * v.weights[i]);
    }
    
    //Calculate position of vertex in world
    worldPosition = mul(float4(sumPos, 1.0f), worldM);
    position = mul(worldPosition, camViewProjM);
    
    //Calculate the normal vector against the world matrix only.
    normal = normalize(mul(normal, (float3x3) worldM));
}