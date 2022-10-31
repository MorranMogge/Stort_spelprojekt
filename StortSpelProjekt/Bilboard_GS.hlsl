struct Output
{
    float4 position : SV_Position;
    float2 uv : UV;
};

cbuffer CamViewProjCB : register(b1)
{
    float4x4 camViewProj;
}
cbuffer CamPosCB : register(b2)
{
    float4 camPos;
}


cbuffer CamUp : register(b3)
{
    float4 camUp;
}

[maxvertexcount(4)]
void main(point float3 input[1] : Position, inout TriangleStream<Output> outputStream)
{
#define Size 2.0f
    static const float4 localPosition[4] = { float4(-Size, Size, 0, 1), float4(Size, Size, 0, 1), float4(-Size, -Size, 0, 1), float4(Size, -Size, 0, 1) };
    static const float2 uv[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    
    float3 zAxis = normalize(input[0] - camPos.xyz);
    float3 xAxis = normalize(cross(-zAxis, camUp.xyz));
    #define yAxis cross(zAxis, xAxis) // maybe not need normalize?
    
#define WorldMatrix float4x4(float4(xAxis, 0), float4(yAxis, 0), float4(zAxis, 0), float4(input[0], 1))
    const float4x4 worldViewProj = mul(WorldMatrix, camViewProj);
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        const Output output =
        {
            mul(localPosition[i], worldViewProj),
            uv[i],
        };
        outputStream.Append(output);
    }
    
}