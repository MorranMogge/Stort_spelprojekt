
cbuffer SimpelColor : register(b0)
{
    float4 color;
}

float4 main(float4 position : SV_Position) : SV_TARGET
{
    return color;
}