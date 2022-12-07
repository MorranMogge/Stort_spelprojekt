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
    float3 localPosition : LocalPosition;
};

VSout main(VShaderIn input)
{
    VSout output;
    
    const float4 startPosition = float4(input.position, 1.0f);
    float4x4 boneTransformation = float4x4(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    );
    boneTransformation += Tx[input.boneIndex[0]] * input.weights[0];
    boneTransformation += Tx[input.boneIndex[1]] * input.weights[1];
    boneTransformation += Tx[input.boneIndex[2]] * input.weights[2];
    boneTransformation += Tx[input.boneIndex[3]] * input.weights[3];

    output.uv = input.uv;

    output.position = mul(float4(input.position, 1.0f), boneTransformation);
    output.position = mul(output.position, worldM);
    output.worldPosition = output.position;
    output.position = mul(output.position, camViewProjM);
    

    //Calculate the normal vector against the world matrix only.
    //output.normal = mul(float4(input.normal, 0.0f), boneTransformation).xyz;
    //output.normal = mul(float4(output.normal, 0.0f), worldM).xyz;
    //output.normal = normalize(output.normal);
    //output.normal = -output.normal;
    
    float4x4 projectedSkinning = mul(boneTransformation , worldM);
    
    //output.normal = input.normal; //normalize(mul(input.normal, (float3x3) boneTransformation));
    output.normal = normalize(mul(input.normal, (float3x3) projectedSkinning));
    output.localPosition = input.position;
    
    return output;
}