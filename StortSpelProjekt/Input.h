#pragma once
#define NOMINMAX
#include "stdafx.h"
#include "ImGui/imgui.h"
#include "imGUI/imgui_impl_dx11.h"
#include "imGUI/imgui_impl_win32.h"

enum class KeyCode
{
	MOUSE_L = ImGuiKey_MouseLeft,
	MOUSE_R = ImGuiKey_MouseRight,
	MOUSE_M = ImGuiKey_MouseMiddle,
	//ImGuiKey_MouseWheelX
	//ImGuiKey_MouseWheelY

	ENTER = ImGuiKey_Enter,
	ENTER_KeyPad = ImGuiKey_KeypadEnter,
	ESC = ImGuiKey_Escape,
	SPACE = ImGuiKey_Space,
	TAB = ImGuiKey_Tab,

	ALT = ImGuiKey_LeftAlt,
	ALT_L = ImGuiKey_LeftAlt,
	ALT_R = ImGuiKey_RightAlt,

	CTRL = ImGuiKey_LeftCtrl,
	CTRL_L = ImGuiKey_LeftCtrl,
	CTRL_R = ImGuiKey_RightCtrl,

	SHIFT = ImGuiKey_LeftShift,
	SHIFT_L = ImGuiKey_LeftShift,
	SHIFT_R = ImGuiKey_RightShift,

	PRTSC = ImGuiKey_PrintScreen,
	INS = ImGuiKey_Insert,
	DEL = ImGuiKey_Delete,
	END = ImGuiKey_End,
	HOME = ImGuiKey_Home,
	PGUP = ImGuiKey_PageUp,
	PGDN = ImGuiKey_PageDown,

	ARROW_Up = ImGuiKey_UpArrow,
	ARROW_Down = ImGuiKey_DownArrow,
	ARROW_Right = ImGuiKey_RightArrow,
	ARROW_Left = ImGuiKey_LeftArrow,

	F1 = ImGuiKey_F1,
	F2 = ImGuiKey_F2,
	F3 = ImGuiKey_F3,
	F4 = ImGuiKey_F4,
	F5 = ImGuiKey_F5,
	F6 = ImGuiKey_F6,
	F7 = ImGuiKey_F7,
	F8 = ImGuiKey_F8,
	F9 = ImGuiKey_F9,
	F10 = ImGuiKey_F10,
	F11 = ImGuiKey_F11,
	F12 = ImGuiKey_F12,

	A = ImGuiKey_A,
	B = ImGuiKey_B,
	C = ImGuiKey_C,
	D = ImGuiKey_D,
	E = ImGuiKey_E,
	F = ImGuiKey_F,
	G = ImGuiKey_G,
	H = ImGuiKey_H,
	I = ImGuiKey_I,
	J = ImGuiKey_J,
	K = ImGuiKey_K,
	L = ImGuiKey_L,
	M = ImGuiKey_M,
	N = ImGuiKey_N,
	O = ImGuiKey_O,
	P = ImGuiKey_P,
	Q = ImGuiKey_Q,
	R = ImGuiKey_R,
	S = ImGuiKey_S,
	T = ImGuiKey_T,
	U = ImGuiKey_U,
	V = ImGuiKey_V,
	W = ImGuiKey_W,
	X = ImGuiKey_X,
	Y = ImGuiKey_Y,
	Z = ImGuiKey_Z,

};

class Input final
{
public:

	//return true as long as key holding down
	static bool KeyDown(const KeyCode key)
	{
		return ImGui::IsKeyDown((ImGuiKey)key);
	}

	//return true ONCE key press
	static bool KeyPress(const KeyCode key)
	{
		return ImGui::IsKeyPressed((ImGuiKey)key, false);
	}

	//return true ONCE key release
	static bool KeyUp(const KeyCode key)
	{
		return ImGui::IsKeyReleased((ImGuiKey)key); 
	}

	//call this if imgui didnt exist in current game state
	static void Update()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

};