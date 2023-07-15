#pragma once
#include "State.h"

namespace state
{
	class LoadModules : public State
	{
	public:
		void Init() override;
		void Destroy() override;

		void Tick(double delta) override;
		void Draw(double delta) override;
	};
}