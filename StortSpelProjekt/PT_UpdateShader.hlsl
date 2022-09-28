
//#include "particle_Include.hlsli"


RWBuffer<float> rawBuffer : register(u0);


cbuffer timeValue : register(b0)
{
    float deltaTime;
};
cbuffer posValue : register(b1)
{
    float3 offsetFromOrigin;
    float enable;
    float4 orientation;
};

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
#define offset 8
 
#define PositionX rawBuffer[DTid.x * offset]
#define PositionY rawBuffer[DTid.x * offset + 1]
#define PositionZ rawBuffer[DTid.x * offset + 2]

#define StartPositionX rawBuffer[DTid.x * offset + 3]
#define StartPositionY rawBuffer[DTid.x * offset + 4]
#define StartPositionZ rawBuffer[DTid.x * offset + 5]
    
#define SimulateTime rawBuffer[DTid.x * offset + 6]

#define LifeTime rawBuffer[DTid.x * offset + 7]

    [flatten]
    SimulateTime = SimulateTime > LifeTime || !enable ? 0.0f : SimulateTime + deltaTime;

    const float speed = 100.0f;
    const float normalizedLifeTime = SimulateTime / LifeTime;
    
    const float3 y = orientation.xyz;
    const float3 z = normalize(cross(y, float3(0, 1, 0)));
    const float3 x = normalize(cross(y, z));
    const float3x3 euler = float3x3(x, y, z);
    
    const float3 startPos = mul(euler, float3(StartPositionX, StartPositionY, StartPositionZ));
    
    PositionX = startPos.x + (speed * normalizedLifeTime * orientation.x) + offsetFromOrigin.x;
    PositionY = startPos.y + (speed * normalizedLifeTime * orientation.y) + offsetFromOrigin.y;
    PositionZ = startPos.z + (speed * normalizedLifeTime * orientation.z) + offsetFromOrigin.z;

}