RWTexture2D<unorm float4> backBuffer : register(u0); //Backbuffer texture
Texture2D backBuffer2 : register(t0);

float FxaaLuma(float3 rgb)
{
    return rgb.y * (0.587 / 0.299) + rgb.x;
}

//[numthreads(1, 1, 1)]
//void main( uint3 DTid : SV_DispatchThreadID )
//{
    
//    float3 rgbN = backBuffer[DTid.xy + uint2(0, -1)].xyz; //backBuffersam(backBuffer, DTid.xy, DTid.xy + uint2(0, -1)).xyz;
//    float3 rgbW = backBuffer[DTid.xy + uint2(-1, 0)].xyz; //FxaaTextureOffset(backBuffer, DTid.xy, DTid.xy + uint2(-1, 0)).xyz;
//    float3 rgbM = backBuffer[DTid.xy + uint2(0, 0)].xyz; //FxaaTextureOffset(backBuffer, DTid.xy, FxaaInt2(0, 0)).xyz;
//    float3 rgbE = backBuffer[DTid.xy + uint2(1, 0)].xyz; //FxaaTextureOffset(backBuffer, DTid.xy, FxaaInt2(1, 0)).xyz;
//    float3 rgbS = backBuffer[DTid.xy + uint2(0, 1)].xyz; //FxaaTextureOffset(backBuffer, DTid.xy, FxaaInt2(0, 1)).xyz;
    
//    float lumaN = FxaaLuma(rgbN);
//    float lumaW = FxaaLuma(rgbW);
//    float lumaM = FxaaLuma(rgbM);
//    float lumaE = FxaaLuma(rgbE);
//    float lumaS = FxaaLuma(rgbS);
//    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
//    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
//    float range = rangeMax - rangeMin;
   
//    float FXAA_EDGE_THRESHOLD_MIN = 1 / 16;
//    float XAA_EDGE_THRESHOLD = 1 / 8;
    
//    if (range <max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * XAA_EDGE_THRESHOLD))
//    {
//        return FxaaFilterReturn(rgbM);
//    }
//}

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float2 texCoord = DTid.xy;
    
    uint width, height, levels;
    backBuffer2.GetDimensions(width, height);
    float2 resolution = float2(width, height);

    //float FXAA_SPAN_MAX = 8.0;
    float FXAA_SPAN_MAX = 16.0;
    //float FXAA_REDUCE_MUL = 1.0 / 8.0;
    float FXAA_REDUCE_MUL = 1.0 / 16.0;
    //float FXAA_REDUCE_MIN = 1.0 / 128.0;
    float FXAA_REDUCE_MIN = 1.0 / 132.0;
    
    float3 rgbNW = backBuffer2.Load(uint3(DTid.xy + uint2(-1, -1), 0));
    float3 rgbNE = backBuffer2.Load(uint3(DTid.xy + uint2(1, -1), 0));
    float3 rgbSW = backBuffer2.Load(uint3(DTid.xy + uint2(-1, 1), 0));
    float3 rgbSE = backBuffer2.Load(uint3(DTid.xy + uint2(1, 1), 0));
    float3 rgbM = backBuffer2.Load(DTid).rgb;
   // FxaaTextureOffset(backBuffer2, DTid.xy, FxaaInt2(0, 1)).xyz;
    
    
    float3 luma = float3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);
    
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    
    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);
    
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
          max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) / resolution;
    
    float3 rgbA = (1.0 / 2.0) *
    (
        backBuffer2.Load(uint3((float2) texCoord + dir * (1.0f / 3.0f - 0.5f), 0)).rgb + backBuffer2.Load(uint3((float2) texCoord + dir * (2.0f / 3.0f - 0.5f), 0)).rgb
    ); //backBuffer.Sample(samplerTest, texCoord + dir * (1.0 / 3.0 - 0.5), 0).rgb + backBuffer.Sample(samplerTest, texCoord + dir * (2.0 / 3.0 - 0.5), 0).rgb

    
    float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (backBuffer2.Load(uint3(texCoord + dir * (0.0 / 3.0 - 0.5), 0)).rgb + backBuffer2.Load(uint3(texCoord + dir * (3.0 / 3.0 - 0.5), 0)).rgb);
    
    float lumaB = dot(rgbB, luma);

    float3 finalColor;
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
    {
        finalColor.xyz = rgbA;
    }
    else
    {
        finalColor.xyz = rgbB;
    }
    
    //backBuffer[DTid.xy] = float4(finalColor.xyz, 1.0f);
    backBuffer[DTid.xy] = float4(finalColor.xyz, 1.0f);
    //backBuffer[DTid.xy] = float4(255,0,0, 1.0f);
}