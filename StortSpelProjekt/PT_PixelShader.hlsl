Texture2D tex : register(t0);
SamplerState samplerState : register(s0);


float4 main(float4 position : SV_Position, float2 uv : UV, float delta : DELTA, float lifeTime : LIFETIME) : SV_Target0
{
    float4 color = tex.Sample(samplerState, uv);
    float deltaT = delta;
    float lifeT = lifeTime;
    color.a = (1 - deltaT / lifeT);
    
    
    // 
    clip(color.a < 0.1f ? -1 : 1); //if alpha is too low, then we just clip it, otherwise we just pass trough
    return color;
}
//    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
