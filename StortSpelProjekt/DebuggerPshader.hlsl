struct VSout
{
    float4 position : SV_POSITION;
    float4 worldPosition : WorldPosition;
    float4 colour : Colour;
};


float4 main(VSout input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}