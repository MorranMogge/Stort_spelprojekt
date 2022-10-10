cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}

cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}

struct VShaderIn
{
    float4x4 boneTrasforms[4] : BONETRANSFORMATION;
	float weights[4] : BONEWEIGHT;
	
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct VSout
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WorldPosition;
};

float4 main(VShaderIn input)
{
    VSout output;
    
	float4x4 boneTransform = input.boneTrasforms[0] * input.weights[0];
	boneTransform += input.boneTrasforms[1] * input.weights[1];
	boneTransform += input.boneTrasforms[2] * input.weights[2];
	boneTransform += input.boneTrasforms[3] * input.weights[3];

    output.uv = input.uv;

    //Calculate position of vertex in world
    output.worldPosition = mul(boneTransform, mul(float4(input.position, 1.0f), worldM));
    output.position = mul(output.worldPosition, camViewProjM);

    //Calculate the normal vector against the world matrix only.
    output.normal = normalize(mul(input.normal, (float3x3) worldM));
    return output;
}