#pragma once
#include <string>

namespace state
{
	class State
	{
	public:
		inline virtual void Init() {}
		inline virtual void Destroy() {}

		inline virtual void Tick(double delta) {}
		inline virtual void Draw(double delta) {}

		inline virtual std::string GetName() const { return "EMPTY"; };
	};
}