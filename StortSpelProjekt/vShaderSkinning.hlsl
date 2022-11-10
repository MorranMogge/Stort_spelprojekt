cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}

cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}

StructuredBuffer<float4x4> Tx : register(t0);

struct VShaderIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    
    uint4 boneIndex : BONEINDEX;
    float4 weights : WEIGHT;
};

struct VSout
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WorldPosition;
};

VSout main(VShaderIn input)
{
    VSout output;
    
    const float4 startPosition = float4(input.position, 1.0f);
    float3 sumPos = float3(0, 0, 0);
    sumPos += mul(startPosition, Tx[input.boneIndex[0]] * input.weights[0]);
    sumPos += mul(startPosition, Tx[input.boneIndex[1]] * input.weights[1]);
    sumPos += mul(startPosition, Tx[input.boneIndex[2]] * input.weights[2]);
    sumPos += mul(startPosition, Tx[input.boneIndex[3]] * input.weights[3]);

    output.uv = input.uv;

    //Calculate position of vertex in world
    output.worldPosition = mul(float4(sumPos, 1.0f), worldM);
    //output.worldPosition = mul(float4(input.position, 1.0f), worldM);
    output.position = mul(output.worldPosition, camViewProjM);

    //Calculate the normal vector against the world matrix only.
    output.normal = normalize(mul(input.normal, (float3x3) worldM));
    return output;
}