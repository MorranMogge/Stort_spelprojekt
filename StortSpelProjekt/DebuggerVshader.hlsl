cbuffer WorldCB : register(b0)
{
    float4x4 worldM;
}
cbuffer CamCB : register(b1)
{
    float4x4 camViewProjM;
}

struct VSin
{
    float3 position : POSITION;
    float3 colour : COLOUR;
};

struct VSout
{
    float4 position : SV_POSITION;
    float4 worldPosition : WorldPosition;
    float4 colour : Colour;
};

VSout main(VSin input)
{
    VSout output;
    //Calculate position of vertex in world
    output.worldPosition = float4(input.position,1.0f);
    output.position = mul(output.worldPosition, camViewProjM);

    //Calculate the normal vector against the world matrix only.
    output.colour = float4(input.colour,1.0f);
    //output.normal = normalize(mul(input.normal, (float3x3) worldM));
    return output;
}