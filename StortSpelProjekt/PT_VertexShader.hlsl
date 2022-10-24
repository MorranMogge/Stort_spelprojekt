struct ParticleStruct
{
    float3 pos : POSITION;
    float delta : DELTA;
    float3 stpos : START_POSITION;
    float lifeTime : LIFETIME;
    float3 direction : DIRECTION;
    float3 offset : BASE_OFFSET;
};

struct Particle
{
    float3 pos : POSITION;
    float faloff : FALOFF;
};

Particle main(in ParticleStruct input)
{
    Particle output = { input.pos, saturate(1 - input.delta / input.lifeTime) };
    return output;
}