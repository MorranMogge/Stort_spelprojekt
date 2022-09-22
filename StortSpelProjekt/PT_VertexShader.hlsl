struct ParticleStruct
{
    float3 pos : POSITION;
    float delta : DELTA;
    float lifeTime : LIFETIME;
};


ParticleStruct main(ParticleStruct input)
{
    ParticleStruct output;
    output.delta = input.delta;
    output.pos = input.pos;
    output.lifeTime = input.lifeTime;

    return input;
}