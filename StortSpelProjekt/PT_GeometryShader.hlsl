struct Output
{
    float4 position : SV_Position;
    float2 uv : UV;

    //float3 normal : Normal;
    //float4 worldPosition : WorldPosition;
};

cbuffer CamViewProjCB : register(b0)
{
    float4x4 mainCamViewProj;
}
cbuffer CamPosCB : register(b1)
{
    float4 mainCamPos;
}

#define Half 0.5f

[maxvertexcount(4)]
void main(point float3 input[1] : Position, inout TriangleStream<Output> outputStream)
{

    static const float2 uv[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
    static const float3 up = { 0.0f, 1.0f, 0.0f }; //up vector

    const float3 normal = normalize(input[0] - mainCamPos.xyz); //plan normal
    const float3 right = -normalize(cross(normal, up)); //right vector

    const float3 worldPos[4] = {
        { input[0] - right * Half + up * Half }, // top left
        { input[0] + right * Half + up * Half }, // top right
        { input[0] - right * Half - up * Half }, // bottom left
        { input[0] + right * Half - up * Half }, // bottom right
    };


    [unroll]
    for (int i = 0; i < 4; i++)
    {
        const Output output = {
            mul(float4(worldPos[i], 1.0f), mainCamViewProj),
            uv[i],

            //normal,
            //float4(worldPos[i], 0.0f), //worldPosition
        };
        outputStream.Append(output);
    }

}