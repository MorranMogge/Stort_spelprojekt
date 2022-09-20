struct VSout
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WorldPosition;
};


float4 main(VSout input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}