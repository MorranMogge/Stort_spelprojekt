
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
#define offset 8
 
#define PositionX rawBuffer[DTid.x * offset]
#define PositionY rawBuffer[DTid.x * offset + 1]
#define PositionZ rawBuffer[DTid.x * offset + 2]

#define StartPositionX rawBuffer[DTid.x * offset + 3]
#define StartPositionY rawBuffer[DTid.x * offset + 4]
#define StartPositionZ rawBuffer[DTid.x * offset + 5]
    
#define SimulateTime rawBuffer[DTid.x * offset + 6]

#define LifeTime rawBuffer[DTid.x * offset + 7]

    
    SimulateTime += deltaTime;

    const float speed = 10.0f * deltaTime;
    
    //transform for euler angle
    float3x3 transform;
    
    if (orientation.x == 0 && orientation.z == 0 && orientation.y < 0)
    {
        transform = float3x3(
            -1.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 1.0f);
        
    }
    else
    {
        float3 new_y = orientation;
        float3 new_z = normalize(cross(new_y, float3(0, 1, 0)));
        float3 new_x = normalize(cross(new_y, new_z));
        
        transform = float3x3(new_x, new_y, new_z);
    }
    
    
    if (SimulateTime > LifeTime)
    {
        SimulateTime -= LifeTime;
        
        float3 startPos = mul(transform, float3(StartPositionX, StartPositionY, StartPositionZ));
        
        
        PositionX = startPos.x + offsetFromOrigin.x;
        PositionY = startPos.y + offsetFromOrigin.y;
        PositionZ = startPos.z + offsetFromOrigin.z;
    }
    else
    {
        PositionX += speed * orientation.x;
        PositionY += speed * orientation.y;
        PositionZ += speed * orientation.z;
    }

}