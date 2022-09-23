
//#include "particle_Include.hlsli"


RWBuffer<float> rawBuffer : register(u0);


cbuffer timeValue : register(b0)
{
    float deltaTime;
};
cbuffer posValue : register(b1)
{
    float3 offsetFromOrigin;
    bool enable;
    float4 orientation;
};

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
#define offset 5
 
#define PositionX rawBuffer[DTid.x * offset]
#define PositionY rawBuffer[DTid.x * offset + 1]
#define PositionZ rawBuffer[DTid.x * offset + 2]

#define SimulateTime rawBuffer[DTid.x * offset + 3]

#define LifeTime rawBuffer[DTid.x * offset + 4]

    
    SimulateTime += deltaTime;

    const float speed = 1.0f * 0.05f;
    
    if (SimulateTime > LifeTime)
    {
        SimulateTime -= LifeTime;
        
        PositionX = 0.0f;
        PositionY = 0.0f;
        PositionZ = 0.0f;
    }
    else
    {
        PositionX += speed * orientation.x;
        PositionY += speed * orientation.y;
        PositionZ += speed * orientation.z;
    }

}