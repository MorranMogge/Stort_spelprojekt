
RWBuffer<float> rawBuffer : register(u0);

cbuffer timeValue : register(b0)
{
    float deltaTime;
};
cbuffer posValue : register(b1)
{
    float3 offsetFromOrigin;
    float enable;
    float3 orientation;
};

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
#define Offset 8
    
#define PositionX rawBuffer[DTid.x * Offset]
#define PositionY rawBuffer[DTid.x * Offset + 1]
#define PositionZ rawBuffer[DTid.x * Offset + 2]
    
#define StartPositionX rawBuffer[DTid.x * Offset + 3]
#define StartPositionY rawBuffer[DTid.x * Offset + 4]
#define StartPositionZ rawBuffer[DTid.x * Offset + 5]
    
#define SimulateTime rawBuffer[DTid.x * Offset + 6]

#define LifeTime rawBuffer[DTid.x * Offset + 7]

#define Speed 1.0f
#define SpeedMultyply 100.0f
    
    // if SimulateTime bigger than LifeTime or not enabled, reset it to 0, else plus delta time
    SimulateTime = SimulateTime > LifeTime || !enable ? 0.0f : SimulateTime + deltaTime;
    
    // up vector from camera
#define CameraUpVector float3(0.0f, 1.0f, 0.0f)

    // euler direction Z
    const float3 eulerZ = normalize(cross(orientation, CameraUpVector));
    
    // euler direction X
    const float3 eulerX = normalize(cross(orientation, eulerZ));
    
// euler direction matrix, 3x3 without translation
#define EulerDirection float3x3(eulerX, orientation, eulerZ)
    
// start position in local space
#define StartPosition float3(StartPositionX, StartPositionY, StartPositionZ)
    
// remap simulate time in range of 0 to 1
#define TimeScale SimulateTime / LifeTime 
    
// oriented/rotated start position in local space
#define OrientedStartPosition mul(EulerDirection, StartPosition)
    
// offset in local space (move particles)
#define LocalOffset (Speed * SpeedMultyply * TimeScale * orientation) 
    
    // position in world space
    const float3 position = OrientedStartPosition + LocalOffset + offsetFromOrigin;
    
    //apply position to buffer
    PositionX = position.x;
    PositionY = position.y;
    PositionZ = position.z;

}