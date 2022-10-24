#pragma once
#define NOMINMAX
#include "stdafx.h"
#include <WinUser.h>

enum class KeyCode
{
	MOUSE_L = 0x01,
	MOUSE_R = 0x02,

	ENTER = 0x0D,
	ESC = 0x1B,
	SPACE = 0x20,
	TAB = 0x09,

	ALT = 0x12,
	ALT_L = 0xA4,
	ALT_R = 0xA5,

	CTRL = 0x11,
	CTRL_L = 0xA2,
	CTRL_R = 0xA3,

	SHIFT = 0x10,
	SHIFT_L = 0xA0,
	SHIFT_R = 0xA1,

	PRTSC = 0x2C,
	INS = 0x2D,
	DEL = 0x2E,
	END = 0x23,
	HOME = 0x24,
	PGUP = 0x21,
	PGDN = 0x22,

	ARROW_Up = 0x26,
	ARROW_Down = 0x28,
	ARROW_Right = 0x27,
	ARROW_Left = 0x25,

	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,

};

class Input final
{
public:

	//return true as long as key holding down
	static bool KeyDown(const KeyCode key)
	{
		if (GetAsyncKeyState((int)key))
		{
			state[key].upTrigged = false;
			return true;
		}
		else if (GetAsyncKeyState((int)key) == 0)
		{
			state[key].trigged = false;
		}
		return false;
	}

	//return true ONCE key press
	static bool KeyPress(const KeyCode key)
	{
		return (KeyDown(key) && !state[key].trigged) ? (state[key].trigged = true) : false;
	}

	//return true ONCE key release
	static bool KeyUp(const KeyCode key)
	{
		return (!KeyDown(key) && !state[key].upTrigged) ? (state[key].upTrigged = true) : false;
	}

private:

	struct State final
	{
		bool trigged : 1 = false;
		bool upTrigged : 1 = true;
	};
	inline static std::unordered_map<KeyCode, State> state;

};