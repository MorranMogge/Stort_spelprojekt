struct ParticleStruct
{
    float3 pos : POSITION;
    float3 stpos : START_POSITION;
    float delta : DELTA;
    float lifeTime : LIFETIME;
};


ParticleStruct main(ParticleStruct input)
{
    return input;
}