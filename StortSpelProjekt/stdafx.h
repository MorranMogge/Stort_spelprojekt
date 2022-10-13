#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

//Windows is huge
#include <Windows.h>

//Basic STD
#include <iostream>
#include <array>
#include <string>
#include <unordered_map>


//DirectX
#include <SimpleMath.h>
#include <d3d11.h>

//Ignores warnings created in react library
#pragma warning(push)
#pragma warning(disable : 26495)
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)
