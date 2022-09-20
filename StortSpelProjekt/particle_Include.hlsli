struct tas
{
    float3 pos;
    float deltaTime;
    float lifetime;
};

RWBuffer<float> particlePositions : register(u0);

cbuffer timeValue : register(b0)
{
    float deltaTime;
    float time;
};
cbuffer posValue : register(b1)
{
    float3 offset;
    bool enable;
};