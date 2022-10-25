struct Output
{
    float4 position : SV_Position;
    float2 uv : UV;
    float faloff : FALOFF;
};

cbuffer CamViewProjCB : register(b0)
{
    float4x4 mainCamViewProj;
}
cbuffer CamPosCB : register(b1)
{
    float4 mainCamPos;
}

struct Particle
{
    float3 pos : POSITION;
    float faloff : FALOFF;
};

#define Half 4.5f

[maxvertexcount(4)]
void main(point Particle input[1], inout TriangleStream<Output> outputStream)
{

    static const float2 uv[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    static const float3 up = { 0.0f, 1.0f, 0.0f }; //up vector

    const float3 normal = normalize(input[0].pos - mainCamPos.xyz); //plan normal
    const float3 right = -normalize(cross(normal, up)); //right vector

    const float3 worldPos[4] = {
        { input[0].pos - right * Half + up * Half }, // top left
        { input[0].pos + right * Half + up * Half }, // top right
        { input[0].pos - right * Half - up * Half }, // bottom left
        { input[0].pos + right * Half - up * Half }, // bottom right
    };


    [unroll]
    for (int i = 0; i < 4; i++)
    {
        const Output output = {
            mul(float4(worldPos[i], 1.0f), mainCamViewProj),
            uv[i],
            input[0].faloff,
        };
        outputStream.Append(output);
    }

}