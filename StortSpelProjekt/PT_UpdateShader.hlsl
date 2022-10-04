
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

#define Offset 8
    
#define PositionX rawBuffer[DTid.x * Offset]
#define PositionY rawBuffer[DTid.x * Offset + 1]
#define PositionZ rawBuffer[DTid.x * Offset + 2]

#define SimulateTime rawBuffer[DTid.x * Offset + 3]
    
#define StartPositionX rawBuffer[DTid.x * Offset + 4]
#define StartPositionY rawBuffer[DTid.x * Offset + 5]
#define StartPositionZ rawBuffer[DTid.x * Offset + 6]

#define LifeTime rawBuffer[DTid.x * Offset + 7]

#define Speed 1.0f
#define Range 100.0f

// up vector from camera
#define CameraUpVector float3(0.0f, 1.0f, 0.0f)

// start position in local space
#define StartPosition float3(StartPositionX, StartPositionY, StartPositionZ)

// remap simulate time in range of 0 to 1
#define TimeScale SimulateTime / LifeTime

//LifeTime = 10;

// position offset in local space 
#define LocalOffset Speed * Range * TimeScale

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // if SimulateTime bigger than LifeTime or not enabled, reset it to 0, else add delta time
    SimulateTime = SimulateTime > LifeTime || !enable ? 0.0f : SimulateTime + deltaTime;
    
    const float3 eulerZ = normalize(cross(orientation, CameraUpVector)); // rotation Z
    const float3 eulerX = normalize(cross(orientation, eulerZ)); // rotation X
    
#define EulerRotation float3x3(eulerX, orientation, eulerZ) // rotation matrix, 3x3 without translation
#define RotatedStartPosition mul(EulerRotation, StartPosition) // oriented/rotated start position in local space
#define RotatedLocalOffset (LocalOffset * orientation) // rotated local offset
    
    // position in world space
    const float3 position = RotatedStartPosition + RotatedLocalOffset + offsetFromOrigin;
    
    //apply position to buffer
    PositionX = position.x;
    PositionY = position.y;
    PositionZ = position.z;
}