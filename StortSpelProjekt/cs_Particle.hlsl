RWBuffer<float> particlePositions : register(u0);

cbuffer timeValue : register(b0)
{
    float time;
};

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    particlePositions[DTid.x * 3] = cos(time * 0.1f * DTid.x);
    
    if (particlePositions[DTid.x * 3 + 1] > 15.0f) // if y position is greater than 15
    {
        particlePositions[DTid.x * 3 + 1] = particlePositions[DTid.x * 3 + 1] - 25; //reset position
    }
    else
    {
        particlePositions[DTid.x * 3 + 1] += (1 - (sin(time * 3.1415926 * DTid.x) * 0.5f + 0.5f)) * 0.003f;
    }
    
    particlePositions[DTid.x * 3 + 2] = sin(time * 0.1f * DTid.x);
}