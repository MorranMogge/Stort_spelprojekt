
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


[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
#define offset 5

#define PositionX rawBuffer[DTid.x * offset]
#define PositionY rawBuffer[DTid.x * offset + 1]
#define PositionZ rawBuffer[DTid.x * offset + 2]

#define SimulateTime rawBuffer[DTid.x * offset + 3]

#define LifeTime rawBuffer[DTid.x * offset + 4]


    SimulateTime += deltaTime;

    const float speed = 1.0f;

    // remap time value, in range of 0 to 1
    //const float normalizedLifeTime = SimulateTime / LifeTime;

    float4 position;
    if (SimulateTime > LifeTime)
    {
        SimulateTime -= LifeTime;

        // reset position
        position = float4(offsetFromOrigin, 1.0f);
    }
    else
    {
        //store xyz in a variable for math
        position = float4(PositionX, PositionY, PositionZ, 0.0f);

        //convert world pos to local pos
        position -= float4(offsetFromOrigin,0);

        //convert orientation to local orientation
        position *= -orientation;

        position += speed;

        //back to world pos
        position += float4(offsetFromOrigin, 0.0f);
        position *= orientation;

    }

    //apply position to particle position
    PositionX = position.x;
    PositionY = position.y;
    PositionZ = position.z;

}