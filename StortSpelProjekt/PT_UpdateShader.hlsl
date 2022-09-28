
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
    float3 orientation;
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

    [flatten]// ugly attribute for performance to flat the if else
    SimulateTime = SimulateTime > LifeTime || !enable ? 0.0f : SimulateTime + deltaTime; 

#define SpeedFactor 100.0f
    const float speed = 1.0f * SpeedFactor;
    
    const float normalizedLifeTime = SimulateTime / LifeTime; // re-map simulate time in 0 to 1
    
    const float3 z = normalize(cross(orientation, float3(0.0f, 1.0f, 0.0f))); // euler Z, cross orientation and up vector(0,1,0)
    const float3 x = normalize(cross(orientation, z)); // euler X, cross orientation and euler Z
    const float3x3 euler = float3x3(x, orientation, z); // apply it to euler matrix
    
    const float3 position = 
        mul(euler, float3(StartPositionX, StartPositionY, StartPositionZ))// start position in local space
        + (speed * normalizedLifeTime * orientation) // + curren offset in local space
        + offsetFromOrigin; // + offset from origin = world space
    
    PositionX = position.x;
    PositionY = position.y;
    PositionZ = position.z;

}