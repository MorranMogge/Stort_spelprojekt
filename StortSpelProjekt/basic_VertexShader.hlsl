struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

Vertex main(Vertex vertex)
{
    return vertex;
}