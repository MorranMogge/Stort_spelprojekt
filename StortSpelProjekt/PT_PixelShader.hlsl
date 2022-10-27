Texture2D tex : register(t0);
SamplerState samplerState : register(s0);


float4 main(float4 position : SV_Position, float2 uv : UV, float faloff : FALOFF, float ison : ISON) : SV_Target0
{
    float4 color = tex.Sample(samplerState, uv);
    color.a *= faloff;
    
    // 
    //clip(color.a < 0.05f ? -1 : 1); //if alpha is too low, then we just clip it, otherwise we just pass trough
    return color;
}
//    float2 smTex = float2(0.5f * lightWorldPosition.x + 0.5f, -0.5f * lightWorldPosition.y + 0.5f);
