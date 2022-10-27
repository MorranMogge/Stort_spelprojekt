
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

cbuffer camUp : register(b2)
{
    float3 camUpDirection;
    float padding;
};

#define Offset 15
    
#define PositionX rawBuffer[DTid.x * Offset]
#define PositionY rawBuffer[DTid.x * Offset + 1]
#define PositionZ rawBuffer[DTid.x * Offset + 2]

#define SimulateTime rawBuffer[DTid.x * Offset + 3]
    
#define StartPositionX rawBuffer[DTid.x * Offset + 4]
#define StartPositionY rawBuffer[DTid.x * Offset + 5]
#define StartPositionZ rawBuffer[DTid.x * Offset + 6]

#define LifeTime rawBuffer[DTid.x * Offset + 7]

#define CurrentDirectionX rawBuffer[DTid.x * Offset + 8]
#define CurrentDirectionY rawBuffer[DTid.x * Offset + 9]
#define CurrentDirectionZ rawBuffer[DTid.x * Offset + 10]

#define BaseOffSetX rawBuffer[DTid.x * Offset + 11]
#define BaseOffSetY rawBuffer[DTid.x * Offset + 12]
#define BaseOffSetZ rawBuffer[DTid.x * Offset + 13]

#define Speed 0.07f
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
    if (SimulateTime > LifeTime || !enable)
    {
        SimulateTime = 0.0f;
        
        CurrentDirectionX = orientation.x;
        CurrentDirectionY = orientation.y;
        CurrentDirectionZ = orientation.z;
        
        BaseOffSetX = offsetFromOrigin.x;
        BaseOffSetY = offsetFromOrigin.y;
        BaseOffSetZ = offsetFromOrigin.z;

    }
    else
    {
        SimulateTime = SimulateTime + deltaTime;
    }
    
    const float3 currentDirection = float3(CurrentDirectionX, CurrentDirectionY, CurrentDirectionZ);
    
    const float3 eulerZ = normalize(cross(currentDirection, camUpDirection)); // rotation Z
    const float3 eulerX = normalize(cross(currentDirection, eulerZ)); // rotation X
    
#define EulerRotation float3x3(eulerX, currentDirection, eulerZ) // rotation matrix, 3x3 without translation
#define RotatedStartPosition mul(EulerRotation, StartPosition) // oriented/rotated start position in local space
#define RotatedLocalOffset (LocalOffset * currentDirection) // rotated local offset
    
    
    float3 baseOffSet = float3(BaseOffSetX, BaseOffSetY, BaseOffSetZ);
    
    // position in world space
    const float3 position = RotatedStartPosition + RotatedLocalOffset + baseOffSet;
    
    //apply position to buffer
    PositionX = position.x;
    PositionY = position.y;
    PositionZ = position.z;
}