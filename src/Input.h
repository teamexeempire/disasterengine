#pragma once
#include <map>
#include <SDL2/SDL.h>

class Input
{
public:
	enum KeyboardKey
	{
		Left = SDLK_LEFT,
		Right = SDLK_RIGHT,
		Up = SDLK_UP,
		Down = SDLK_DOWN,
		Space = SDLK_SPACE,

		Ctrl = SDLK_LCTRL,
		LCtrl = SDLK_LCTRL,
		RCtrl = SDLK_RCTRL,

		Alt = SDLK_LALT,
		LAlt = SDLK_LALT,
		RAlt = SDLK_RALT,

		Shift = SDLK_LSHIFT,
		LShift = SDLK_LSHIFT,
		RShift = SDLK_RSHIFT,

		Tab = SDLK_TAB,
		Caps = SDLK_CAPSLOCK,
		
		A = SDLK_a,
		B = SDLK_b,
		C = SDLK_c,
		D = SDLK_d,
		E = SDLK_e,
		F = SDLK_f,
		G = SDLK_g,
		H = SDLK_h,
		I = SDLK_i,
		J = SDLK_j,
		K = SDLK_k,
		L = SDLK_l,
		M = SDLK_m,
		N = SDLK_n,
		O = SDLK_o,
		P = SDLK_p,
		Q = SDLK_q,
		R = SDLK_r,
		S = SDLK_s,
		T = SDLK_t,
		U = SDLK_u,
		V = SDLK_v,
		W = SDLK_w,
		X = SDLK_x,
		Y = SDLK_y,
		Z = SDLK_z,
	};

	static bool IsKeyDown(int keycode);
	static bool IsKeyPressed(int keycode);
	static bool IsKeyUp(int keycode);

private:
	friend class MainLoop;

	inline static std::map<int, bool> keyStates;
	inline static std::map<int, bool> prevKeyStates;
};