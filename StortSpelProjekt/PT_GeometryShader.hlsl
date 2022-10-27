struct Output
{
    float4 position : SV_Position;
    float2 uv : UV;
    float faloff : FALOFF;
    float ison : ISON;
};

cbuffer CamViewProjCB : register(b0)
{
    float4x4 mainCamViewProj;
}
cbuffer CamPosCB : register(b1)
{
    float4 mainCamPos;
}

cbuffer CamUp : register(b3)
{
    float4 camUp;
}

struct Particle
{
    float3 pos : POSITION;
    float faloff : FALOFF;
    float ison : ISON;
};


[maxvertexcount(4)]
void main(point Particle input[1], inout TriangleStream<Output> outputStream)
{

#define Size (3.0f * input[0].faloff)
    const float4 localPosition[4] = { float4(-Size, Size, 0, 1), float4(Size, Size, 0, 1), float4(-Size, -Size, 0, 1), float4(Size, -Size, 0, 1) };
    static const float2 uv[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    
    float3 zAxis = normalize(input[0].pos - mainCamPos.xyz);
    float3 xAxis = normalize(cross(-zAxis, camUp.xyz));
#define yAxis cross(zAxis, xAxis) // maybe not need normalize?
    
#define WorldMatrix float4x4(float4(xAxis, 0), float4(yAxis, 0), float4(zAxis, 0), float4(input[0].pos, 1))
    const float4x4 worldViewProj = mul(WorldMatrix, mainCamViewProj);
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        const Output output =
        {
            
            mul(localPosition[i], worldViewProj),
            uv[i],
            input[0].faloff,
            input[0].ison,
        };
        outputStream.Append(output);
    }

}