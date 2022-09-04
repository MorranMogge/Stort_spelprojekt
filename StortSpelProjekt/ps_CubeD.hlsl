TextureCube tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(float4 position : SV_Position, float3 reflectVector : ReflectVector) : SV_Target0
{
    return float4(tex.Sample(samplerState, reflectVector));
}