
//#include "particle_Include.hlsli"


RWBuffer<float> particlePositions : register(u0);


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


[numthreads(18, 1, 1)]
void main(int3 DTid : SV_DispatchThreadID)
{
    //particlePositions[DTid.x * 3] = cos(time * 0.9f + DTid.x);
    //particlePositions[DTid.x * 3 + 2] = sin(time * 0.9f + DTid.x);
    
    particlePositions[DTid.x * 5] = particlePositions[DTid.x * 5]; //+cos(time * +DTid.x);
    
    if (particlePositions[DTid.x * 5 + 1] > 15)
    {
        particlePositions[DTid.x * 5 + 1] = particlePositions[DTid.x * 5 + 1] - 25; //sin(time * 1.1f + DTid.x);
    }
    else
    {
        particlePositions[DTid.x * 5 + 1] = particlePositions[DTid.x * 5 + 1] + (deltaTime * 0.5); //sin(time * 1.1f + DTid.x);
    }
    particlePositions[DTid.x * 5 + 2] = particlePositions[DTid.x * 5 + 2]; //+sin(time * +DTid.x); //sin(time * 1.1f + DTid.x);
    
    //particlePositions[DTid.x * 5] = 0;
    //particlePositions[DTid.x * 5+1] = 0;
    //particlePositions[DTid.x * 5+2] = 0;
    
}