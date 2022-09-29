Texture2D tex : register(t0);
SamplerState samplerState : register(s0);


float4 main(float4 position : SV_Position, float2 uv : UV, float delta : DELTA, float lifeTime : LIFETIME) : SV_Target0
{
    float4 color = tex.Sample(samplerState, uv);
    color.a = 
    clip(color.a < 0.1f ? -1 : 1); //if alpha is too low, then we just clip it, otherwise we just pass trough
    return color;
}