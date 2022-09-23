struct ParticleStruct
{
    float3 pos : POSITION;
    float delta : DELTA;
    float lifeTime : LIFETIME;
};


ParticleStruct main(ParticleStruct input)
{
    return input;
}