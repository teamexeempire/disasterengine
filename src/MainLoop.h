#pragma once
#include "state/State.h"
#include "ModPack.h"

class MainLoop
{
public:
	static int Run();

	template <typename T>
	static void SwitchState();

private:
	static inline state::State* state = nullptr;
};